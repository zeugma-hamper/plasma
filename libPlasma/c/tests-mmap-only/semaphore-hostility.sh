#!/bin/bash

PATH=${PATH}:..:.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

$IV \
semaphore-hostility ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
                      "${TEST_POOL}"

exit $?
