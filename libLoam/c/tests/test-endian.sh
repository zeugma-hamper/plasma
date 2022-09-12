#!/bin/sh

PATH=${PATH}:.

test-endian 16 0xabcd 0xcdab 32 0xdeadbeef 0xefbeadde 64 0x1234567890abcdef 0xefcdab9078563412

exit $?
