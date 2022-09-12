#!/bin/bash
# Try various characters which should result in POOL_POOLNAME_BADTH

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

# Decompose network pool name
POOL_SERVER=`echo "${TEST_POOL}" | ruby -p -e '$_.sub!(%r([^/]+$), "")'`

PATH=..:${PATH}

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    "${POOL_SERVER}illegal/to/contain/a\\backslash" 2>>${TEST_LOG}
[ "$?" != "16" ] && exit 1

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    "${POOL_SERVER}illegal/to/contain/../doubledot" 2>>${TEST_LOG}
[ "$?" != "16" ] && exit 1

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    "${POOL_SERVER}illegal/to/contain//doubleslash" 2>>${TEST_LOG}
[ "$?" != "16" ] && exit 1

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    "${POOL_SERVER}.illegal/to/start/with/dot" 2>>${TEST_LOG}
[ "$?" != "16" ] && exit 1

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    "${POOL_SERVER}/illegal/to/start/with/slash" 2>>${TEST_LOG}
[ "$?" != "16" ] && exit 1

$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    "${POOL_SERVER}illegal/to/end/with/slash/" 2>>${TEST_LOG}
[ "$?" != "16" ] && exit 1

# Completely empty pool name should be illegal, too
$IV \
p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
    "${POOL_SERVER}" 2>>${TEST_LOG}
[ "$?" != "16" ] && exit 1

exit 0
