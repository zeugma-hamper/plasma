#!/bin/bash

# What if we need to rewrite the config file and it isn't rewritable?
# Do we at least avoid horrible crashing, etc?

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
p-create -G ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 2

# Remove pool semaphores

$IV \
p-sleep ${POOL_XTRA} "${TEST_POOL}" >>${TEST_LOG}
[ "$?" != "0" ] && exit 3

# make config file read-only
chmod a-w "$OB_POOLS_DIR_SH"/${TEST_POOL}/pool.conf

# This should fail with an error but not segfault

$IV \
p-deposit ${POOL_XTRA} -d a_descrip -i a_key:a_value ${TEST_POOL} 2>>${TEST_LOG}
result=$?

# Fix pool
chmod u+w "$OB_POOLS_DIR_SH"/${TEST_POOL}/pool.conf

# Now delete it
$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 4

# Aaaaand just for jollies, create and delete one more time to check
# for leftover cruft

$IV \
p-create -G ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 5

$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 6

# p-deposit returns 1 on a "normal" failure
[ "$result" != "1" ] && exit 7

exit 0
