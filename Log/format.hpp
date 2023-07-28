
#ifndef __MY_FMT_H__
#define __MY_FMT_H__
#include "level.hpp"
#include "message.hpp"
#include <iostream>
#include <memory>
#include <ctime>

namespace log
{
    // 抽象格式化子项基类
    class FormatItem
    {
    public:
        using ptr = std::shared_ptr<FormatItem>;
        virtual void format(std::ostream &out, LogMsg &msg) = 0;
    };

    // 派生格式化子项子类— — 消息、等级、时间、文件名、行号、线程ID、日志器名、制表符、换行、其他
    class MsgFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._payload;
        }
    };

    class LevelFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << LogLevel::toString(msg._level);
        }
    };

    class TimeFormatItem : public FormatItem
    {
    public:
        TimeFormatItem(const std::string &fmt = "%H:%M:%S")
            : _time_fmt(fmt)
        {
        }

        void format(std::ostream &out, LogMsg &msg) override
        {
            struct tm t;
            localtime_r(&msg._ctime, &t);
            char tmp[32] = {0};
            strftime(tmp, 31, _time_fmt.c_str(), &t);
            out << tmp;
        }

    private:
        std::string _time_fmt;
    };

    class FileFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._file;
        }
    };

    class LineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._line;
        }
    };

    class TidFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._tid;
        }
    };

    class LoggerFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._logger;
        }
    };

    class TableFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << "\t";
        }
    };

    class NlineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << "\n";
        }
    };

    class OtherFormatItem : public FormatItem
    {
    public:
        OtherFormatItem(const std::string &s)
            : _s(s)
        {
        }

        void format(std::ostream &out, LogMsg &msg) override
        {
            out << _s;
        }

    private:
        std::string _s;
    };
}

#endif