#!/bin/bash

# Test that attempting to create a pool that is too small will fail.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:$PATH

# Don't pass 0 for size, because that means "use default size".

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s 1 \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}" 2>>${TEST_LOG}

# Should fail
[ "$?" != "1" ] && exit 1
exit 0
