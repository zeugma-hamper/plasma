#!/bin/bash

# if [ "$OBLONG_TEST_USE_VALGRIND" -a "$OBLONG_TEST_USE_POOL_TCP_SERVER" ]; then
#     echo Skipping $0.
#     echo "  " The reason for skipping is that it sometimes hangs
#     echo "  " when the following vars are both non-empty:
#     echo "    " OBLONG_TEST_USE_VALGRIND \(currently $OBLONG_TEST_USE_VALGRIND\)
#     echo "    " OBLONG_TEST_USE_POOL_TCP_SERVER \(currently $OBLONG_TEST_USE_POOL_TCP_SERVER\)
# fi

# Run the await test on a pool

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:${PATH}
$IV \
await_test ${POOL_XTRA} -t "${POOL_TYPE}" \
    -s "${POOL_SIZE}" -i "${POOL_TOC_CAPACITY}" \
    -u 10 \
    -S 1 "${TEST_POOL}"
exit $?
