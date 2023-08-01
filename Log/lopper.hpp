#ifndef __MY_LOPPER__
#define __MY_LOPPER__

#include "buffer.hpp"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include<atomic>

namespace log
{
    using func_t = std::function<void(Buffer &)>;

    // 是否安全，即当剩余空间不够时，是选择阻塞（安全），还是扩容（不安全）
    enum class AsyncType
    {
        ASYNC_SAFE,
        ASYNC_UNSAFE
    };

    class AsyncLopper
    {
    public:
        using ptr = std::shared_ptr<AsyncLopper>;
        AsyncLopper(func_t fun, AsyncType atype = AsyncType::ASYNC_SAFE)
            : _thread(std::thread(&AsyncLopper::threadEntry, this))
            , _stop(false)
            , _fun(fun)
            , _atype(atype)
        {
        }

        ~AsyncLopper()
        {
            stop();
        }

         void stop()
        {
            _stop = true;
            _con_cond.notify_all(); // 唤醒所有的工作线程
            _thread.join(); // 等待工作线程的退出
        }

        void push(const char *data, size_t len)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            // 条件变量控制——如果缓冲区剩余空间大小 大于等于 要写入数据的大小，才被唤醒(这个的前提是，空间不够则阻塞，而非扩容)
            if(_atype == AsyncType::ASYNC_SAFE)
                _pro_cond.wait(lock, [&]()
                           { return _pro_buf.writeAbleSize() >= len; });
            // 满足条件才能走下俩
            _pro_buf.push(data, len);
            _con_cond.notify_one(); // 唤醒一个消费者去处理
        }

    private:
        void threadEntry() // 线程入口函数
        {
            while (1)
            {
                // 判断生产者缓冲区（第一次进入，消费者缓冲区肯定无数据）有无数据，有则交换，无则阻塞
                {
                    std::unique_lock<std::mutex> lock(_mutex);
                    // 退出标志被设置，并且生产缓冲区无数据，才可以退出，否则会出现退出标志被设置，但是生产缓冲区还有数据就退出的情况
                    if( _stop && _pro_buf.empty()) break;
                    // 1.要生产者缓冲区不为空才停止等待
                    _con_cond.wait(lock, [&]()
                                   { return  _stop || !_pro_buf.empty(); });
                    _con_buf.swap(_pro_buf);
                    // 2.唤醒生产者 (在安全状态才需要考虑唤醒，因为只有安全状态才会陷入阻塞_pro_cond.wait)
                    if(_atype == AsyncType::ASYNC_SAFE)
                        _pro_cond.notify_all();
                }

                // 3.对消费缓冲区的数据进行处理
                _fun(_con_buf);
                // 4.初始化消费缓冲区
                _con_buf.reset();
            }
        }

    private:
        func_t _fun; // 具体对缓冲区数据进行处理的回调函数，由异步工作器的使用者传入
    private:
        std::atomic<bool> _stop; // 工作器停止标志
        Buffer _pro_buf;
        Buffer _con_buf;
        std::mutex _mutex;
        std::condition_variable _pro_cond;
        std::condition_variable _con_cond;
        std::thread _thread;
        AsyncType _atype;
    };

}

#endif
