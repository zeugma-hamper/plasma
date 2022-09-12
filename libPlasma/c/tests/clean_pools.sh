#!/bin/bash

# Clean up from any previously failed runs of make check.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:${PATH}
$IV \
p-stop "${TEST_POOL}" 2>>/dev/null

# The rest of this won't work on network pools
echo "${TEST_POOL}" | grep -F "://" > /dev/null
[ "$?" != "1" ] && exit 0

for i in `p-list | grep ${TEST_POOL}`; do
    # Perhaps should do something on error...
    $IV \
    p-stop $i
done

exit 0
