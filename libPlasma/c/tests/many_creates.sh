#!/bin/bash

# Create lots of pools over and over in the same process - catches
# leaks.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

if [[ "$OBLONG_TEST_USE_VALGRIND" && "$OBLONG_TEST_USE_POOL_TCP_SERVER" ]]; then
    N=1
else
    N=100
fi

echo $0: N=$N >> ${TEST_LOG}

PATH=.:$PATH
$IV \
many_creates ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" \
    -n $N \
    "${TEST_POOL}"

exit $?
