cmake_minimum_required(VERSION 2.8)

project(file_tools)

add_definitions(-std=c++11)

# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

find_package(PkgConfig)

pkg_search_module(JSON_C REQUIRED json-c)

#添加头文件搜索路径/usr/local/include/thrift /usr/local/include/
# "/home/qilimi/project/libSonixCamera"
# "/home/qilimi/project/libiris"
include_directories(/usr/local/include $(CCRTP_INCLUDE_DIRS))

#添加库文件搜索路径/usr/local/lib/sharenew  自己的目录
link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu /usr/local/lib/sharenew)

#用于将当前目录下的所有源文件的名字保存在变量 DIR_SRCS 中
aux_source_directory(. DIR_FILE_TOOLS_SRCS)

# add_executable(xn_iris_device ${DIR_SRCS})
add_library(file_tools ${DIR_FILE_TOOLS_SRCS})

#在这里根据名字查找文件，或者写usb
target_link_libraries(file_tools pthread $(JSON_C_LIBRARIES))
