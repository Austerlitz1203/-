#include"util.hpp"
#include"level.hpp"
#include"format.hpp"
#include"message.hpp"

using namespace std;
using namespace log;
using namespace util;

int main()
{
    LogMsg ms(LogLevel::value::DEBUG,111,"test.cpp","root","测试格式化字符串.....");
    Formatter fm("%%abcdef[%d{%H:%M:%S}][%t][%l][%p]%m%n");
    string s=fm.format(ms);
    cout<<s<<endl;

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