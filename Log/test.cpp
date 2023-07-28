#include"util.hpp"
#include"level.hpp"

using namespace std;
using namespace log;
using namespace util;

int main()
{
    // string pathname("./abc/bcd/test.txt");
    // cout<<Date::getTime()<<endl;
    // cout<<File::path(pathname)<<endl;
    // cout<<File::exists(pathname)<<endl;
    // File::createDirectory(pathname);
    // cout<<File::exists(pathname)<<endl;

    cout<<log::LogLevel::toString(log::LogLevel::value::DEBUG)<<endl;
    cout<<log::LogLevel::toString(log::LogLevel::value::INFO)<<endl;
    cout<<log::LogLevel::toString(log::LogLevel::value::OFF)<<endl;
    cout << log::LogLevel::toString(log::LogLevel::value::FATAL) << endl;
    return 0;
}