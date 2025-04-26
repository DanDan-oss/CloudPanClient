#!/bin/bash
# 参考资料 https://github.com/happyfish100/fastdfs/blob/master/INSTALL
set -eux
set -o pipefail
current_dir=$(pwd)
tool_dir=/opt/buildtools
install_dir=${tool_dir}/FastDFS

current_ip=`ifconfig  | grep "inet " | grep -v "127.0.0.1" | awk '{print $2}'`
tracker_ip_address=${current_ip}
storage_ip_address=${current_ip}
tracker_bind_port="22122"
storage_bind_port="23000"
tracker_log_path="/home/FastDFS/tracker"
storage_log_path="/home/FastDFS/storage"
client_log_path="/home/FastDFS/client"
storage_path0="/home/FastDFS/storage/storage_path0"

[ -d ${install_dir} ] && rm -rf ${install_dir}
mkdir -p ${install_dir}
cd ${install_dir}

git clone https://github.com/happyfish100/libfastcommon.git
git clone https://github.com/happyfish100/libserverframe.git
git clone https://github.com/happyfish100/fastdfs.git

cd ${install_dir}/libfastcommon; git checkout V1.0.72
bash make.sh clean && bash make.sh && bash make.sh install

cd ${install_dir}/libserverframe; git config --global --add safe.directory ${install_dir}/libserverframe; git checkout V1.2.1
bash make.sh clean && bash make.sh && bash make.sh install

cd ${install_dir}/fastdfs; git config --global --add safe.directory ${install_dir}/fastdfs; git checkout V6.11.0
bash make.sh clean && bash make.sh && bash make.sh install 
bash setup.sh /etc/fdfs

# 测试是否安装成功
fdfs_test

# 修改tracker(追踪器)-->(IP地址、端口、日志存放目录)
mkdir -p ${tracker_log_path}
cp -rf /etc/fdfs/tracker.conf /etc/fdfs/tracker.conf.back
sed -i "s@bind_addr =.*@bind_addr = ${tracker_ip_address}@" /etc/fdfs/tracker.conf
sed -i "s@port =.*@port =  ${tracker_bind_port}@" /etc/fdfs/tracker.conf
sed -i "s@base_path =.*@base_path = ${tracker_log_path}@" /etc/fdfs/tracker.conf

# 修改storage(存储节点)-->(IP地址、端口、日志目录、存储目录、追踪器tracker IP)
mkdir -p ${storage_log_path}
mkdir -p ${storage_path0}
cp -rf /etc/fdfs/storage.conf /etc/fdfs/storage.conf.back
sed -i "s@tracker_server =.*@tracker_server = ${tracker_ip_address}:${tracker_bind_port}@" /etc/fdfs/storage.conf
sed -i "s@bind_addr =.*@bind_addr = ${storage_ip_address}@" /etc/fdfs/storage.conf
sed -i "s@port =.*@port =  ${storage_bind_port}@" /etc/fdfs/storage.conf
sed -i "s@base_path =.*@base_path = ${storage_log_path}@" /etc/fdfs/storage.conf
sed -i "s@store_path_count =.*@store_path_count = 1@" /etc/fdfs/storage.conf   # 指定存储器存储目录为1个,如果有两个目录填2
sed -i "s@store_path0 =.*@store_path0 = ${storage_path0}@" /etc/fdfs/storage.conf    # 指定存储路径0号
#sed -i "s@store_path1 =.*@store_path1 =  ${storage_path1}@" /etc/fdfs/storage.conf

# 修改client(客户端)-->(日志目录、追踪器tracker IP:port)
mkdir -p ${client_log_path}
cp -rf /etc/fdfs/client.conf /etc/fdfs/client.conf.back
sed -i "s@tracker_server =.*@tracker_server = ${tracker_ip_address}:${tracker_bind_port}@" /etc/fdfs/client.conf
sed -i "s@base_path =.*@base_path = ${client_log_path}@" /etc/fdfs/client.conf

# (可选) 启动跟踪器服务器、启动存储服务器
/usr/bin/fdfs_trackerd /etc/fdfs/tracker.conf restart
/usr/bin/fdfs_storaged /etc/fdfs/storage.conf restart

#(可选)在Linux中，可以启动fdfs_trackerd和fdfs_storaged作为服务
/sbin/service fdfs_trackerd restart
/sbin/service fdfs_storaged restart

# 运行监视器程序查看 Storage  ip_addr = 192.168.0.104  是否为ACTIVE
/usr/bin/fdfs_monitor /etc/fdfs/client.conf

# 测试上传下载
# fdfs_upload_file /etc/fdfs/client.conf /home/CodeHub/CloudPan/CloudPan-sys-install/FastDFS.sh
# fdfs_download_file /etc/fdfs/client.conf group1/M00/00/00/wKgAaGXcJm6AHGchAAANSiPRY8s1437.sh