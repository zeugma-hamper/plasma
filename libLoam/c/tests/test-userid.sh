#!/bin/sh

PATH=${PATH}:.
mkdir -p scratch
test-userid `id -un` `id -u` scratch/test-userid-self-test.txt

exit $?
