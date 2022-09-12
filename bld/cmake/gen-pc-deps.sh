#!/bin/sh
# Rough script to generate the list of dependencies for pc files
me=`basename $PWD`
cat *.[cCh] | grep '^#include.*/' | sed 's,.*<,,;s,/c++,++,;s,/.*,,;s/.*"//' | sort -u | grep . | grep -v boost | grep -w -v $me
