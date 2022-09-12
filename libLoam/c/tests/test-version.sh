#!/bin/sh
set -e
set -x

PATH=.:$PATH

# Show output, and run any internal tests
test-version

get_field() {
    test-version | grep $1 | sed 's/.*{//;s/}.*//;s/ *$//'
}

check_kernel_unix() {
    expected=`uname -r`
    observed=`get_field kernel`
    if test "$expected" != "$observed"
    then
        echo "kernel: expected $expected, observed $observed"
        exit 1
    fi
}

check_libc_linux() {
    expected=`/usr/bin/getconf GNU_LIBC_VERSION`;
    observed=`get_field libc`
    if test "$expected" != "$observed"
    then
        echo "libc: expected $expected, observed $observed"
        exit 1
    fi
}

check_cpu_linux() {
    expected=`grep 'model name' < /proc/cpuinfo | sed 's/.*:\s*//;s/ *$//' | uniq`
    observed=`get_field cpu`
    if test "$expected" != "$observed"
    then
        echo "cpu: expected $expected, observed $observed"
        exit 1
    fi
}

case "`uname`" in
CYGWIN*)
    # GetVersionEx is broken above Win 8.0.  FIXME: use RtlGetVersion().
    echo "sorry, Windows hides its true nature from the user these days."
    ;;
Darwin)
    check_kernel_unix
    ;;
Linux)
    check_kernel_unix
    check_libc_linux
    check_cpu_linux
    ;;
*)
    echo "test-version.sh: unknown system `uname`"
    exit 1
    ;;
esac
