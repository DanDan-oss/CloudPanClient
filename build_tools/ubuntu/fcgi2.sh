#!/bin/bash
set -eux
set -o pipefail

current_dir=$(pwd)
tmp_cpus=$(grep -w processor /proc/cpuinfo|wc -l)

sudo apt-get install autoconf -y
sudo apt-get install libtool -y


sudo git clone https://github.com/FastCGI-Archives/fcgi2.git
cd fcgi2

sudo ./autogen.sh
sudo ./configure
make -j ${tmp_cpus}
make install

# echo install dir