cmake_minimum_required (VERSION 3.0)
project("file_download")

# 头文件
include_directories(/usr/include)
include_directories(${CMAKE_SOURCE_DIR}/..)

#源文件
file(GLOB COMMON_SRC  "${CMAKE_SOURCE_DIR}/../common/make_log.c")
file(GLOB LOCAL_SRC  "${CMAKE_SOURCE_DIR}/main.c")

#参数
add_compile_options(-Wall)

#定义可执行文件
add_executable(file_download.exe ${COMMON_SRC} ${LOCAL_SRC})
set_target_properties(file_download.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(file_download.exe fcgi)

