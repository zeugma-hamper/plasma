#!/bin/bash

# Run the performance test on a pool

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=.:$PATH

$IV \
perf_test ${POOL_XTRA} -t "${POOL_TYPE}" \
    -s "${POOL_SIZE}" -i "${POOL_TOC_CAPACITY}" \
    -S 1 -n 3 -l 10 "${TEST_POOL}" >>${TEST_LOG}

exit $?
