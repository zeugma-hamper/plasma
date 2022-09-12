#!/bin/sh
# Script to be run by creduce
# Compiles matrix_test.i into matrix_test,
# then checks to see if the resulting
# matrix_test exhibits the target problem.
#
# Takes a lock before running the test
# to let creduce compile all sorts of crazy
# versions of matrix_test.i in parallel
# while oracle.sh is running that last one
# that managed to compile.
# (oracle.sh doesn't support running tests in parallel.)
#
# Note: this can use a lot of system resources.
# Watch free RAM and CPU temperature.
# --n 2 might be the right creduce parameter
# on an 8 core machine with 32MB of RAM...?

set -ex
runid=$$
export runid
LOGFILE=/tmp/delta.$runid.log
export LOGFILE
exec > $LOGFILE 2>&1

# Remove log file if creduce kills us
trap 'rm -f "$LOGFILE"' TERM

SRCDIR=$(cd $(dirname $0); pwd)
if ! make -f $SRCDIR/makefile matrix_test
then
   rm $LOGFILE
   exec /bin/false
fi
flock --close /tmp/delta.lock $SRCDIR/oracle.sh
