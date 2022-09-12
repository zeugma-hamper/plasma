#!/bin/sh
set -e
set -x

# RecentServerOnly fails if any pools exist, so nuke all the pools
env

PATH=.:..:$PATH

case $TEST_POOL in
tcp*)
    p-stop -w "$(echo $TEST_POOL | sed 's,[^/]*$,*,')" || true
    ;;
*)
    p-stop -w '*' || true
    ;;
esac


RecentServerOnly "$@"
