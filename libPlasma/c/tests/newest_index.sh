#!/bin/bash

# Get newest index after one protein deposited

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:${PATH}
if [ "`$IV p-newest-idx ${POOL_XTRA} ${TEST_POOL} | tr -d '\015'`" != "0" ]; then
    exit 1;
fi

exit $?
