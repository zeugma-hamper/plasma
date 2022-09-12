#!/bin/bash

# Test lots of pools, with each reader participating in several pools
#
# Keep in mind that with network pools, each pool participate creates
# another process, so you multiply the number of readers by the number
# of pools they participate in (the "-m" option) to get the total
# number of processes necessary to finish this test.  This should be
# kept below 150 or so in order to avoid tripping the OS X limit of
# 266 processes per user.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:${PATH}
$IV \
matrix_test ${POOL_XTRA} -t "${POOL_TYPE}" \
    -s "${POOL_SIZE}" -i "${POOL_TOC_CAPACITY}" \
    -p 50 -d 30 -r 10 -m 5 "${TEST_POOL}" >>${TEST_LOG}

exit $?
