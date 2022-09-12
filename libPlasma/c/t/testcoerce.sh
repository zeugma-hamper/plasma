#!/bin/bash
srcdir="$(cd "$(dirname "$0")" && pwd)"

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=.:$PATH

$IV \
testcoerce > scratch/testcoerce.out
[ "$?" != "0" ] && exit 1

exec diff -ub scratch/testcoerce.out $srcdir/testcoerce.expected
