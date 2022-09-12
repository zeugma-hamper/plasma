#!/bin/sh
# Usage: $0 message test1 test2 test3 ...
# Iterate over the tests specified on the commandline, in order
# Announce results with given message
set -x

message="$1"
shift

errors=0
failed=false
for t
do
  if ! $t
  then
    echo "$0: FAIL: $t"
    failed=true
    errors=`expr $errors + 1`
  fi
done
if $failed
then
  echo "${message}: $errors tests failed"
  exit 1
else
  echo "${message}: all tests passed"
fi
