#include "../sylar/log.h"
#include <iostream>

using namespace sylar;

int main()
{
    sylar::Logger::ptr logger = std::make_shared<Logger>();

    sylar::StdOutLogAppender::ptr stdappender(new sylar::StdOutLogAppender);
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    stdappender->setFormatter(fmt);
    stdappender->setLevel(LogLevel::ERROR);

    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
    sylar::LogFormatter::ptr fmt2(new sylar::LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    file_appender->setFormatter(fmt2);
    file_appender->setLevel(sylar::LogLevel::ERROR);

    logger->addAppender(stdappender);
    logger->addAppender(file_appender);
    std::string ss = "123456677";
    SYLAR_LOG_FATAL(logger) << 11111;

    SYLAR_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
}