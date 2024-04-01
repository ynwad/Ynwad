cmake_minimum_required(VERSION 2.8)
project(sylar)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

add_definitions(-Wno-builtin-macro-redefined)
#显示详细的原始编译信息，主要用于定位一些链接错误，看看库路径什么的是否配置对
set(CMAKE_VERBOSE_MAKEFILE ON)

#C++ 编译标志相关变量。
# set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++17 -Wall -Wno-deprecated -Werror -Wno-unused-function -lcrypto")
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -pthread -g -std=c++17 -Wall -Wno-deprecated-declarations -Werror -Wno-unused-function -lcrypto")

include_directories(.)
include_directories(/usr/include/jsoncpp)

link_directories(/usr/local/lib)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIR})
endif()

find_package(MariaDB REQUIRED)
if(MariaDB_FOUND)
    include_directories(${MYSQL_INCLUDE_DIR})
endif()
message("MYSQL_INCLUDE_DIR:       ${MYSQL_INCLUDE_DIR}")
message("MYSQL_LIBRARIES:       ${MYSQL_LIBRARIES}")

#设置变量
set(LIB_SRC
    sylar/mutex.cc
    sylar/util.cc
    sylar/log.cc
    sylar/thread.cc
    sylar/config.cc
    sylar/db/sqlite3.cc
    sylar/db/mysql.cc
    sylar/email/email.cc
    sylar/email/smtp.cc
    sylar/env.cc
    sylar/fiber.cc
    sylar/timer.cc
    sylar/scheduler.cc
    sylar/iomanager.cc
    sylar/fd_manager.cc
    sylar/hook.cc
    sylar/address.cc
    sylar/socket.cc
    sylar/tcp_server.cc
    sylar/bytearray.cc
    sylar/http/http.cc
    sylar/http/http_connection.cc
    sylar/http/http_parser.cc
    sylar/http/http_session.cc
    sylar/http/http_server.cc
    sylar/http/servlet.cc
    sylar/http/servlets/config_servlet.cc
    sylar/http/servlets/status_servlet.cc
    sylar/http/ws_connection.cc
    sylar/http/ws_session.cc
    sylar/http/ws_server.cc
    sylar/http/ws_servlet.cc
    sylar/stream.cc
    sylar/streams/socket_stream.cc
    sylar/streams/zlib_stream.cc
    sylar/util/json_util.cc
    sylar/util/hash_util.cc
)

ragelmaker(sylar/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar/http)
ragelmaker(sylar/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar/http)
ragelmaker(sylar/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar)

#添加要生成的库文件
add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar)

set_target_properties(sylar PROPERTIES OUTPUT_NAME "sylar")
add_library(sylar_static STATIC ${LIB_SRC})
SET_TARGET_PROPERTIES (sylar_static PROPERTIES OUTPUT_NAME "sylar")

set(LIBS
    sylar
    yaml-cpp
    jsoncpp
    sqlite3
    ${OPENSSL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    mariadbclient
)

sylar_add_executable(test_log "tests/test_log.cc" sylar "${LIBS}")
sylar_add_executable(test_thread "tests/test_thread.cc" sylar "${LIBS}")
sylar_add_executable(test_config "tests/test_config.cc" sylar "${LIBS}")
sylar_add_executable(test_fiber "tests/test_fiber.cc" sylar "${LIBS}")
sylar_add_executable(test_scheduler "tests/test_scheduler.cc" sylar "${LIBS}")
sylar_add_executable(test_iomanager "tests/test_iomanager.cc" sylar "${LIBS}")
sylar_add_executable(test_hook "tests/test_hook.cc" sylar "${LIBS}")
sylar_add_executable(test_socket "tests/test_socket.cc" sylar "${LIBS}")
sylar_add_executable(test_tcp_server "tests/test_tcp_server.cc" sylar "${LIBS}")
sylar_add_executable(test_bytearray "tests/test_bytearray.cc" sylar "${LIBS}")
sylar_add_executable(test_http_parser "tests/test_http_parser.cc" sylar "${LIBS}")
sylar_add_executable(test_http "tests/test_http.cc" sylar "${LIBS}")
sylar_add_executable(test_http_connection "tests/test_http_connection.cc" sylar "${LIBS}")
sylar_add_executable(test_http_server "tests/test_http_server.cc" sylar "${LIBS}")
sylar_add_executable(test_ws_client "tests/test_ws_client.cc" sylar "${LIBS}")
sylar_add_executable(test_ws_server "tests/test_ws_server.cc" sylar "${LIBS}")
sylar_add_executable(test_email "tests/test_email.cc" sylar "${LIBS}")
sylar_add_executable(test_sqlite3 "tests/test_sqlite3.cc" sylar "${LIBS}")
sylar_add_executable(test_mysql "tests/test_mysql.cc" sylar "${LIBS}")


# target_link_libraries(test sylar)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)