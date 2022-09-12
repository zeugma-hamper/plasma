#!/bin/bash

# Test that attempting to create a pool name that is too big will fail.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:$PATH

# Size should be maximum unsigned 64 bit integer

SIZE=18446744073709551615

$IV \
p-create -t "${POOL_TYPE}" -s ${SIZE} \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}" 2>>${TEST_LOG}

# Should fail

[ "$?" != "1" ] && exit 1

# Make sure it didn't leave any turds lying around by trying to create
# the pool again with a reasonable size

$IV \
p-create -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"

[ "$?" != "0" ] && exit 1

# And destroy the test pool

$IV \
p-stop "${TEST_POOL}"

exit $?
