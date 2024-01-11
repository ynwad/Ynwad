/*
 * @Author: ynwad
 * @Date: 2023-12-21 22:55:51
 * @LastEditors: ynwad qingchenchn@gmail.com
 * @LastEditTime: 2024-01-11 22:11:20
 * @FilePath: /ynwad/sylar/utils.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
/**
 * @file util.h
 * @brief 常用的工具函数
 * @author sylar.yin
 * @email qingchenchn@gmail.com
 * @date 2022-6-24
 */

#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <cxxabi.h>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <yaml-cpp/yaml.h>

namespace sylar {
/**
 * @brief 返回当前线程的ID
 */
pid_t GetThreadId();

/**
 * @brief 返回当前协程的ID
 */
uint32_t GetFiberId();

class FSUtil {
public:
    static void ListAllFile(std::vector<std::string>& files
                            ,const std::string& path
                            ,const std::string& subfix);
    static bool Mkdir(const std::string& dirname);
    static std::string Dirname(const std::string& filename);
    static std::string Basename(const std::string& filename);
    static bool OpenForWrite(std::ofstream& ofs, const std::string& filename
                    ,std::ios_base::openmode mode);
};

template<class T>
const char* TypeToName() {
    static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
    return s_name;
}

}
#endif