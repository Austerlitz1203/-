#ifndef __MY_LOGGER__
#define __MY_LOGGER__

#define _GUN_SOURCE
#include "util.hpp"
#include "sink.hpp"
#include "format.hpp"
#include "level.hpp"
#include <atomic>
#include <mutex>
#include <vector>
#include <stdarg.h>

namespace log
{
    class Logger
    {
    public:
        using ptr = std::shared_ptr<Logger>;
        Logger(const string &name,
               LogLevel::value level,
               Formatter::ptr &formatter,
               std::vector<LogSink::ptr> sink)
            : _logger_name(name), _limit_level(level), _formatter(formatter), _sinks(sink.begin(), sink.end())
        {
            
        }

        // 构造日志消息对象并格式化，得到格式化之后的字符串然后输出
        void debug(const string &file, size_t line, const string &fmt, ...)
        {
            // 1.判断当前日志是否达到输出等级
            if (LogLevel::value::DEBUG < _limit_level)
                return;
            // 2. 对 fmt 格式化字符串和不定参进行字符串组织，得到日志消息的字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                cout << "vasprintf error" << endl;
                return;
            }
            va_end(ap);
            serialize(file, line, res, LogLevel::value::DEBUG);
            free(res);
        }
        void info(const string &file, size_t line, const string &fmt, ...)
        {
            // 1.判断当前日志是否达到输出等级
            if (LogLevel::value::INFO < _limit_level)
                return;
            // 2. 对 fmt 格式化字符串和不定参进行字符串组织，得到日志消息的字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                cout << "vasprintf error" << endl;
                return;
            }
            va_end(ap);
            serialize(file, line, res, LogLevel::value::INFO);
            free(res);
        }
        void warn(const string &file, size_t line, const string &fmt, ...)
        {
            // 1.判断当前日志是否达到输出等级
            if (LogLevel::value::WARN < _limit_level)
                return;
            // 2. 对 fmt 格式化字符串和不定参进行字符串组织，得到日志消息的字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                cout << "vasprintf error" << endl;
                return;
            }
            va_end(ap);
            serialize(file, line, res, LogLevel::value::WARN);
            free(res);
        }
        void error(const string &file, size_t line, const string &fmt, ...)
        {
            // 1.判断当前日志是否达到输出等级
            if (LogLevel::value::ERROR < _limit_level)
                return;
            // 2. 对 fmt 格式化字符串和不定参进行字符串组织，得到日志消息的字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                cout << "vasprintf error" << endl;
                return;
            }
            va_end(ap);
            serialize(file, line, res, LogLevel::value::ERROR);
            free(res);
        }
        void fatal(const string &file, size_t line, const string &fmt, ...)
        {
            // 1.判断当前日志是否达到输出等级
            if (LogLevel::value::FATAL < _limit_level)
                return;
            // 2. 对 fmt 格式化字符串和不定参进行字符串组织，得到日志消息的字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                cout << "vasprintf error" << endl;
                return;
            }
            va_end(ap);
            serialize(file, line, res, LogLevel::value::FATAL);
            free(res);
        }

    protected:
        void serialize(const string &file, size_t line, const char *res, LogLevel::value loglevel)
        {
            // 3. 构造LogMsg对象
            LogMsg msg(loglevel, line, file, _logger_name, res);
            // 4. 按照Logger格式化的要求，对 LogMsg 格式化
            std::stringstream ss;
            _formatter->format(ss, msg);
            // 5. 进行日志落地
            dolog(ss.str().c_str(), ss.str().size());
        }

        // 实际输出由log 完成
        virtual void dolog(const char *data, size_t len) = 0;

    protected:
        std::mutex _mutex;
        string _logger_name;
        LogLevel::value _limit_level;
        Formatter::ptr _formatter;
        std::vector<LogSink::ptr> _sinks;
    };

    class SyncLogger : public Logger
    {
    public:
        SyncLogger(const string &name,
                   LogLevel::value level,
                   Formatter::ptr &formatter,
                   std::vector<LogSink::ptr> sink)
            : Logger(name, level, formatter, sink)
        {
        }

    protected:
        void dolog(const char *data, size_t len) override
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if(_sinks.empty()) return;
            for(auto &sink:_sinks)
            {
                sink->log(data,len);
            }
        }
    };
}

#endif
