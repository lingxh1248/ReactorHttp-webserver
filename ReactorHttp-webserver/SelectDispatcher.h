#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include "Dispatcher.h"
#include <sys/select.h>
#include <string>
using namespace std;


class SelectDispatcher : public Dispatcher
{
public:
    SelectDispatcher(EventLoop* evLoop);
    ~SelectDispatcher();
    // 添加
    int add() override; // c++11 中，override关键字，表示重写父类虚函数
    // 删除
    int remove();
    // 修改
    int modify();
    // 事件监测
    int dispatch(int timeout = 2); // 单位: s

private:
    void setFdSet();
    void clearFdSet();

private:
    fd_set m_readSet;
    fd_set m_writeSet;
    const int m_maxSize = 1024;
};