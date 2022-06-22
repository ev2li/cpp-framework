#include <iostream>
#include "sylar/log.h"
#include "sylar/util.h"
#include "sylar/singleton.h"

int main(int argc, char** argv) {
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

   // sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0, sylar::GetThreadId(), sylar::GetFiberId(), time(0)));

    //event->getSS() << "hello sylar log";
    //logger->log(sylar::LogLevel::DEBUG, event);
    //std::cout << "hello sylar log" << std::endl;
    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::ERROR);
    logger->addAppender(file_appender);
    SYLAR_LOG_INFO(logger) << "test marco";
    SYLAR_LOG_ERROR(logger) << "test marco error";
    SYLAR_LOG_FMT_ERROR(logger, "test marcro fmt error %s", "aa");

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO(logger) << "xxx";
    return 0;
}