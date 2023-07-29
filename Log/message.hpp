
#ifndef __MY_LOGMSG__
#define __MY_LOGMSG__

#include"level.hpp"
#include"util.hpp"
#include<thread>


namespace log
{
    struct LogMsg
    {
        time_t _ctime;
        log::LogLevel::value _level;
        size_t _line; // 行号
        std::thread::id _tid;
        std::string _file; // 源文件名
        std::string _logger; // 日志器名称
        std::string _payload;

        LogMsg(
            log::LogLevel::value level,
            size_t line,
            const std::string file,
            const std::string logger,
            const std::string payload
        )
            :_level(level)
            ,_ctime(log::util::Date::getTime())
            ,_line(line)
            ,_file(file)
            ,_logger(logger)
            ,_payload(payload)
            ,_tid(std::this_thread::get_id())
        {}
    };
}

#endif // !__MY_LOGMSG__