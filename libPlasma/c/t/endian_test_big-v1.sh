#!/bin/bash
srcdir="$(cd "$(dirname "$0")" && pwd)"
srcdirw="$((cygpath -w "$srcdir" 2> /dev/null || echo "$srcdir")| tr '\\' /)"

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

PATH=.:$PATH

$IV \
endian_test $srcdirw/big-endian-protein.bin
exit $?
