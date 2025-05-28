#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include "Dispatcher.h"
#include <string>
#include <sys/epoll.h>
using namespace std;


class EpollDispatcher : public Dispatcher
{
public:
    EpollDispatcher(EventLoop* evLoop);
    ~EpollDispatcher();
    // 添加
    int add() override; // c++11 中，override关键字，表示重写父类虚函数
    // 删除
    int remove();
    // 修改
    int modify();
    // 事件监测
    int dispatch(int timeout = 2); // 单位: s
    
private:
    int epollCtl(int op);

private:
    int m_epfd;
    struct epoll_event* m_events;
    const int m_MaxNode = 520;
};