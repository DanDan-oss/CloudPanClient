#!/usr/local/bash
set -eux

function init_env
{
    # export LOCAL_ADDR=$(hostname -I)
    export LOCAL_ADDR=$(ifconfig | grep '192.168.' | awk  -F ' ' '{print $2}')
    export CLOUDPAN_PATH="/home/CodeHub/CloudPan/CloudPanClient"

    local_fdfs_client_conf="${CLOUDPAN_PATH}/code/http_server/conf/client.conf"
    local_fdfs_fdstdfs_conf="${CLOUDPAN_PATH}/code/http_server/conf/mod_fastdfs.conf"
    local_fdfs_storage_conf="${CLOUDPAN_PATH}/code/http_server/conf/storage.conf"
    local_fdfs_tracker_conf="${CLOUDPAN_PATH}/code/http_server/conf/tracker.conf"
    local_nginx_conf="${CLOUDPAN_PATH}/code/http_server/conf/nginx.conf"
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

    echo "build fdfs_register"
    [ -d "${server_code_root}/fdfs_register/build" ] && rm -rf ${server_code_root}/file_download/build
    [ -d "${server_code_root}/fdfs_register/bin" ] && rm -rf ${server_code_root}/file_download/bin
    mkdir -p ${server_code_root}/fdfs_register/build
    cd ${server_code_root}/fdfs_register/build
    cmake ..  && make
    cp -rf ../bin/fdfs_register.exe ..

    echo "end build code ..."
}

function fdfs_init_confi
{

    echo "start init configuration file ..."
    echo "current ip address = ${LOCAL_ADDR} ..."
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${local_fdfs_client_conf}
    sed -i "s@bind_addr.*=.*@bind_addr = ${LOCAL_ADDR}@g" ${local_fdfs_storage_conf}
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${local_fdfs_storage_conf}
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${local_fdfs_fdstdfs_conf}
    sed -i "s@bind_addr.*=.*@bind_addr = ${LOCAL_ADDR}@g" ${local_fdfs_tracker_conf}

    cp -rf ${local_fdfs_client_conf} /etc/fdfs
    cp -rf ${local_fdfs_fdstdfs_conf} /etc/fdfs
    cp -rf ${local_fdfs_storage_conf} /etc/fdfs
    cp -rf ${local_fdfs_tracker_conf} /etc/fdfs
    cp -rf ${local_nginx_conf} /usr/local/nginx/conf

    echo "end  init configuration file ..."
}

function fdfs_srever_stop
{
    echo "stop run server"
    fdfs_trackerd_pid=$(ps aux | grep "fdfs_trackerd" | grep -v grep | awk '{print $2}')
    fdfs_storaged_pid=$(ps aux | grep "fdfs_storaged" | grep -v grep | awk '{print $2}')
    echo_pid=$(ps aux | grep "echo" | grep -v grep | awk '{print $2}')
    file_upload_pid=$(ps aux | grep "file_upload.exe" | grep -v grep | awk '{print $2}')
    file_download_pid=$(ps aux | grep "file_download.exe" | grep -v grep | awk '{print $2}')
    fdfs_register_pid=$(ps aux | grep "fdfs_register.exe" | grep -v grep | awk '{print $2}')
    nginx_pid=$(ps aux | grep "nginx" | grep -v grep | awk '{print $2}')
    
    [ -n "${fdfs_trackerd_pid}" ] && kill -9 ${fdfs_trackerd_pid}
    [ -n "${fdfs_storaged_pid}" ] && kill -9 ${fdfs_storaged_pid}
    [ -n "${file_download_pid}" ] && kill -9 ${file_download_pid}
    [ -n "${file_upload_pid}" ] && kill -9 ${file_upload_pid}
    [ -n "${fdfs_register_pid}" ] && kill -9 ${fdfs_register_pid}
    [ -n "${echo_pid}" ] && kill -9 ${echo_pid}
    [ -n "${nginx_pid}" ] && nginx -s stop

    return 0
}

function fdfs_srever_start
{
    echo "start run server"
    [ -d "${CLOUDPAN_PATH}/temp" ] && rm -rf ${CLOUDPAN_PATH}/temp
    mkdir -p ${CLOUDPAN_PATH}/temp && cd ${CLOUDPAN_PATH}/temp
    fdfs_trackerd ${local_fdfs_tracker_conf} start
    fdfs_storaged ${local_fdfs_storage_conf} start
    nginx
    spawn-fcgi -a 127.0.0.1 -p 7787 -f /home/build_tools/fcgi2/examples/echo
    spawn-fcgi -a 127.0.0.1 -p 7788 -f ${CLOUDPAN_PATH}/code/http_server/file_upload/file_upload.exe
    spawn-fcgi -a 127.0.0.1 -p 7789 -f ${CLOUDPAN_PATH}/code/http_server/file_download/file_download.exe
    spawn-fcgi -a 127.0.0.1 -p 7790 -f ${CLOUDPAN_PATH}/code/http_server/fdfs_register/fdfs_register.exe
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