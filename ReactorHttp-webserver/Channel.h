#pragma once
#include <functional>


// 定义函数指针
// typedef int(*handleFunc)(void* arg);
// using handleFunc = int(*)(void*); // c++11中，函数指针的别名定义

// 定义文件描述符的读写事件
// c11中的强类型枚举，底层是int类型，但是实际使用不能按照int类型来用，如果要按照其他类型来用，需要强制转换
enum class FDEvent 
{
    TimeOut = 0x01,
    ReadEvent = 0x02,
    WriteEvent = 0x04
};

// 可调用对象包装器：1.函数指针 2.可调用对象（可以像函数一样使用）
// 最终得到了地址，但是没有调用
class Channel
{
public:
    // 构造函数, 初始化Channel, 并绑定回调函数
    using handleFunc = std::function<int(void*)>;
    Channel(int fd, FDEvent events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);
    // 回调函数
    handleFunc readCallback;
    handleFunc writeCallback;
    handleFunc destroyCallback;
    // 修改fd的写事件(检测 or 不检测)
    void writeEventEnable(bool flag);
    // 判断是否需要检测文件描述符的写事件
    bool isWriteEventEnable();
    // 取出私有成员的值
    inline int getEvent()
    {
        return m_events;
    }
    inline int getSocket()
    {
        return m_fd;
    }
    inline const void* getArg() // const修饰，不能修改m_arg的值
    {
        return m_arg;
    }
private:
    // 文件描述符
    int m_fd;
    // 事件
    int m_events;
    // 回调函数的参数
    void* m_arg;
};