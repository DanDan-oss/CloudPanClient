#!/usr/local/bash
set -eux

function init_env
{
    # export LOCAL_ADDR=$(hostname -I)
    export LOCAL_ADDR=$(ifconfig | grep '192.168.' | awk  -F ' ' '{print $2}')
    export LOCAL_FDFS_CLIENT_CONF="/etc/fdfs/client.conf"
    export LOCAL_FDFS_STORAGE_CONF="/etc/fdfs/storage.conf"
    export LOCAL_FDFS_FDSTDFS_CONF="/etc/fdfs/mod_fastdfs.conf"
    export LOCAL_FDFS_TRACKER_CONF="/etc/fdfs/tracker.conf"
    export LOCAL_NGINX_CONF="/home/build_tools/nginx/conf/nginx.conf"
    export CLOUDPAN_PATH="/home/CodeHub/CloudPan/CloudPanClient"
}

function build_server
{
    local server_code_root="${CLOUDPAN_PATH}/code/http_server"
    echo "start build code ..."

    echo "build fdfs_client"
    [ -d "${server_code_root}/fdfs_client/build" ] && rm -rf ${server_code_root}/fdfs_client/build
    [ -d "${server_code_root}/fdfs_client/bin" ] && rm -rf ${server_code_root}/fdfs_client/bin
    mkdir -p ${server_code_root}/fdfs_client/build
    cd ${server_code_root}/fdfs_client/build
    cmake ..  && make

    echo "build file_upload"
    cd ${server_code_root}/file_upload
    echo "end build code ..."
}

function fdfs_init_confi
{
    echo "start init configuration file ..."
    echo "current ip address = ${LOCAL_ADDR} ..."
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${LOCAL_FDFS_CLIENT_CONF}
    sed -i "s@bind_addr.*=.*@bind_addr = ${LOCAL_ADDR}@g" ${LOCAL_FDFS_STORAGE_CONF}
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${LOCAL_FDFS_STORAGE_CONF}
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${LOCAL_FDFS_FDSTDFS_CONF}
    sed -i "s@bind_addr.*=.*@bind_addr = ${LOCAL_ADDR}@g" ${LOCAL_FDFS_TRACKER_CONF}
    echo "end  init configuration file ..."
}

function fdfs_srever_stop
{
    echo "stop run server"
    fdfs_trackerd ${LOCAL_FDFS_TRACKER_CONF} stop
    fdfs_storaged ${LOCAL_FDFS_STORAGE_CONF} stop
    nginx -s stop
    fastcgi_pid=$(ps aux | grep "fastcgi.exe" | grep -v grep | awk '{print $2}')
    echo_pid=$(ps aux | grep "echo" | grep -v grep | awk '{print $2}')
    [ -n "${fastcgi_pid}" ] && kill -9 ${fastcgi_pid}
    [ -n "${echo_pid}" ] && kill -9 ${echo_pid}
}

function fdfs_srever_start
{
    echo "start run server"
    fdfs_trackerd ${LOCAL_FDFS_TRACKER_CONF} start
    fdfs_storaged ${LOCAL_FDFS_STORAGE_CONF} start
    spawn-fcgi -a 127.0.0.1 -p 7787 -f /home/build_tools/fcgi2/examples/echo
    spawn-fcgi -a 127.0.0.1 -p 7788 -f ${CLOUDPAN_PATH}/code/http_server/file_upload/fileupload.exe
    spawn-fcgi -a 127.0.0.1 -p 7789 -f ${CLOUDPAN_PATH}/code/http_server/file_download/fastcgi.exe
    #spawn-fcgi -a 127.0.0.1 -p 7790 -f ${CLOUDPAN_PATH}/code/http_server/fdfs_client/bin/fdfs_client.exe
    nginx
}

function help
{
    echo "COMMAND"
    echo "    bash start.sh restart"
    echo "    bash start.sh stop"
    echo "    bash start.sh start"
    echo "    bash start.sh build"
}

function main
{
    export COMMAND=$1
    case ${COMMAND} in
    restart)
        main stop
        main build
        main start
        ;;
    stop)
        fdfs_srever_stop
        ;;
    start)
        fdfs_init_confi
        fdfs_srever_start
        ;;
    build)
        build_server
        ;;
    *)
        help
        ;;
    esac
}

init_env
main $@