cmake_minimum_required (VERSION 3.0)
project("file_upload")

# 头文件
include_directories(/usr/include)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_SOURCE_DIR}/..)

#源文件
file(GLOB COMMON_SRC  
    "${CMAKE_SOURCE_DIR}/../common/make_log.c" 
    "${CMAKE_SOURCE_DIR}/../common/deal_mysql.c"
    "${CMAKE_SOURCE_DIR}/../common/fdfs_api.c"
    "${CMAKE_SOURCE_DIR}/../common/deal_mysql.c"
    "${CMAKE_SOURCE_DIR}/../common/configure.c"
    "${CMAKE_SOURCE_DIR}/../common/util_cgi.c"
    "${CMAKE_SOURCE_DIR}/../common/cJSON.c")
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/main.c" "${CMAKE_SOURCE_DIR}/file_upload.c"
)

#参数
add_compile_options(-Wall)

#定义可执行文件
add_executable(file_upload.exe ${COMMON_SRC} ${LOCAL_SRC})
set_target_properties(file_upload.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(file_upload.exe fcgi mysqlclient fdfsclient fastcommon m)

