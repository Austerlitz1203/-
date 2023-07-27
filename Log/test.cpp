#include"util.hpp"

using namespace std;
using namespace log;
using namespace util;

int main()
{
    string pathname("./abc/bcd/test.txt");
    cout<<Date::getTime()<<endl;
    cout<<File::path(pathname)<<endl;
    cout<<File::exists(pathname)<<endl;
    File::createDirectory(pathname);
    cout<<File::exists(pathname)<<endl;
    return 0;
}