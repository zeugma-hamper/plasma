#!/bin/bash

# Test backward compatibility for mmap pools by resurrecting a really
# old pool from a tar file and see if it can still be read.

srcdir="$(cd "$(dirname "$0")" && pwd)"
PATH=${PATH}:..:.

# Handle internal valgrind
IV=""
if [ "$#" == "2" ]; then
    if [ "$1" == "--internal-valgrind" ]; then
        IV=$2
    fi
fi

# Create a dummy pool so the pools dir will exist
$IV \
p-create "dummy pool"
[ "$?" != "0" ] && exit 1

# Resurrect old pool from tar
tar -xjf $srcdir/arf-coord.tar.bz2 -C "$OB_POOLS_DIR_SH"

# See if the pool contains what it should
$IV \
compare-pool-and-file arf-coord $srcdir/arf-coord.yaml
[ "$?" != "0" ] && exit 1

# Try depositing something to the pool
$IV \
p-deposit -d a_descrip -i a_key:a_value arf-coord 2>>${TEST_LOG}
[ "$?" != "0" ] && exit 1

# And now delete it and the dummy pool
$IV \
p-stop arf-coord "dummy pool"
[ "$?" != "0" ] && exit 1

exit 0
