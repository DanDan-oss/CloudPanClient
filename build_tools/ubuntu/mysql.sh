#!/bin/bash
set -eux
set -o pipefail

sudo apt-get install mysql-server -y
sudo apt install mysql-client -y
sudo apt install libmysqlclient-dev -y
sudo apt-get install libmysqlclient-dev -y