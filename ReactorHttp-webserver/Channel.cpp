#include "Channel.h"
#include <stdlib.h>


Channel::Channel(int fd, FDEvent events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg)
{
    m_arg = arg;
    m_fd = fd;
    m_events = static_cast<int>(events);
    readCallback = readFunc;
    writeCallback = writeFunc;
    destroyCallback = destroyFunc;
}

void Channel::writeEventEnable(bool flag)
{
    if (flag)
    {
        // m_events |= (int)FDEvent::WriteEvent; //c语言风格的强制类型转换
        m_events |= static_cast<int>(FDEvent::WriteEvent); //c++风格的强制类型转换
    }
    else
    {
        m_events = m_events & ~static_cast<int>(FDEvent::WriteEvent);
    }
}

bool Channel::isWriteEventEnable()
{
    return m_events & static_cast<int>(FDEvent::WriteEvent);
}