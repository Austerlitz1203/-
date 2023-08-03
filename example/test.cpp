#include"../Log/log.hpp"


using namespace std;
using namespace log;
using namespace util;


void test()
{
    Logger::ptr slogger = getLogger("async-logger");
    slogger->debug( "%s", "测试日志");
    slogger->info( "%s", "测试日志");
    slogger->warn( "%s", "测试日志");
    slogger->error("%s", "测试日志");
    slogger->fatal("%s", "测试日志");

}

int main()
{

    // 测试样例
    DEBUG("测试开始");
    std::unique_ptr<LoggerBuilder> lb(new GlobalLoggerBuilder());
    lb->buildLoggerName("async-logger");
    lb->buildLoggerType(LoggerType::SyncLogger);
    lb->buildLoggerLevel(LogLevel::value::DEBUG);
    lb->buildFormatter("[%f][%l][%d{%H:%M:%S}][%p]%T%m%n");
    lb->buildSink<StdOutSink>();
    lb->buildSink<FileSink>("./logfile/async-test.log");
    lb->buildSink<RollSinkBySize>("./logfile/async-test-rollbysize.log",1024*1024*5);
    lb->build();

    test();
    DEBUG("测试结束");

    return 0;
}