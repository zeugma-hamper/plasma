#!/bin/bash

# Test 50 total readers and depositors - OS X per-user process limit
# is 266, there are about 50 processes or so to start with, then 50
# total readers and writers, and if the server is local, you'll have
# 50 pool server threads too.

# Delay to allow old processes to die (bug 4592)
sleep 2

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
    -d 25 -r 25 "${TEST_POOL}" >>${TEST_LOG}

exit $?
