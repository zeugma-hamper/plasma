#!/bin/bash

# Call p-deposit without args

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=${PATH}:..
$IV \
p-deposit ${POOL_XTRA} 2>>${TEST_LOG}

# Should fail
[ "$?" != "1" ] && exit 1
exit 0
