#!/bin/bash

# Get the last protein

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:$PATH

$IV \
p-nth ${POOL_XTRA} ${TEST_POOL} -1 >>${TEST_LOG}

exit $?
