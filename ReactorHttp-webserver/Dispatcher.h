#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include <string>
using namespace std;
class EventLoop;
class Dispatcher
{
public:
    Dispatcher(EventLoop* evLoop);
    virtual ~Dispatcher();
    // 添加
    virtual int add();
    // 删除
    virtual int remove();
    // 修改
    virtual int modify();
    // 事件监测
    virtual int dispatch(int timeout = 2); // 单位: s
    inline void setChannel(Channel* channel) { m_channel = channel; }
    
protected: // protected成员可以被派生类访问,但不能被类外访问
    string m_name = string();
    Channel* m_channel;
    EventLoop* m_evLoop;
};