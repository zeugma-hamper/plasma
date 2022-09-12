#!/bin/bash
# note: when run from ctest on windows, pwd shows a pure unix path,
# so avoid pwd here (it's only needed to follow symlinks, and we don't have any)
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
test-interop \
  $srcdirw/little-endian-protein-version2.bin \
  $srcdirw/big-endian-protein-version2.bin \
  $srcdirw/little-endian-protein.bin \
  $srcdirw/big-endian-protein.bin
exit $?
