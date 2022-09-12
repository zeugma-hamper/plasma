#!/bin/bash

#
# Add up the output from matrix_test to get total reads and deposits.
#
# Input looks like so:
#
# reader 10
# reader 22
# depositor 3
# reader 10
# depositor 8
#
# Output looks like so:
#
# <number of reads> <number of deposits>
#

awk 'BEGIN {results["reader"] = 0; results["depositor"] = 0;}{results[$1] += $2};END {print results["reader"] " " results["depositor"]}'
