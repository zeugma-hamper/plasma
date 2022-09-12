#!/bin/bash
srcdir="$(dirname "$0")"
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
spew-test $srcdirw/little-endian-protein-version2.bin $srcdirw/spew-expected.yaml
exit $?
