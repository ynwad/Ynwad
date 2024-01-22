/*
 * @Author: ynwad
 * @Date: 2024-01-22 21:47:20
 * @LastEditors: ynwad qingchenchn@gmail.com
 * @LastEditTime: 2024-01-22 22:47:38
 * @FilePath: /ynwad/sylar/fd_manager.cc
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "fd_manager.h"
#include "hook.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

FdCtx::FdCtx(int fd)
    :m_isInit(false)
    ,m_isSocket(false)
    ,m_sysNonblock(false)
    ,m_userNonblock(false)
    ,m_isClosed(false)
    ,m_fd(fd)
    ,m_recvTimeout(-1)
    ,m_sendTimeout(-1) {
    init();
}

FdCtx::~FdCtx() {
}

bool FdCtx::init() {
    
}