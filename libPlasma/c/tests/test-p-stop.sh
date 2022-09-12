#!/bin/bash

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:$PATH

# Without -q, deleting a nonexistent pool is an error
$IV \
p-stop ${TEST_POOL}-nonexistent 2>>${TEST_LOG}
[ "$?" != "10" ] && exit 1

# With -q, deleting a nonexistent pool should not be an error
$IV \
p-stop -q ${TEST_POOL}-nonexistent 2>>${TEST_LOG}
[ "$?" != "0" ] && exit 1

exit 0
