#!/bin/sh
# Script to generate certificates for yovo unit tests
# Assumes current directory is bld/cmake/fixtures/tcps
set -ex
PATH=../../../../libPlasma/c:$PATH

# Use security level 3, generate certificates good for one year, and expect developer to check them into git
# FIXME: run this automatically if certificates are expired?
# FIXME: revisit whether we can use security level 4 sometime soon
rm -f *.pem
ob-plasma-cert.sh -t 365 -s 3 mkca
ob-plasma-cert.sh -t 365 -s 3 mkserver localhost
ob-plasma-cert.sh -t 365 -s 3 mkclient
