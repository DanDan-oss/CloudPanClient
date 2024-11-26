cmake_minimum_required (VERSION 3.0)
project("fdfs_client")

# 头文件
include_directories(/usr/include/fastdfs)
include_directories(/usr/include/fastcommon)
include_directories(${CMAKE_SOURCE_DIR}/..)

#源文件
file(GLOB COMMON_SRC  "${CMAKE_SOURCE_DIR}/../common/*.c")
file(GLOB LOCAL_SRC  "${CMAKE_SOURCE_DIR}/*.c")

#参数
add_compile_options(-Wall)

#定义可执行文件
add_executable(fdfs_client.exe ${COMMON_SRC} ${LOCAL_SRC})
set_target_properties(fdfs_client.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(fdfs_client.exe fdfsclient fastcommon)

