#include "../sylar/log.h"

using namespace sylar;

extern const std::string thread_name;

int main()
{
    sylar::Logger::ptr logger = std::make_shared<Logger>();
    // std::shared_ptr<Logger> logger = std::make_shared<Logger>();

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
    SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)<<"222222222222222222222222222222"<<"  "<<ss;
    // SYLAR_LOG_FATAL(logger) << 11111;
    // std::shared_ptr<LogEvent> pEvent = std::make_shared<LogEvent>();
    // pEvent->getLogger()->log(logger, );
}