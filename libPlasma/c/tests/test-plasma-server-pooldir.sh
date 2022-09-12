#!/bin/sh
set -ex
# Ad-hoc manual script to test init script support for changing pool directory
#
# Deletes /var/ob with extreme prejudice.
# Verifies that OB_POOL_DIR in /etc/oblong/plasma.conf is obeyed
# Needs to be run on both Upstart and Systemd systems to test both code paths
# Expects oblong-plasma-serverX.Y_*.deb to be in the current directory
# and expects peek, poke, p-create, and ob-version to be on the PATH

echo "test: Initial state"
sudo rm -f /etc/oblong/plasma.conf
sudo dpkg --purge 'oblong-plasma-server*' || true
sudo killall pool-server-zeroconf-adapter || true
sudo killall pool_tcp_server || true
sudo rm -rf /var/ob/pools/*

echo "test: Default, with no poolserver"
ob-version | grep ob_pools_dir
p-create poketest
poke poketest -d yes,no -i sedate:true
peek -rx poketest | grep 'sedate: true'
echo "Verify setting bad pool directory fails"
if OB_POOLS_DIR=/tmp/nonexistant peek -rx poketest | grep 'sedate: true'
then
   echo fail: OB_POOLS_DIR noop
   exit 1
fi

echo "test: Add poolserver, retrieve same protein"
sudo dpkg -i oblong-plasma-server*.deb || true
sudo apt-get install -f
if ! ps augxw | grep -v grep | grep pool_tcp_server
then
   echo fail: pool_tcp_server not started
   exit 1
fi
peek -rx tcp://localhost/poketest | grep 'sedate: true'

echo "test: Ditto, but with different pool directory"
sudo rm -f /etc/oblong/plasma.conf
sudo dpkg --purge 'oblong-plasma-server*' || true
sudo killall pool-server-zeroconf-adapter || true
sudo killall pool_tcp_server || true
sudo rm -rf /var/ob/pools/*
sudo rm -rf /var/ob2/pools/*
echo OB_POOLS_DIR=/var/ob2/pools | sudo tee -a /etc/oblong/plasma.conf
. /etc/oblong/plasma.conf
export OB_POOLS_DIR
sudo mkdir -p $OB_POOLS_DIR
sudo chown $LOGNAME $OB_POOLS_DIR
ob-version | grep ob_pools_dir
p-create poketest
poke poketest -d yes,no -i sedate:true
peek -rx poketest | grep 'sedate: true'
sudo dpkg -i oblong-plasma-server*.deb || true
sudo apt-get install -f
if ! ps augxw | grep -v grep | grep pool_tcp_server
then
   echo fail: pool_tcp_server not started
   exit 1
fi
peek -rx tcp://localhost/poketest | grep 'sedate: true'
if ls -l /var/ob/pools/poketest
then
    echo fail: old pool dir there
    exit 1
fi
if ! ls -l /var/ob2/pools/poketest
then
    echo fail: new pool dir note there
    exit 1
fi

echo PASS
