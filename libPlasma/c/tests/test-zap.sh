#!/bin/bash

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:$PATH

# Create a pool
$IV \
p-create -z ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

# And stick something in it
$IV \
p-deposit ${POOL_XTRA} -d a_descrip -i a_key:a_value ${TEST_POOL}
[ "$?" != "0" ] && exit 1

# Creating it again should fail
$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}" 2>>${TEST_LOG}
[ "$?" != "12" ] && exit 1

# Although with -q we shouldn't see an error
$IV \
p-create -q ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

# There should still be something in it
$IV \
p-oldest-idx ${POOL_XTRA} ${TEST_POOL} 1>>${TEST_LOG}
[ "$?" != "0" ] && exit 1

# Now zap it
$IV \
p-create -z ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

# And it should be empty
$IV \
p-oldest-idx ${POOL_XTRA} ${TEST_POOL} 2>>${TEST_LOG}
[ "$?" != "14" ] && exit 1

# Oh yeah, now clean up...
$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 1

exit 0
