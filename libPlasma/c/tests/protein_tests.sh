#!/bin/sh
set -e
set -x

srcdir=$(cd $(dirname $0); pwd)
PATH="${srcdir}:$PATH"

# protein_tests depend on being run in this order
${srcdir}/../../../bld/list-runner.sh "$0" \
  create_one_pool.sh \
  get_protein_no_proteins.sh \
  add_protein_no_args.sh \
  add_protein.sh \
  get_protein.sh \
  get_last_protein.sh \
  add_protein.sh \
  get_second_protein.sh \
  get_negative_index.sh \
  get_future_index.sh \
  get_no_pool.sh \
  delete_one_pool.sh \
  #
