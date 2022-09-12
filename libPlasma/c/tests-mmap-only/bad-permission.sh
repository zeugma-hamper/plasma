#!/bin/bash

# Test that pools with the wrong permissions don't cause segfaults

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
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

TEST_POOL_DIR="$OB_POOLS_DIR"/$TEST_POOL
TEST_POOL_DIR_SH="$OB_POOLS_DIR_SH"/$TEST_POOL

# Make pool non-writable
find "$TEST_POOL_DIR_SH" -type f | xargs chmod a-w 2>/dev/null
if [ "$?" != "0" ]; then
  echo "could not find pool files in $TEST_POOL_DIR_SH or could not make them non-writeable" 1>&2
  exit 1
fi

# This should fail with an error but not segfault
$IV \
p-nth ${POOL_XTRA} ${TEST_POOL} 0 2>>${TEST_LOG}
result=$?

# Fix pool
find "$TEST_POOL_DIR_SH" -type f | xargs chmod u+w
if [ "$?" != "0" ]; then
  echo "could not find pool files in $TEST_POOL_DIR_SH or could not make them writeable" 1>&2
  exit 1
fi

$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 1

# p-nth returns 1 on a "normal" failure
[ "$result" != "1" ] && exit 1

exit 0
