#!/bin/bash

# Awaitful reading with some pools with no depositors

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
    -a 0 -p 100 -d 10 -r 10 -m 5 "${TEST_POOL}" >>${TEST_LOG}

exit $?
