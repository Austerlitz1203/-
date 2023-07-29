#ifndef __MY_UTIL__
#define __MY_UTIL__

#include <iostream>
#include <string>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using std::string;

namespace log
{
    namespace util
    {
        class Date
        {
        public:
            static size_t getTime()
            {
                return (size_t)time(nullptr);
            }
        };

        class File
        {
        public:
            static bool exists(const string &pathname)
            {
                struct stat st;
                if (stat(pathname.c_str(), &st) < 0)
                {
                    return false;
                }
                return true;
            }

            static string path(const string &pathname)
            {
                size_t pos = pathname.find_last_of("/\\");
                if (pos == string::npos) // 没找到
                {
                    return ".";
                }
                return pathname.substr(0, pos + 1);
            }

            static void createDirectory(const string &pathname)
            {
                size_t pos = 0, idx = 0;
                while (idx < pathname.size())
                {
                    pos = pathname.find_first_of("/\\", idx); // 从 idx 开始找
                    if (pos == string::npos)
                    {
                        mkdir(pathname.c_str(), 0777);
                        break;
                    }

                    string parent_dir = pathname.substr(0, pos);
                    // if(parent_dir == "." || parent_dir == "..") {idx=pos+1;continue;}
                    if (exists(parent_dir) == true)
                    {
                        idx = pos + 1;
                        continue;
                    }
                    mkdir(parent_dir.c_str(), 0777);
                    idx = pos + 1;
                }
            }
        };
    }
}

#endif // !__MY_UTIL__