#!/bin/bash

# Test the bug 1088 issue - ACLs causing mkfifo to fail (on OS X)

PATH=${PATH}:..

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

DUMMY_POOL=${TEST_POOL}-DUMMY
TEST_POOL=${TEST_POOL}-$(basename $0)
TEST_POOL_DIR=$OB_POOLS_DIR_SH
LAMARCK=`uname`

# Create a dummy pool to make sure the pool dir exists
$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" "${DUMMY_POOL}"
[ "$?" != "0" ] && exit 1

# Add the ACL
if [ "${LAMARCK}" == "Darwin" ]; then
    chmod +a \
        "user:_spotlight allow list,search,file_inherit,directory_inherit" \
        ${TEST_POOL_DIR}
fi

# Running await_test will exercise mkfifo
$IV \
await_test ${POOL_XTRA} -t "${POOL_TYPE}" \
    -s "${POOL_SIZE}" -i "${POOL_TOC_CAPACITY}" \
    -u 2 \
    -S 1 "${TEST_POOL}"
result=$?

# Remove the ACL
if [ "${LAMARCK}" == "Darwin" ]; then
    chmod -a# 0 ${TEST_POOL_DIR}
fi

# Remove dummy pool
$IV \
p-stop ${POOL_XTRA} ${DUMMY_POOL}
[ "$?" != "0" ] && exit 1

exit $result
