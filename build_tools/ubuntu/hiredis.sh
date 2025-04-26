#!/bin/bash
set -eux
set -o pipefail

current_dir=$(pwd)
tmp_cpus=$(grep -w processor /proc/cpuinfo|wc -l)

sudo apt-get install redis-server -y

sudo git clone https://github.com/redis/hiredis.git
cd hiredis/

make -j ${tmp_cpus}
make install

# echo install dir