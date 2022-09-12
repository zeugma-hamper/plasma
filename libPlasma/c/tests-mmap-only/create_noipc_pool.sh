#!/bin/bash

# Remove the SysV semaphores for this pool (as would happen on boot)
# and see if we can still do pool operations.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=${PATH}:..
TEST_POOL=${TEST_POOL}-$(basename $0)

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

# Remove pool semaphores

$IV \
p-sleep ${POOL_XTRA} "${TEST_POOL}" >>${TEST_LOG}
[ "$?" != "0" ] && exit 1

# Pool should still exist and be usable

$IV \
p-deposit ${POOL_XTRA} -d a_descrip -i a_key:a_value ${TEST_POOL}
[ "$?" != "0" ] && exit 1

# Remove pool semaphores

$IV \
p-sleep ${POOL_XTRA} "${TEST_POOL}" >>${TEST_LOG}
[ "$?" != "0" ] && exit 1

# Second create should fail

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}" 2>>${TEST_LOG}
[ "$?" = "0" ] && exit 1

# Remove pool semaphores

$IV \
p-sleep ${POOL_XTRA} "${TEST_POOL}" >>${TEST_LOG}
[ "$?" != "0" ] && exit 1

# Now delete it
$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 1

# Aaaaand just for jollies, create and delete one more time to check
# for leftover cruft

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 1

exit 0
