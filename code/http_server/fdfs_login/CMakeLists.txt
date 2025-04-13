cmake_minimum_required (VERSION 3.0)
project("fdfs_login")

# 头文件
include_directories(/usr/include)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_SOURCE_DIR}/..)

#源文件
file(GLOB COMMON_SRC  
    "${CMAKE_SOURCE_DIR}/../common/make_log.c" 
    "${CMAKE_SOURCE_DIR}/../common/deal_mysql.c" 
    "${CMAKE_SOURCE_DIR}/../common/cJSON.c"
    "${CMAKE_SOURCE_DIR}/../common/configure.c"
    "${CMAKE_SOURCE_DIR}/../common/cryptutil.cpp"
)
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/login.c")

# 编译选项
add_compile_options(-Wall)

#定义可执行文件
add_executable(fdfs_login.exe ${COMMON_SRC} ${LOCAL_SRC})

# 输出目录
set_target_properties(fdfs_login.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 链接库
target_link_libraries(fdfs_login.exe fcgi mysqlclient m crypto)