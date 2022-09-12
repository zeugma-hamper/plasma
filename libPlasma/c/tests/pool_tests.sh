#!/bin/sh
set -e
set -x

srcdir=$(cd $(dirname $0); pwd)
PATH="${srcdir}:$PATH"

SRCDIR=$(cd $(dirname $0); pwd)
PATH="${SRCDIR}:$PATH"

# pool_tests depend on being run in this order
${srcdir}/../../../bld/list-runner.sh "$0" \
  clean_pools.sh \
  create_one_pool.sh \
  avoid-fallback.rb \
  gang_loyalty.sh \
  delete_one_pool.sh \
  #
