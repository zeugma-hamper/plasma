#!/bin/sh
set -e
set -x

srcdir=$(cd $(dirname $0); pwd)
PATH="${srcdir}:$PATH"

SRCDIR=$(cd $(dirname $0); pwd)
PATH="${SRCDIR}:$PATH"

# index_tests depend on being run in this order
${srcdir}/../../../bld/list-runner.sh "$0" \
  create_one_pool.sh \
  newest_index_none.sh \
  oldest_index_none.sh \
  add_protein.sh \
  newest_index.sh \
  oldest_index.sh \
  add_protein.sh \
  newest_index_two.sh \
  oldest_index.sh \
  delete_one_pool.sh \
  #
