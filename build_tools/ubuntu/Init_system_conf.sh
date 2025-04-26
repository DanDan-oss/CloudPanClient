#!/bin/bash

set -eux
set -o pipefail

ROOT_PATH=`pwd`

#设置源
function init_proxy()
{
	sudo cp -a /etc/apt/sources.list /etc/apt/sources.list.bak
	sudo sed -i "s@http://.*archive.ubuntu.com@http://mirrors.aliyun.com@g" /etc/apt/sources.list
	sudo sed -i "s@http://.*security.ubuntu.com@http://mirrors.aliyun.com@g" /etc/apt/sources.list
	apt-get update
}

#安装系统工具
function install_systools()
{
	sudo apt install libpcre3 libpcre3-dev -y
	sudo apt install cmake make -y
	sudo apt install gcc g++ -y
}


# 安装编译工具 by 安装脚本
function install_buildtools()
{
	BUILDTOOLS=(
		hiredis.sh
		mysql.sh
		nginx-1.27.5.sh
		fcgi2.sh
		FastDFS.sh
	)

    pushd ${ROOT_PATH}
	for TOOL in ${BUILDTOOLS[@]}
	do
		[ -f "${TOOL}" ] && bash ${TOOL}
	done
	popd
}

function backup_cert(){

    file=$1
    real_file=${file}
    if [ -f ${file} ];then
        if [ -L ${file} ];then
            real_file=$(readlink -f ${file})
        fi
        cp -fa ${real_file}  ${real_file}.bak
    fi
}


function main()
{
	init_proxy
	
	install_buildtools
	# 执行定义脚本回调
	[ -f 'post/post.sh' ] && bash post/post.sh
	
}

main
