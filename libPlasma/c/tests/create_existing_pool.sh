#!/bin/bash

# Create an existing pool and make sure nothing bad happens

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:${PATH}
$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

# Second create should fail

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}" 2>>${TEST_LOG}
[ "$?" != "12" ] && exit 1

# Make sure the pool is still there and usable

$IV \
p-deposit ${POOL_XTRA} -d a_descrip -i a_key:a_value ${TEST_POOL}
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

exit $?
