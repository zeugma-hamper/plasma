#!/bin/sh
exec /opt/yobuild/bin/libtool --mode=execute valgrind --quiet --suppressions=../bld/valgrind.supp --leak-check=full ./pool_tcp_server -n
