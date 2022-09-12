#!/bin/bash
# Test that can can round-trip a protein that contains invalid UTF-8.

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
bin2yaml $srcdirw/kp_enter.bin scratch/386-tmp.yaml
[ "$?" != "0" ] && exit 1

$IV \
yaml2bin scratch/386-tmp.yaml scratch/386-tmp.bin
[ "$?" != "0" ] && exit 1

# XXX: Might eventually want to switch to a custom slaw-compare utility,
# because:
#    this test will fail on big-endian systems, because kp_enter.bin
#    is little-endian, but 386-tmp.bin is native endian.
exec cmp $srcdir/kp_enter.bin scratch/386-tmp.bin
