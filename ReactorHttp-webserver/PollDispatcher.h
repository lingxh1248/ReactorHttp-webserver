#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include "Dispatcher.h"
#include <poll.h>
#include <string>
using namespace std;


class PollDispatcher : public Dispatcher
{
public:
    PollDispatcher(EventLoop* evLoop);
    ~PollDispatcher();
    // 添加
    int add() override; // c++11 中，override关键字，表示重写父类虚函数
    // 删除
    int remove();
    // 修改
    int modify();
    // 事件监测
    int dispatch(int timeout = 2); // 单位: s

private:
    int m_maxfd;
    struct pollfd *m_fds;
    const int m_maxNode = 1024;
};