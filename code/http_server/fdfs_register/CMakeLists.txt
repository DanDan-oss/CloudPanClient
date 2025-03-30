cmake_minimum_required (VERSION 3.0)
project("fdfs_register")

# 头文件
include_directories(/usr/include)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_SOURCE_DIR}/..)

#源文件
file(GLOB COMMON_SRC  "${CMAKE_SOURCE_DIR}/../common/make_log.c" "${CMAKE_SOURCE_DIR}/../common/deal_mysql.c")
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/register.c"
)

#参数
add_compile_options(-Wall)

#定义可执行文件
add_executable(fdfs_register.exe ${COMMON_SRC} ${LOCAL_SRC})
set_target_properties(fdfs_register.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(fdfs_register.exe fcgi mysqlclient)