#!/bin/bash

# Test handling of unimplemented/bad type

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=..:${PATH}
$IV \
p-create ${POOL_XTRA} -t unimplemented -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}" 2>>${TEST_LOG}
[ "$?" != "18" ] && exit 1

$IV \
p-create ${POOL_XTRA} -t "" -s "${POOL_SIZE}" \
    -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}" 2>>${TEST_LOG}
[ "$?" != "18" ] && exit 1

$IV \
p-create ${POOL_XTRA} -t \
    unimplemented256characterswilloverflowthefirstsprintfinpoolloadmethodssoitshouldreturnpooltypebadthorsoihopebutsoonwewillfindoutforsuremwhahahahahahahahahahahahahahahahahahahahahahahahahahahayesiamevilmwahahahahahahahahahahahahahahahahahahahahahahahahahaha \
    -s "${POOL_SIZE}" -i "${POOL_TOC_CAPACITY}" "${TEST_POOL}" 2>>${TEST_LOG}
[ "$?" != "18" ] && exit 1

exit 0
