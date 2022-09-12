#!/bin/bash
# List pools

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

# Decompose network pool name
POOL_SERVER=`echo "${TEST_POOL}" | ruby -p -e '$_.sub!(%r([^/]+$), "")' | tr -d '\015'`
POOL_BASE=`echo "${TEST_POOL}" | ruby -p -e '$_.sub!(%r(^.*/), "")'`
POOL_BASE=`echo $POOL_BASE | tr -d '\015'`

# We have to do cleanup before returning on error, so record return
# value and drive on.
ret=0

PATH=..:$PATH

for i in 1 2 3; do
    $IV \
    p-create ${POOL_XTRA} -t "${POOL_TYPE}" -s "${POOL_SIZE}" \
        -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}$i"
    [ "$?" != "0" ] && ret=1
done

# There may be other pools on the system, just check if ours exist
POOL_LIST=`$IV p-list $POOL_SERVER | tr -d '\015'`
for i in 1 2 3; do
    # grep rets 0 on a match
    echo ${POOL_LIST} | grep "${POOL_BASE}$i" >>${TEST_LOG}
    [ "$?" != "0" ] && ret=1
done

# Do the same for p-list -i
POOL_LIST=`$IV p-list -i $POOL_SERVER | tr -d '\015'`
for i in 1 2 3; do
    # grep rets 0 on a match
    echo ${POOL_LIST} | grep "${POOL_BASE}$i" >>${TEST_LOG}
    [ "$?" != "0" ] && ret=1
done

for i in 1 2 3; do
    $IV \
    p-stop ${POOL_XTRA} "${TEST_POOL}$i"
    [ "$?" != "0" ] && ret=1
done

exit $(($ret))
