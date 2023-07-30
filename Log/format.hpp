
#ifndef __MY_FMT_H__
#define __MY_FMT_H__
#include "level.hpp"
#include "message.hpp"
#include <iostream>
#include <memory>
#include <ctime>
#include <vector>
#include <cassert>
#include <sstream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace log
{
    // 抽象格式化子项基类
    class FormatItem
    {
    public:
        using ptr = std::shared_ptr<FormatItem>;
        virtual void format(std::ostream &out, const LogMsg &msg) = 0;
    };

    // 派生格式化子项子类— — 消息、等级、时间、文件名、行号、线程ID、日志器名、制表符、换行、其他
    class MsgFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const LogMsg &msg) override
        {
            out << msg._payload;
        }
    };

    class LevelFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const LogMsg &msg) override
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

        void format(std::ostream &out, const LogMsg &msg) override
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
        void format(std::ostream &out, const LogMsg &msg) override
        {
            out << msg._file;
        }
    };

    class LineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const LogMsg &msg) override
        {
            out << msg._line;
        }
    };

    class TidFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const LogMsg &msg) override
        {
            out << msg._tid;
        }
    };

    class LoggerFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const LogMsg &msg) override
        {
            out << msg._logger;
        }
    };

    class TableFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const LogMsg &msg) override
        {
            out << "\t";
        }
    };

    class NlineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const LogMsg &msg) override
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

        void format(std::ostream &out, const LogMsg &msg) override
        {
            out << _s;
        }

    private:
        std::string _s;
    };

    /*
        %d 日期
        %t 线程 id
        %c 日志器名称
        %f 源文件名
        %l 行号
        %p 日志级别
        %T 制表符缩进
        %m 消息主题
        %n 换行
    */

    class Formatter
    {
    public:
        using ptr=std::shared_ptr<Formatter>;
        
        Formatter(const string &pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n")
            : _pattern(pattern)
        {
            assert(parsePattern());
        }

        // 对 msg 进行格式化
        void format(std::ostream &out, const LogMsg &msg)
        {
            for (auto &items : _items)
            {
                items->format(out, msg);
            }
        }

        string format(const LogMsg &msg)
        {
            std::stringstream ss;
            format(ss, msg);
            return ss.str();
        }

    private:
        // 对格式化字符串进行解析
        bool parsePattern()
        {
            std::vector<std::pair<string, string>> fmt_order;
            string key, val;
            size_t pos = 0;
            // abcde[%d{%H%M%S}][%t][%m]
            // 原始字符串

            // 解析格式化字符串，将规则和子项规则放入 fmt_order
            while (pos < _pattern.size())
            {
                if (_pattern[pos] != '%')
                {
                    val.push_back(_pattern[pos++]);
                    continue;
                }
                if (pos + 1 < _pattern.size() && _pattern[pos + 1] == '%')
                {
                    val.push_back('%');
                    pos += 2;
                    continue;
                }

                // 把原始字符串或者% 插入
                if (!val.empty())
                {
                    fmt_order.push_back(std::make_pair(key, val));
                    key.clear();
                    val.clear();
                }

                // 开始处理格式化字符，现在pos是在%的位置
                pos += 1;
                if (pos == _pattern.size())
                {
                    cout << "格式化字符串出错，%后面缺少内容" << endl;
                    return false;
                }
                key.push_back(_pattern[pos]);
                pos += 1;
                if (pos < _pattern.size() && _pattern[pos] == '{')
                {
                    pos += 1;
                    while (pos < _pattern.size() && _pattern[pos] != '}')
                    {
                        val.push_back(_pattern[pos++]);
                    }

                    if (pos == _pattern.size())
                    {
                        cout << "{} 没有匹配成功" << endl;
                        return false;
                    }
                    // 此时 pos 在 } 的位置
                    pos+=1;
                }
                fmt_order.push_back(std::make_pair(key, val));
                key.clear();
                val.clear();
            }


            // 根据fmt_order,构建不同的格式化子项对象指针放入 items
            for (auto &it : fmt_order)
            {
                _items.push_back(createItem(it.first, it.second));
            }
            return true;
        }

    private:
        // 根据不同的格式化字符创建不同的格式化子项对象
        FormatItem::ptr createItem(const string &key, const string &val)
        {
            if (key == "d")
                return std::make_shared<TimeFormatItem>(val);
            if (key == "t")
                return std::make_shared<TidFormatItem>();
            if (key == "c")
                return std::make_shared<LoggerFormatItem>();
            if (key == "f")
                return std::make_shared<FileFormatItem>();
            if (key == "l")
                return std::make_shared<LineFormatItem>();
            if (key == "p")
                return std::make_shared<LevelFormatItem>();
            if (key == "T")
                return std::make_shared<TableFormatItem>();
            if (key == "m")
                return std::make_shared<MsgFormatItem>();
            if (key == "n")
                return std::make_shared<NlineFormatItem>();
            if(key.empty())
                return std::make_shared<OtherFormatItem>(val);
            cout<<"没有对应的匹配规则："<<key<<endl;
            abort();
        }

    private:
        string _pattern; // 格式化规则字符串
        std::vector<FormatItem::ptr> _items;
    };

}

#endif