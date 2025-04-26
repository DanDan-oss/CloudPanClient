#!/bin/bash
set -eux
set -o pipefail

current_dir=$(pwd)
tool_dir=/opt/buildtools
install_dir=${tool_dir}/nginx-1.27.5
download_nginx_url="https://nginx.org/download/nginx-1.27.5.tar.gz"

# download nginx-1.27.5.tar.gz
[ -f 'nginx-1.27.5.tar.gz' ] || wget ${download_nginx_url}

# check install files
ls nginx-1.27.5.tar.gz

# clear
[ -d "${install_dir}" ] && rm -rf ${install_dir}
mkdir -p ${install_dir}
tmp_cpus=$(grep -w processor /proc/cpuinfo|wc -l)


tar -zxvf nginx-1.27.5.tar.gz
git clone https://gitee.com/fastdfs100/fastdfs-nginx-module.git fastdfs-nginx-module


cd ${current_dir}/nginx-1.27.5

./configure --prefix=${install_dir} --pid-path=/var/run/nginx/nginx.pid --lock-path=${install_dir}/logs/error.log --error-log-path=${install_dir}/logs/error.log --http-log-path=${install_dir}/logs/access.log \--with-http_gzip_static_module --http-client-body-temp-path=${install_dir}/client --http-proxy-temp-path=${install_dir}/proxy --http-fastcgi-temp-path=${install_dir}/fastcgi --http-uwsgi-temp-path=${install_dir}/scgi --add-module=${current_dir}/fastdfs-nginx-module/src

make -j ${tmp_cpus}
make install

chmod 755 -R ${install_dir}
[ -d "/usr/local/nginx" ] && rm -rf /usr/local/nginx
ln -sf ${install_dir} /usr/local/nginx
ln -sf /usr/local/nginx/sbin/nginx 

#delete
rm -rf ${current_dir}/nginx-1.27.5.tar.gz
rm -rf ${current_dir}/nginx-1.27.5
rm -rf ${current_dir}/fastdfs-nginx-module

# echo install dir
echo "Install Path=${install_dir}"