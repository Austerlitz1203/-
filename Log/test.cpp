#include "util.hpp"
#include "level.hpp"
#include "format.hpp"
#include "message.hpp"
#include "sink.hpp"
#include "logger.hpp"
#include <unistd.h>
#include<vector>

using namespace std;
using namespace log;
using namespace util;

int main()
{
    //  logger 的测试
    string name("test-logger");
    LogLevel::value level = LogLevel::value::WARN;
    Formatter::ptr fmt(new Formatter());
    LogSink::ptr outlpt= SinkFactory::create<StdOutSink>();
    LogSink::ptr filelpt = SinkFactory::create<FileSink>("./logfile/filelpt.log");
    LogSink::ptr rolllpt = SinkFactory::create<RollSinkBySize>("./logfile/roll-", 1024 * 1024);
    vector<LogSink::ptr> ptrs= {outlpt,filelpt,rolllpt};
    SyncLogger::ptr  slogger(new SyncLogger(name,level,fmt,ptrs));

    slogger->debug(__FILE__,__LINE__,"%s","测试日志");
    slogger->info(__FILE__, __LINE__, "%s", "测试日志");
    slogger->warn(__FILE__, __LINE__, "%s", "测试日志");
    slogger->error(__FILE__, __LINE__, "%s", "测试日志");
    slogger->fatal(__FILE__, __LINE__, "%s", "测试日志");

    size_t cursize=0,count=0;
    while(cursize<10*1024*1024)
    {
        slogger->fatal(__FILE__,__LINE__,"测试日志-%d",count++);
        cursize+=20;
    }



    // LogMsg ms(LogLevel::value::DEBUG,111,"test.cpp","root","测试格式化字符串.....");
    // Formatter fm;
    // string s=fm.format(ms);
    // cout<<s<<endl;

    // LogSink::ptr outlpt= SinkFactory::create<StdOutSink>();
    // LogSink::ptr filelpt = SinkFactory::create<FileSink>("./logfile/filelpt.log");
    // LogSink::ptr rolllpt = SinkFactory::create<RollSinkBySize>("./logfile/roll-", 1024 * 1024);
    // outlpt->log(s.c_str(),s.size());
    // filelpt->log(s.c_str(),s.size());

    // size_t cursize=0;
    // size_t count=0;
    // while(cursize<10*1024*1024)
    // {
    //     string tmp=s + to_string(count++);
    //     rolllpt->log(tmp.c_str(),tmp.size());
    //     cursize+=tmp.size();
    // }

    // LogSink::ptr timelpt = SinkFactory::create<RollSinkByTime>("./logfile/time-", GapType::GAP_SECOND);
    // time_t cur=Date::getTime();
    // while (Date::getTime() < cur+5)
    // {
    //     timelpt->log(s.c_str(),s.size());
    //     usleep(100);
    // }

    // string pathname("./abc/bcd/test.txt");
    // cout<<Date::getTime()<<endl;
    // cout<<File::path(pathname)<<endl;
    // cout<<File::exists(pathname)<<endl;
    // File::createDirectory(pathname);
    // cout<<File::exists(pathname)<<endl;

    // cout<<log::LogLevel::toString(log::LogLevel::value::DEBUG)<<endl;
    // cout<<log::LogLevel::toString(log::LogLevel::value::INFO)<<endl;
    // cout<<log::LogLevel::toString(log::LogLevel::value::OFF)<<endl;
    // cout << log::LogLevel::toString(log::LogLevel::value::FATAL) << endl;
    return 0;
}