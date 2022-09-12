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

PATH=..:.:$PATH

$IV \
bin2yaml $srcdirw/little-endian-protein-version2.bin scratch/yayt-tmp.yaml
[ "$?" != "0" ] && exit 1

$IV \
yaml2bin scratch/yayt-tmp.yaml scratch/yayt-tmp.bin
[ "$?" != "0" ] && exit 1

$IV \
endian_test scratch/yayt-tmp.bin
exit $?
