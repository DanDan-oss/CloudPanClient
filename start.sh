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
    cp -rf ../bin/fdfs_client.exe ..

    echo "build file_upload"
    [ -d "${server_code_root}/file_upload/build" ] && rm -rf ${server_code_root}/file_upload/build
    [ -d "${server_code_root}/file_upload/bin" ] && rm -rf ${server_code_root}/file_upload/bin
    mkdir -p ${server_code_root}/file_upload/build
    cd ${server_code_root}/file_upload/build
    cmake ..  && make
    cp -rf ../bin/file_upload.exe ..

    echo "build file_download"
    [ -d "${server_code_root}/file_download/build" ] && rm -rf ${server_code_root}/file_download/build
    [ -d "${server_code_root}/file_download/bin" ] && rm -rf ${server_code_root}/file_download/bin
    mkdir -p ${server_code_root}/file_download/build
    cd ${server_code_root}/file_download/build
    cmake ..  && make
    cp -rf ../bin/file_download.exe ..

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
    [ -f "/home/FastDFS/tracker/data/fdfs_trackerd.pid" ] && fdfs_trackerd ${LOCAL_FDFS_TRACKER_CONF} stop
    [ -f "/home/FastDFS/storage/data/fdfs_storaged.pid" ] && fdfs_storaged ${LOCAL_FDFS_STORAGE_CONF} stop
    echo_pid=$(ps aux | grep "echo" | grep -v grep | awk '{print $2}')
    file_upload_pid=$(ps aux | grep "file_upload.exe" | grep -v grep | awk '{print $2}')
    file_download_pid=$(ps aux | grep "file_download.exe" | grep -v grep | awk '{print $2}')
    nginx_pid=$(ps aux | grep "nginx" | grep -v grep | awk '{print $2}')
    [ -n "${file_download_pid}" ] && kill -9 ${file_download_pid}
    [ -n "${file_upload_pid}" ] && kill -9 ${file_upload_pid}
    [ -n "${echo_pid}" ] && kill -9 ${echo_pid}
    [ -n "${nginx_pid}" ] && nginx -s stop
    return 0
}

function fdfs_srever_start
{
    echo "start run server"
    [ -d "${CLOUDPAN_PATH}/temp" ] && rm -rf ${CLOUDPAN_PATH}/temp
    mkdir -p ${CLOUDPAN_PATH}/temp && cd ${CLOUDPAN_PATH}/temp
    fdfs_trackerd ${LOCAL_FDFS_TRACKER_CONF} start
    fdfs_storaged ${LOCAL_FDFS_STORAGE_CONF} start
    nginx
    spawn-fcgi -a 127.0.0.1 -p 7787 -f /home/build_tools/fcgi2/examples/echo
    spawn-fcgi -a 127.0.0.1 -p 7788 -f ${CLOUDPAN_PATH}/code/http_server/file_upload/file_upload.exe
    spawn-fcgi -a 127.0.0.1 -p 7789 -f ${CLOUDPAN_PATH}/code/http_server/file_download/file_download.exe
    #spawn-fcgi -a 127.0.0.1 -p 7790 -f ${CLOUDPAN_PATH}/code/http_server/fdfs_client/fdfs_client.exe
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