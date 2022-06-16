#include "../log.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        ynwad::LogEventWrap(ynwad::LogEvent::ptr(new ynwad::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, 0,\
                0, time(0), "Thread_Name"))).getSS()

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__)


using namespace ynwad;

int main()
{
    std::shared_ptr<Logger> logger = std::make_shared<Logger>();

    StdOutLogAppender::ptr stdappender(new StdOutLogAppender);
    LogFormatter::ptr fmt(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    stdappender->setFormatter(fmt);
    stdappender->setLevel(LogLevel::ERROR);

    FileLogAppender::ptr file_appender(new FileLogAppender("./log.txt"));
    LogFormatter::ptr fmt2(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    file_appender->setFormatter(fmt2);
    file_appender->setLevel(LogLevel::ERROR);

    logger->addAppender(stdappender);
    logger->addAppender(file_appender);
    std::string ss = "123456677";
    SYLAR_LOG_LEVEL(logger, LogLevel::FATAL)<<"222222222222222222222222222222"<<"  "<<ss;

    // std::shared_ptr<LogEvent> pEvent = std::make_shared<LogEvent>();
    // pEvent->getLogger()->log(logger, );
}