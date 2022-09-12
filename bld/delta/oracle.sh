#!/bin/sh
# Run the test and exit with status 0, and prints VICTORY, if the target problem is detected.
# Exit with nonzero status on any other kind of failure, or if the test succeeds,
# and prints FAILURE along with some explanation on the cause.
# Somewhat fragile and timing-dependent.

set -ex

# delta.sh will set runid and redirect output to a log file for us...
if test "$runid" = ""
then
  runid=$$
fi

# see also makefile and check.sh
# FIXME: only set these in one file
READERS=30
DEPOSITORS=0
CONNECTIONS=$(expr $READERS + $DEPOSITORS)

MAXWORKERS=$(expr $CONNECTIONS + 10)
MINREADERS=$(expr $READERS - 2)
MINDEPOSITORS=$(expr $DEPOSITORS - 2)
MINCONNECTIONS=$(expr $MINREADERS + $MINDEPOSITORS)

SRCDIR=$(cd $(dirname $0); pwd)

# Return age of given file in seconds
file_age() {
  perl -e "printf('%d', 24*60*60.0 * -M '$1' )"
}

compile_test() {
  if ! make -f $SRCDIR/makefile matrix_test
  then
    declare_failure "compile_test: make failed"
  fi
}

start_test() {
  # Creates check.log.*
  rm -f check.log.* make.log
  make -f $SRCDIR/makefile check > make.log 2>&1 &
}

test_hung() {
  if grep '^TEST FAILED' make.log
  then
    declare_failure "test_hung: make.log says TEST FAILED"
  fi
  latest=$(ls -t | grep check.log | head -n 1)
  if test "$latest" = ""
  then
    # No results yet
    return 1
  fi

  if grep ERROR:.AddressSanitizer $latest
  then
    declare_failure "test_hung: asan error"
  fi
  age=$(file_age $latest)
  if test $age -gt 10
  then
    echo "sanity:check: hang detected"
    return 0
  fi
  # No hang
  return 1
}

declare_failure() {
  echo "FAILURE: $1"
  killall make > /dev/null 2>&1 || true
  killall matrix_test > /dev/null 2>&1 || true
  if false
  then
    latest=$(ls -tr check.log.* | tail -n 1)
    cat make.log || true
    if test "$latest" != ""
    then
      cat $latest || true
    fi
  else
    # failures aren't too interesting...
    # if delta.sh set LOGFILE, remove that file.
    if test "$LOGFILE" != ""
    then
      rm -f "$LOGFILE" || true
    fi
  fi

  # 'exit 1' is not strong enough, does not reliably terminate, but 'exec false' is reliable.
  exec /bin/false
}

declare_victory() {
  killall make || declare_failure "no make?"
  killall matrix_test || declare_failure "no matrix_test?"
  latest=$(ls -tr check.log.* | tail -n 1)
  if test "$latest" = ""
  then
    declare_failure "no latest log?"
  fi
  cat make.log || true
  cat $latest
  echo "VICTORY!"
  cp matrix_test.i /var/tmp/delta.$runid.matrix_test.i
  # 'exit 0' is not strong enough, does not reliably terminate, but 'exec true' is reliable.
  exec /bin/true
}

list_test() {
  ps augxw | grep -v grep | grep matrix_test
}

case $(cat /proc/sys/kernel/yama/ptrace_scope) in
0) ;;
*) declare_failure "cannot ptrace.  Please do 'echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope'"
esac

start_test

TIMEOUT=20

# Wait until one iteration finishes.
n=0
while test $n -lt $TIMEOUT
do
  if test -f check.log.2
  then
    break
  fi
  sleep 1
  if test -f check.log.1 && test_hung
  then
    break
  fi
  n=$(expr $n + 1)
done

TIMEOUT=100
# Wait until timeout.  If it fails or hangs, stop waiting.
while test $n -lt $TIMEOUT
do
  if test_hung
  then
    break
  fi
  sleep 1
  n=$(expr $n + 1)
done

if test "$latest" = ""
then
  echo "wtf"
  declare_failure "no latest log"
fi

# One last sanity check.  We expect at most two of the workers to hang early.
if test $(grep '^reader' < $latest | wc -l) -lt $MINREADERS
then
  declare_failure "TEST FAILED, READERS"
fi
if test $(grep '^depositor' < $latest | wc -l) -lt $MINDEPOSITORS
then
  declare_failure "TEST FAILED, DEPOSITORS"
fi
if test $(grep 'Connected securely' < $latest | wc -l) -lt $MINCONNECTIONS
then
  declare_failure "TEST FAILED, CONNECTIONS"
fi

# Is the first straggler stuck where we expect?
pid=$(list_test | grep R | awk '{print $2}' | head -n 1)
case "$pid" in
[0-9]*) ;;
*) declare_failure "bad pid";;
esac

$SRCDIR/btrace $pid

if $SRCDIR/btrace $pid | grep internal_sched_yield
then
    echo "The target problem was detected.  Keep this change."
    declare_victory
else
    echo "The target problem was NOT detected.  Roll back this change."
    declare_failure "wrong stacktrace"
fi
