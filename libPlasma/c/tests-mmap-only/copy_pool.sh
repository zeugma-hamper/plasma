#!/bin/bash

# Tests bug 298.  Similar in spirit to create_noipc_pool.sh, but the
# crucial difference is that in that test, the file still has the same
# inode, so the semaphore is re-created with the same key.  In this test,
# the pool is copied to a different file with a different inode, so the
# semaphore will have a new key, and we'll have to rewrite the config
# file to contain the new key.  (Unfortunately, the code path to rewrite
# the config file has been around for nearly a year but hasn't been
# tested until now.)

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
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

# Stick something in the pool

$IV \
p-deposit ${POOL_XTRA} -d a_descrip -i a_key:a_value ${TEST_POOL}
[ "$?" != "0" ] && exit 1

# Copy the pool

cp -R "$OB_POOLS_DIR_SH"/${TEST_POOL} "$OB_POOLS_DIR_SH"/acopyofapool
[ "$?" != "0" ] && exit 1

# Remove the original pool

$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 1

# Move the copied pool back to the original location
# (Avoids file naming problem "pools/acopyofapool/test_pool.mmap-pool")

mv "$OB_POOLS_DIR_SH"/acopyofapool "$OB_POOLS_DIR_SH"/${TEST_POOL}
[ "$?" != "0" ] && exit 1

# We should be able to use the copied pool under the original name

$IV \
p-deposit ${POOL_XTRA} -d a_descrip -i a_key:a_value ${TEST_POOL} 2>>${TEST_LOG}
[ "$?" != "0" ] && exit 1

$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 1

exit 0
