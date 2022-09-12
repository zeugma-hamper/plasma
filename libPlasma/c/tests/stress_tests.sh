#!/bin/sh
set -e
set -x

srcdir=$(cd $(dirname $0); pwd)
PATH="${srcdir}:$PATH"

SRCDIR=$(cd $(dirname $0); pwd)
PATH="${SRCDIR}:$PATH"

# stress_tests depend on being run in this order
${srcdir}/../../../bld/list-runner.sh "$0" \
  perf_test.sh \
  lots_await.sh \
  lots_readers.sh \
  lots_depositors.sh \
  lots_rw.sh \
  lots_pools.sh \
  delete_pools.sh \
  #
