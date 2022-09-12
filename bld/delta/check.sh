#!/bin/sh

# note: this must match values in oracle.sh and makefile
READERS=30
DEPOSITORS=0

set -e

ASAN="on"
# FIXME - assumes source tree is $HOME/src/yovo, and this is linux
CMAKE_BINARY_DIR="$HOME/src/yovo/obj-x86_64-linux-gnu"
CMAKE_SOURCE_DIR="$HOME/src/yovo"
COVERAGE="OFF"
TSAN="OFF"

# The test fixtures this script implements
yt_fixtures_srctop="$CMAKE_SOURCE_DIR"/bld/cmake/fixtures
yt_all_fixtures="$(ls "$yt_fixtures_srctop")"

# Remove /opt/oblong* from PATH to avoid running against old installs
PATH="$(echo "$PATH" | tr : '\012' | grep -v /opt/oblong | tr '\012' :)"
# Put all the directories containing just-built binaries on the PATH
#PATH="${ALL_BIN_DIRS_ABS_COLONSEP}:${PATH}"
PATH=$HOME/src/yovo/obj-x86_64-linux-gnu/libLoam/c:$HOME/src/yovo/obj-x86_64-linux-gnu/libLoam/c/tests:$HOME/src/yovo/obj-x86_64-linux-gnu/libPlasma/c:$HOME/src/yovo/obj-x86_64-linux-gnu/libPlasma/c/tests:$HOME/src/yovo/obj-x86_64-linux-gnu/libPlasma/c/t:$HOME/src/yovo/obj-x86_64-linux-gnu/libPlasma/c/tests-mmap-only:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin:/var/lib/snapd/snap/bin:/snap/bin:/var/lib/snapd/snap/bin:
yt_poolserver=false
# Test work area, created by start, deleted by stop
yt_dir="${CMAKE_BINARY_DIR}"/yotest.d

# how many seconds poolserver sleeps during shutdown
yt_cooldown=3

# Get shell and env vars for the given fixture type (call before start)
get_fixture_vars() {
    local fix
    fix=$1
    yt_fixture_src="$yt_fixtures_srctop"/$fix
    yt_fixture_log="$yt_dir"/$fix/pool_tcp_server.log
    yt_fixture_pidfile="$yt_dir"/$fix/pool_tcp_server-P.out.txt

    # Don't want to find any files in /etc/oblong during tests,
    # but do want to find this fixture's certificates.
    OB_ETC_PATH="$yt_fixture_src"
    export OB_ETC_PATH
    OB_POOLS_DIR="$yt_dir"/$fix/var/pools
    export OB_POOLS_DIR
}

# Get info about the given running fixture
get_pid() {
    # duplicates yt_fixture_pidfile from get_fixture_vars
    awk '/pid:/ {print $2}' < "$yt_dir/$1"/pool_tcp_server-P.out.txt | tr -d '\015'
}
get_port() {
    # duplicates yt_fixture_pidfile from get_fixture_vars
    awk '/port:/ {print $2}' < "$yt_dir/$1"/pool_tcp_server-P.out.txt | tr -d '\015'
}

# Get test pool URL for the given fixture type; fixture must be running
get_url() {
    local fix
    fix=$1

    case "$fix" in
    local)
        echo "test_pool"
        return 0
        ;;
    tcp|tcps|tcpo)
        echo "$fix://localhost:$(get_port "$fix")/test_pool"
        return 0
        ;;
    esac

    echo "yotest: get_url: Unknown test fixture '$fix'" >&2
    # in case script doesn't die:
    echo "get_url:fatal-bad-fix-$fix"
    exit 1
}

# Return true (0) if cmake was called with -DCOVERAGE=ON, false (1) otherwise
built_for_coverage() {
    case "$(echo "$COVERAGE" | tr A-Z a-z)" in
    1|yes|on|true) return 0;;
    *) return 1;;
    esac
}

# Return true (0) if cmake was called with -DASAN=ON, false (1) otherwise
built_for_asan() {
    case "$(echo "$ASAN" | tr A-Z a-z)" in
    1|yes|on|true) return 0;;
    *) return 1;;
    esac
}

# Return true (0) if cmake was called with -DTSAN=ON, false (1) otherwise
built_for_tsan() {
    case "$(echo "$TSAN" | tr A-Z a-z)" in
    1|yes|on|true) return 0;;
    *) return 1;;
    esac
}

# Set environment variables used by sanitizers
get_sanitizer_vars() {
    if built_for_tsan
    then
        # Thread sanitizer
        if test "$TSAN_OPTIONS" != ""
        then
            echo "yotest: warning: overriding TSAN_OPTIONS.  FIXME"
        fi
        # From https://github.com/google/sanitizers/wiki/ThreadSanitizerFlags
        # alphabetical order, please
        TSAN_OPTIONS="suppressions=$CMAKE_SOURCE_DIR/bld/cmake/tsan-suppressions.txt"
        export TSAN_OPTIONS
    elif built_for_asan
    then
        # ASAN implies leak sanitizer, too
        if test "$LSAN_OPTIONS" != ""
        then
            echo "yotest: warning: overriding LSAN_OPTIONS.  FIXME"
        fi
        # alphabetical order, please
        LSAN_OPTIONS="print_suppressions=1:report_objects=1:verbosity=1:log_threads=1:suppressions=$CMAKE_SOURCE_DIR/bld/cmake/lsan-suppressions.txt"
        export LSAN_OPTIONS
        echo "yotest: LSAN_OPTIONS=$LSAN_OPTIONS"

        # Address sanitizer
        if test "$ASAN_OPTIONS" != ""
        then
            echo "yotest: warning: overriding ASAN_OPTIONS.  FIXME"
        fi
        # alphabetical order, please
        # Can't turn on check_initialization_order=1 or strict_init_order=1 yet,
        # as it flags reference counting for static slaws like A_NULL_SLAW
        ASAN_OPTIONS="check_initialization_order=0 detect_odr_violation=2 detect_stack_use_after_return=1 strict_init_order=0"
        export ASAN_OPTIONS

        # For tests that load a g-speak plugin into a prebuilt non-asan executable,
        # tell the test script where to find asan to preload it.
        # Currently only used by libPlasma/ruby/test-helpers.rb
        if test -f /etc/issue
        then
            # assume we want the latest... might fail if you install gcc8 on ubu1804 but are using gcc7?
            ASAN_PRELOAD=$(ls /usr/lib/$(dpkg-architecture -q DEB_TARGET_GNU_TYPE)/libasan.so.? | tail -n 1)
        elif test -d /Applications
        then
            ASAN_PRELOAD=$(find $(xcode-select -p) -name '*asan_osx_dynamic.dylib')
        else
            ASAN_PRELOAD=unknown-system-type
        fi
        if ! test -f $ASAN_PRELOAD
        then
            echo "yotest: error: cannot find asan library to preload"
            exit 1
        fi
        export ASAN_PRELOAD
    fi
}
do_run() {
    local fix
    fix=$1
    shift
    local cmd
    cmd=$1
    get_fixture_vars "$fix"

    # Every test gets a new scratch directory
    # This assumes we never want to run tests in parallel.
    rm -rf scratch
    mkdir scratch

    # Tell libPlasma/ruby/test-helpers.rb that it's always ok to run tests
    # FIXME: remove all references to this variable once we remove automake build?
    export OBLONG_YOVO_BUILD_TYPE=shared

    # Assume that test names are globally unique, and put each test's
    # output at the top of the fixture working directory, named after the test.
    local test_name
    test_name="$(echo "$cmd" | sed 's,.*/,,;s,\.*,,')"
    TEST_LOG="$yt_dir/$fix/$test_name".log
    export TEST_LOG

    # If this isn't a multiple of 4096, test-info fails
    export POOL_SIZE="1048576"
    export POOL_TOC_CAPACITY="50"
    export POOL_TYPE="mmap"   # note: this appears to be obsoleted by method from url?

    # Note: many tests assume srcdir=${PROJECT_SOURCE_DIR}, but we can't set
    # that in this script; assume cmake set that for us.

    # Several tests use this to find scripts in the bld directory
    abs_top_srcdir="${CMAKE_SOURCE_DIR}"
    export abs_top_srcdir

    # OB_TEST_PROCTOR is only set when running tests, so our C++ code
    # can avoid doing things that screw up tests.  Only one thing uses it so far.
    OB_TEST_PROCTOR=YEP
    export OB_TEST_PROCTOR

    TEST_POOL="$(get_url "$fix")"
    export TEST_POOL

    case "$fix" in
    local)
        # some tests assume remote pool if this is set
        unset OBLONG_TEST_USE_POOL_TCP_SERVER
        ;;
    tcp*)
        # Some tests sense this to check for TCP pool type...
        # and if it's not identical to the one we just built,
        # assumes it's an archived old version!
        OBLONG_TEST_USE_POOL_TCP_SERVER="$yt_poolserver"
        export OBLONG_TEST_USE_POOL_TCP_SERVER

        # Get apps that try to use a local pool to fail
        OB_POOLS_DIR=/dev/null
        ;;
    *)
        echo "yotest: Unknown test fixture '$fix'"
        exit 1
    esac

    # In case this is an address or thread sanitizer run, set appropriate environment variables
    get_sanitizer_vars

    env | sort
    echo "yotest: run $test_name with pool $TEST_POOL"
    ./matrix_test ${POOL_XTRA} -t "${POOL_TYPE}" \
    -s "${POOL_SIZE}" -i "${POOL_TOC_CAPACITY}" \
    -r $READERS -d $DEPOSITORS \
    "${TEST_POOL}" #>>${TEST_LOG}
}

do_run tcpo lots_readers.sh
