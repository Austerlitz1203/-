#include"../Log/log.hpp"
#include<thread>
#include<vector>
#include<chrono>

using namespace log;
using namespace std;

void bench(const string& logger_name,size_t thr_count,size_t msg_count,size_t msg_len)
{
    // 1. 获取日志器
    Logger::ptr logger=getLogger(logger_name);
    if(logger.get() == nullptr)
        return;

    cout<<"测试日志："<<msg_count<<"条，总大小："<<(msg_count*msg_len) /1024<<"KB"<<endl;
    // 2. 组织指定长度的日志消息
    string msg(msg_len-1,'A');
    // 3. 创建指定数量的线程
    size_t msg_per_thr = msg_count / thr_count;
    vector<double> cost_arr;
    vector<thread> _threads;
    for(int i=0;i<thr_count;i++)
    {
        _threads.emplace_back([&,i](){
            auto start=chrono::high_resolution_clock::now();
            for(int i=0;i<msg_per_thr;i++)
            {
                logger->fatal("%s",msg.c_str());
            }
            auto end=chrono::high_resolution_clock::now();
            std::chrono::duration<double> cost=end-start;
            cost_arr.push_back(cost.count());
            cout<<"线程"<<i<<":"<<"\t输出数量"<<msg_per_thr<<"，耗时"<<cost.count()<<"s"<<endl;
        });
    }

    for(int i=0;i<thr_count;i++)
    {
        _threads[i].join();
    }

    // 4. 由于多线程是并发的，所以耗时最长的那个线程，就是总耗时
    double max_cost = cost_arr[0];
    for(auto &a:cost_arr)
    {
        max_cost = max_cost < a ? a:max_cost;
    }

    size_t msg_per_sec = msg_count / max_cost;
    size_t size_per_sec = (msg_count*msg_len)/(max_cost*1024);
    cout<<"总耗时："<<max_cost<<"s"<<endl;
    cout<<"每秒输出的日志数量："<<msg_per_sec<<"条"<<endl;
    cout<<"每秒输出的日志大小："<<size_per_sec<<"KB"<<endl;

}

void sync_bench()
{
    // 测试样例
    DEBUG("测试开始");
    std::unique_ptr<LoggerBuilder> lb(new GlobalLoggerBuilder());
    lb->buildLoggerName("sync-logger");
    lb->buildLoggerType(LoggerType::SyncLogger);
    lb->buildLoggerLevel(LogLevel::value::DEBUG);
    lb->buildFormatter("[%f][%l][%d{%H:%M:%S}][%p]%T%m%n");
    lb->buildSink<FileSink>("./logfile/sync-test.log");
    lb->build();
    bench("sync-logger",3,1000000,100);
}

void async_bench()
{
    DEBUG("测试开始");
    std::unique_ptr<LoggerBuilder> lb(new GlobalLoggerBuilder());
    lb->buildLoggerName("async-logger");
    lb->buildLoggerType(LoggerType::ASyncLogger);
    lb->buildEnableUnsafe(); // 开启非安全模式，不需要考虑实际写到磁盘的情况
    lb->buildLoggerLevel(LogLevel::value::DEBUG);
    lb->buildFormatter("[%f][%l][%d{%H:%M:%S}][%p]%T%m%n");
    lb->buildSink<FileSink>("./logfile/async-test.log");
    lb->build();
    bench("async-logger", 3, 1000000, 100);
}

int main()
{
    //sync_bench();
    async_bench();
    return 0;
}