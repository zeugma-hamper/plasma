#!/bin/bash

# Pass a token between pools, 5 pools, each thread listening on 3 pools

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=.:..:$PATH

ret=0

test_pools=`for i in 1 2 3 4 5; do echo "${TEST_POOL}"$i; done`

test_args="$POOL_XTRA -t $POOL_TYPE -s $POOL_SIZE -i $POOL_TOC_CAPACITY -S 1 -j 3 $test_pools"

#gdb_command_file=`mktemp /tmp/pingpong_test_gdb_command_file.XXXXXXXX` || exit 1

#echo set args $test_args > $gdb_command_file

#gdb_prefix="libtool --mode=execute gdb -x $gdb_command_file"

#$gdb_prefix ./pingpong_test

$IV \
pingpong_test $test_args >> ${TEST_LOG}

[ "$?" != "0" ] && ret=1

# Clean up

p-stop -q $test_pools

#rm -f $gdb_command_file

exit $ret
