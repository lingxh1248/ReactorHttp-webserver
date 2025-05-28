#include "TcpServer.h"
#include <arpa/inet.h>
#include "TcpConnection.h"
#include <stdio.h>
#include <stdlib.h>
#include "Log.h"


TcpServer::TcpServer(unsigned short port, int threadNum)
{
    m_port = port;
    m_mainLoop = new EventLoop();
    m_threadNum = threadNum;
    m_threadPool = new ThreadPool(m_mainLoop, m_threadNum);
    setListner();
}


void TcpServer::setListner()
{
    // 1. 创建监听的fd
    m_lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_lfd == -1)
    {
        perror("socket");
        return;
    }
    // 2. 设置端口复用
    int opt = 1;
    int ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
    if (ret == -1)
    {
        perror("setsockopt");
        return;
    }
    // 3. 绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(m_lfd, (struct sockaddr*)&addr, sizeof addr);
    if (ret == -1)
    {
        perror("bind");
        return;
    }
    // 4. 设置监听
    ret = listen(m_lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        return;
    }
}

void TcpServer::run()
{
     Debug("服务器程序启动...");
    // 启动线程池
    m_threadPool->run(); // 启动服务器后在启动线程池
    // 添加检测的任务
    // 初始化一个channel实例
    // channel实例中包含一个监听的fd，检测的是读事件，当这个读事件触发时，相应的acceptConnection回调函数会被调用
    // 然后把这个channel对象添加到主反应堆模型对应的任务队列中，这个主反应堆模型对应的是主线程
    // 由于在初始化线程池后，需要将线程池中的子线程添加到主反应堆实例中，因此一定要保证线程池中的子线程都被创建成功后，才能让主线程继续向下执行，所以在初始化线程池的时候，对主线程进行了阻塞，确保子线程都被创建成功让主线程往子线程对应的反应堆中添加任务
    Channel *channel = new Channel(m_lfd, FDEvent::ReadEvent, acceptConnection, nullptr, nullptr, this); 
    m_mainLoop->addTask(channel, ElemType::ADD);
    // 启动反应堆模型
    // 用于监听的fd的读事件的处理，也就是acceptConnection函数的执行是在主线程里面完成的
    m_mainLoop->run();
}

int TcpServer::acceptConnection(void* arg)
{
    TcpServer* server = static_cast<TcpServer*>(arg);
    // 和客户端建立连接
    // 通过server里面的监听的fd来和客户端建立连接，得到一个和客户端通信的fd
    int cfd = accept(server->m_lfd, NULL, NULL);
    // 从线程池中取出一个子线程的反应堆实例, 去处理这个cfd
    EventLoop* evLoop = server->m_threadPool->takeWorkerEventLoop();
    // 将cfd放到 TcpConnection中处理
    new TcpConnection(cfd, evLoop);
    return 0;
}