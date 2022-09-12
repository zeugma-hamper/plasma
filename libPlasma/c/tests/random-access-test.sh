#!/bin/bash

# Use -S to always ask for statistics, then redirect them to the log file.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=.:$PATH

$IV \
random-access-test ${POOL_XTRA} -S -t "${POOL_TYPE}" \
    -s "${POOL_SIZE}" -i "${POOL_TOC_CAPACITY}" \
    "${TEST_POOL}" >>${TEST_LOG}

exit $?
