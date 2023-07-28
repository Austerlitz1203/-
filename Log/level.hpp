
#ifndef MY_LEVEL_H

#define MY_LEVEL_H

namespace log
{
    class LogLevel
    {
    public:
        enum class value {
            UNKNOW = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            OFF
        };

        static const char* toString(log::LogLevel::value lev)
        {
            switch (lev)
            {
            case log::LogLevel::value::UNKNOW:
                return "UNKNOW";
            case log::LogLevel::value::DEBUG:
                return "DEBUG";
            case log::LogLevel::value::INFO:
                return "INFO";
            case log::LogLevel::value::WARN:
                return "WARN";
            case log::LogLevel::value::ERROR:
                return "ERROR";
            case log::LogLevel::value::FATAL:
                return "FATAL";
            case log::LogLevel::value::OFF:
                return "OFF";
            default:
                return "UNKNOW";
            }
            return "UNKNOW";
        }


    };
}

#endif