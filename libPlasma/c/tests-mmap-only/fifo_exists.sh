#!/bin/bash

# Test fifo collision handling

PATH=${PATH}:..

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

TEST_POOL=${TEST_POOL}-$(basename $0)

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

$IV \
./fifo_exists ${POOL_XTRA} "${TEST_POOL}" 2>>${TEST_LOG}
result=$?

$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 1

[ "$result" != "0" ] && exit 1

exit 0
