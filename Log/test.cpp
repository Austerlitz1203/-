#include"util.hpp"
#include"level.hpp"
#include"format.hpp"
#include"message.hpp"
#include"sink.hpp"
#include<unistd.h>

using namespace std;
using namespace log;
using namespace util;

int main()
{
    LogMsg ms(LogLevel::value::DEBUG,111,"test.cpp","root","测试格式化字符串.....");
    Formatter fm;
    string s=fm.format(ms);
    cout<<s<<endl;

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


    LogSink::ptr timelpt = SinkFactory::create<RollSinkByTime>("./logfile/time-", GapType::GAP_SECOND);
    time_t cur=Date::getTime();
    while (Date::getTime() < cur+5)
    {
        timelpt->log(s.c_str(),s.size());
        usleep(100);
    }

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