#!/bin/bash

# Try to get a protein with future index

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:$PATH

$IV \
p-nth ${POOL_XTRA} ${TEST_POOL} 2 2>>${TEST_LOG}

# Should fail
[ "$?" != "14" ] && exit 1
exit 0
