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
testvcoerce > scratch/testvcoerce.out
[ "$?" != "0" ] && exit 1

exec diff -ub scratch/testvcoerce.out $srcdir/testvcoerce.expected
