#!/bin/bash

# Test that pools can only join one gang

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=.:$PATH

$IV \
doppelganger ${POOL_XTRA} "${TEST_POOL}"

exit $?
