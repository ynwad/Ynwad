/*
 * @Author: ynwad
 * @Date: 2024-01-18 00:31:08
 * @LastEditors: ynwad qingchenchn@gmail.com
 * @LastEditTime: 2024-01-18 00:52:17
 * @FilePath: /ynwad/tests/test_fiber.cc
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "../sylar/sylar.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void run_in_fiber() {
    SYLAR_LOG_INFO(g_logger) << "run_in_fiber begin";
    sylar::Fiber::GetThis()->back();
    SYLAR_LOG_INFO(g_logger) << "run_in_fiber end";
    sylar::Fiber::GetThis()->back();
}

void test_fiber() {
    SYLAR_LOG_INFO(g_logger) << "main begin -1";
    {
        sylar::Fiber::GetThis();
        SYLAR_LOG_INFO(g_logger) << "main begin";
        sylar::Fiber::ptr fiber(new sylar::Fiber(run_in_fiber));
        fiber->call();
        SYLAR_LOG_INFO(g_logger) << "main after swapIn";
        fiber->call();
        SYLAR_LOG_INFO(g_logger) << "main after end";
        fiber->call();
    }
    SYLAR_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char** argv) {
    sylar::Thread::SetName("main");

    std::vector<sylar::Thread::ptr> thrs;
    for(int i = 0; i < 1; ++i) {
        thrs.push_back(sylar::Thread::ptr(
                    new sylar::Thread(&test_fiber, "name_" + std::to_string(i))));
    }
    for(auto i : thrs) {
        i->join();
    }
    return 0;
}
