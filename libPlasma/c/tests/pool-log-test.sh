#!/bin/bash

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=.:..:$PATH

$IV \
pool-log-test ${POOL_XTRA} -t "${POOL_TYPE}" \
    -s "${POOL_SIZE}" -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}"
[ "$?" != "0" ] && exit 1

# Oh yeah, now clean up...
$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}
[ "$?" != "0" ] && exit 1

exit 0
