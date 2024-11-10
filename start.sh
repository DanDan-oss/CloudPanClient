#!/usr/local/bash
source /etc/profile

function init_env
{
    export LOCAL_ADDR=$(ifconfig | grep '192.168.' | awk  -F ' ' '{print $2}')
    export LOCAL_FDFS_CLIENT_CONF="/etc/fdfs/client.conf"
    export LOCAL_FDFS_STORAGE_CONF="/etc/fdfs/storage.conf"
    export LOCAL_FDFS_FDSTDFS_CONF="/etc/fdfs/mod_fastdfs.conf"
    export LOCAL_FDFS_TRACKER_CONF="/etc/fdfs/tracker.conf"
    export LOCAL_NGINX_CONF="/home/build_tools/nginx/conf/nginx.conf"
    export CLOUDPAN_PATH="/home/CodeHub/CloudPan"
    
    echo "start env ..."
    echo "current ip address = ${LOCAL_ADDR} ..."
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${LOCAL_FDFS_CLIENT_CONF}
    sed -i "s@bind_addr.*=.*@bind_addr = ${LOCAL_ADDR}@g" ${LOCAL_FDFS_STORAGE_CONF}
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${LOCAL_FDFS_STORAGE_CONF}
    sed -i "s@tracker_server.*=.*:@tracker_server = ${LOCAL_ADDR}:@g" ${LOCAL_FDFS_FDSTDFS_CONF}
    sed -i "s@bind_addr.*=.*@bind_addr = ${LOCAL_ADDR}@g" ${LOCAL_FDFS_TRACKER_CONF}
    echo "end env ..."
}

function main
{
    export COMMAND=$1
    case ${COMMAND} in
    stop)
        echo "stop"
        fdfs_trackerd /etc/fdfs/tracker.conf stop
        fdfs_storaged /etc/fdfs/storage.conf stop
        nginx -s stop
        kill -9 $(ps aux | grep "fastcgi.exe" | grep -v grep | awk '{print $2}')
        kill -9 $(ps aux | grep "echo" | grep -v grep | awk '{print $2}')
        ;;
    start)
        init_env
        echo "start"
        fdfs_trackerd /etc/fdfs/tracker.conf start
        fdfs_storaged /etc/fdfs/storage.conf start
        cd ${CLOUDPAN_PATH}/CloudPanServer && make clean && make
        spawn-fcgi -a 127.0.0.1 -p 7788 -f ${CLOUDPAN_PATH}/CloudPanServer/fastcgi.exe
        spawn-fcgi -a 127.0.0.1 -p 7787 -f /home/build_tools/fcgi2/examples/echo
        nginx
        ;;
    restart)
        main stop
        main start
        ;;
    *)
        echo "COMMAND"
        echo "    bash start.sh stop"
        echo "    bash start.sh start"
        echo "    bash start.sh reload"
        ;;
    esac
}

main $@