#ifndef __MY_LOG__
#define __MY_LOG__

#include "logger.hpp"

namespace log
{
    // 获取指定日志器的全局接口，避免用户自己操作单例对象
    Logger::ptr getLogger(const string &name)
    {
        return LoggerManager::getInstance().getLogger(name);
    }

    Logger::ptr rootLogger()
    {
        return LoggerManager::getInstance().rootLogger();
    }

// 使用宏函数对日志器的接口进行代理
#define debug(fmt, ...) debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define info(fmt, ...) info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define warn(fmt, ...) warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define error(fmt, ...) error(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define fatal(fmt, ...) fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

// 通过默认日志器进行标准输出打印
#define DEBUG(fmt,...) log::rootLogger()->debug(fmt,##__VA_ARGS__)
#define INFO(fmt, ...) log::rootLogger()->info(fmt, ##__VA_ARGS__)
#define WARN(fmt,...) log::rootLogger()->warn(fmt,##__VA_ARGS__)
#define ERROR(fmt,...) log::rootLogger()->error(fmt,##__VA_ARGS__)
#define FATAL(fmt, ...) log::rootLogger()->fatal(fmt, ##__VA_ARGS__)
}

#endif