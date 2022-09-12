#!/bin/bash

# The null string is not a valid pool name.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

# Decompose network pool name
POOL_SERVER=`echo "${TEST_POOL}" | ruby -p -e '$_.sub!(%r([^/]+$), "")'`

PATH=..:$PATH
$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${POOL_SERVER}" 2>>${TEST_LOG}

# Should fail
[ "$?" != "16" ] && exit 1
exit 0
