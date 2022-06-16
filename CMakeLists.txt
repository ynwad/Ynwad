cmake_minimum_required(VERSION 2.8)
project(ynwad)

#显示详细的原始编译信息，主要用于定位一些链接错误，看看库路径什么的是否配置对
set(CMAKE_VERBOSE_MAKEFILE ON)

#C++ 编译标志相关变量。
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -pthread -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

#设置变量
set(LIB_SRC
${PROJECT_SOURCE_DIR}/log.cc
${PROJECT_SOURCE_DIR}/utils.cc
    )

#添加要生成的库文件
add_library(ynwad SHARED ${LIB_SRC})
# set_target_properties(ynwad PROPERTIES OUTPUT_NAME "ynwad")
# add_library(ynwad_static STATIC $(LIB_SRC))
# SET_TARGET_PROPERTIES (ynwad_static PROPERTIES OUTPUT_NAME "ynwad")

add_executable(test tests/test.cc)
add_dependencies(test ynwad)
target_link_libraries(test ynwad)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)