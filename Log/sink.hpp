#ifndef __MY_SINK__
#define __MY_SINK__

#include "util.hpp"
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

using std::string;

namespace log
{

    // 日志落地类
    class LogSink
    {
    public:
        using ptr = std::shared_ptr<LogSink>;
        LogSink() {}
        virtual ~LogSink() {}
        virtual void log(const char *data, size_t len) = 0;
    };

    // 落地方向：标准输出
    class StdOutSink : public LogSink
    {
    public:
        void log(const char *data, size_t len) override
        {
            std::cout.write(data, len);
        }
    };

    // 落地方向：指定文件
    class FileSink : public LogSink
    {
    public:
        FileSink(const string &file)
            : _filename(file)
        {
            // 1. 创建文件所在目录
            log::util::File::createDirectory(log::util::File::path(_filename));
            // 2. 打开文件
            _ofs.open(_filename, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }

        void log(const char *data, size_t len) override
        {
            _ofs.write(data, len);
            assert(_ofs.good());
        }

    private:
        string _filename;
        std::ofstream _ofs;
    };

    // 落地方向：滚动文件
    class RollSinkBySize : public LogSink
    {
    public:
        // 构造时传入文件名并打开文件
        RollSinkBySize(const string &filepath, size_t max_size)
            : _basename(filepath), _max_fsize(max_size), _cur_fsize(0), _name_count(0)
        {
            string _filename = CreateNewFile();
            // 1. 创建文件所在目录
            log::util::File::createDirectory(log::util::File::path(_filename));
            // 2. 打开文件
            _ofs.open(_filename, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }

        void log(const char *data, size_t len) override
        {
            if (_cur_fsize >= _max_fsize)
            {
                _ofs.close();
                string _filename = CreateNewFile();
                _ofs.open(_filename, std::ios::binary | std::ios::app);
                _cur_fsize = 0;
                assert(_ofs.is_open());
            }
            _ofs.write(data, len);
            _cur_fsize += len;
            assert(_ofs.good());
        }

    private:
        // 进行大小判断，超过 _max_fsize 则创建新的文件
        string CreateNewFile()
        {
            std::stringstream ss;
            time_t t = util::Date::getTime();
            struct tm lt;
            localtime_r(&t, &lt);
            ss << _basename;
            ss << lt.tm_year + 1900;
            ss << lt.tm_mon + 1;
            ss << lt.tm_mday;
            ss << lt.tm_hour;
            ss << lt.tm_min;
            ss << lt.tm_sec; // 可能一秒内创建多个文件
            ss << "-";
            ss << _name_count++; // 区分一下
            ss << ".log";
            return ss.str();
        }

    private:
        // 基础文件名+扩展文件名生成一个完整的文件名
        string _basename;
        size_t _max_fsize; // 最大的大小
        size_t _cur_fsize; // 已经写入的大小
        std::ofstream _ofs;
        size_t _name_count;
    };

    // 落地方向：时间 。
    /*
     以时间进行文件滚动，实际上就是以时间段进行滚动
    */

    enum class GapType
    {
        GAP_SECOND,
        GAP_MINUTE,
        GAP_HOUR,
        GAP_DAY,
    };

    class RollSinkByTime : public LogSink
    {
    public:
        // 构造时传入文件名并打开文件
        RollSinkByTime(const string &filepath, GapType type)
            : _basename(filepath)
        {
            switch (type)
            {
            case GapType::GAP_SECOND:
                _gap_size = 1;
                break;
            case GapType::GAP_MINUTE:
                _gap_size = 60;
                break;
            case GapType::GAP_HOUR:
                _gap_size = 3600;
                break;
            case GapType::GAP_DAY:
                _gap_size = 24*3600;
                break;
            }
            _cur_gap = _gap_size == 1 ? log::util::Date::getTime() : log::util::Date::getTime() % _gap_size;
            string _filename = CreateNewFile();
            // 1. 创建文件所在目录
            log::util::File::createDirectory(log::util::File::path(_filename));
            // 2. 打开文件
            _ofs.open(_filename, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }

        void log(const char *data, size_t len) override
        {
            time_t cur=log::util::Date::getTime();
            if((_gap_size != 1 && (cur % _gap_size != _cur_gap))
             || (_gap_size == 1 && (cur != _cur_gap)))
            {
                if(_gap_size == 1) _cur_gap = cur;
                else _cur_gap = cur%_gap_size;

                _ofs.close();
                string _filename = CreateNewFile();
                _ofs.open(_filename, std::ios::binary | std::ios::app);
                assert(_ofs.is_open());
            }
            _ofs.write(data,len);
            assert(_ofs.good());
        }

    private:

        string CreateNewFile()
        {
            std::stringstream ss;
            time_t t = util::Date::getTime();
            struct tm lt;
            localtime_r(&t, &lt);
            ss << _basename;
            ss << lt.tm_year + 1900;
            ss << lt.tm_mon + 1;
            ss << lt.tm_mday;
            ss << lt.tm_hour;
            ss << lt.tm_min;
            ss << lt.tm_sec;
            ss << ".log";
            return ss.str();
        }

    private:
        // 基础文件名+扩展文件名生成一个完整的文件名
        string _basename;
        size_t _cur_gap; // 当前是第几个时间段
        size_t _gap_size;
        std::ofstream _ofs;
    };

    // 模板来实现不同种类的落地，可变参数包实现不同参数个数的传参
    class SinkFactory
    {
    public:
        template <typename LogType, typename... Args>
        static LogSink::ptr create(Args &&...args)
        {
            return std::make_shared<LogType>(std::forward<Args>(args)...);
        }
    };
}

#endif