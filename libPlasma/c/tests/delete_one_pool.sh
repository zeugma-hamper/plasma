#!/bin/bash

# Delete a pool.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:${PATH}
$IV \
p-stop ${POOL_XTRA} ${TEST_POOL}

exit $?
