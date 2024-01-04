cmake_minimum_required(VERSION 2.8)
project(sylar)

cmake_policy(SET CMP0045 OLD)

include (cmake/utils.cmake)
add_definitions(-Wno-builtin-macro-redefined)
#显示详细的原始编译信息，主要用于定位一些链接错误，看看库路径什么的是否配置对
set(CMAKE_VERBOSE_MAKEFILE ON)

#C++ 编译标志相关变量。
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++17 -Wall -Wno-deprecated -Werror -Wno-unused-function")

link_directories(/usr/local/lib)

#设置变量
set(LIB_SRC
    sylar/mutex.cc
    sylar/utils.cc
    sylar/log.cc
)

#添加要生成的库文件
add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar)

set_target_properties(sylar PROPERTIES OUTPUT_NAME "sylar")
add_library(sylar_static STATIC ${LIB_SRC})
SET_TARGET_PROPERTIES (sylar_static PROPERTIES OUTPUT_NAME "sylar")

set(LIBS
    sylar
    yaml-cpp
)

sylar_add_executable(test "tests/test_log.cc" sylar "${LIBS}")
# target_link_libraries(test sylar)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)