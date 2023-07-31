#ifndef __MY_BUFFER__
#define __MY_BUFFER__

#include<vector>
#include<iostream>
#include<cassert>
namespace log
{
    #define DEFULT_BUFFER_SIZE (10*1024*1024)
    #define THRESHOLD_BUFFER_SIZE (80*1024*1024)
    #define INCREMENT_BUFFER_SIZE (10*1024*1024)
    class Buffer
    {
    public:
        Buffer()
            :_buffer(DEFULT_BUFFER_SIZE)
            ,_reader_idx(0)
            ,_writer_idx(0)
        {}
        // 向缓冲区写入数据
        void push(const char* data,size_t len)
        {
            // 考虑到缓冲区可能不够的情况，两种方法：1.错误，返回。2.扩容。
            //if(len > writeAbleSize()) return;
            ensureEnoughSize(len);
            std::copy(data,data+len,&_buffer[_writer_idx]);
            moveWriter(len);
        }
        // 返回可读数据的起始地址
        const char* begin()
        {
            return &_buffer[_reader_idx];
        }
        // 返回可读数据的长度
        size_t readAbleSize()
        {
            return (_writer_idx - _reader_idx);
        }
        size_t writeAbleSize()
        {
            return (_buffer.size() - _writer_idx);
        }
        // 重置读写位置，初始化缓冲区
        void reset()
        {
            _reader_idx = 0;
            _writer_idx = 0;
        }
        // 对 Buffer 实现交换操作
        void swap(Buffer& buffer)
        {
            _buffer.swap(buffer._buffer);
            std::swap(_reader_idx,buffer._reader_idx);
            std::swap(_writer_idx,buffer._writer_idx);
        }
        // 判断缓冲区是否为空
        bool empty()
        {
            return (_reader_idx == _writer_idx);
        }

        void moveReader(size_t len)
        {
            assert(len <= readAbleSize()); // 要加上等于号
            _reader_idx += len;
        }

    private:
        void moveWriter(size_t len)
        {
            assert(len <= writeAbleSize());
            _writer_idx+=len;
        }

        void ensureEnoughSize(size_t len)
        {
            if(len <= writeAbleSize()) return;
            size_t newsize=0;
            if(_buffer.size() < THRESHOLD_BUFFER_SIZE)
                newsize=_buffer.size()*2 + len;
            else
                newsize=_buffer.size()+INCREMENT_BUFFER_SIZE + len;
            _buffer.resize(newsize);
        }

    private:
        std::vector<char> _buffer;
        size_t _reader_idx;
        size_t _writer_idx;
    };


}



#endif