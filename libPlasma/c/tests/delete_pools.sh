#!/bin/bash

# Delete all the pools created by the matrix tests.  If $TEST_POOL is
# "testpool", they look like
#
# testpool1 testpool2 ... testpool100 (or so)

# XXX below doesn't work for network pools (unless client and server
# are using the same OB_DIR).
#
#POOL_LIST=`../p-list`
#for i in ${POOL_LIST}; do

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

# hackish sleep to avoid the 4ac88b7a issue discussed below
sleep 10

# Use nifty bug 1021 wildcard feature

PATH=..:${PATH}
$IV \
p-stop -q -w ${POOL_XTRA} \
  ${TEST_POOL} ${TEST_POOL}100 ${TEST_POOL}'[1-9][0-9]' ${TEST_POOL}'[0-9]'

exit $?

# DEAD CODE BELOW THIS LINE -- REMOVE LATER?

# There may be a need to insert a sleep here if the problem described
# in commit 4ac88b7a crops up again.  It may crop up again since the
# hack of going backwards will impose less of a delay now that we're
# using the new "multi-stop" feature.

# For the record, I could not reproduce the 4ac88b7a on coconut by
# changing the loop to go forwards instead of backwards.

# It isn't strictly necessary to stop plain $TEST_POOL,
# i.e. non-numbered $TEST_POOL, but doing so helps prevent cascading
# failures.

# POOL_LIST=$TEST_POOL
# for (( i=100; i>=0; --i )); do
#     POOL_LIST="$POOL_LIST $TEST_POOL$i"
# done
#
# $IV \
# ../p-stop -q ${POOL_XTRA} $POOL_LIST
#
# exit $?
