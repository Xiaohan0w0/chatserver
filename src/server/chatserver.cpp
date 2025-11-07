#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"
#include <string>
using json = nlohmann::json;

// 上报连接断开相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 客户端断开连接
    if(!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time)
{
    // 获取用户数据
    string buf = buffer->retrieveAllAsString();
    // 数据的返序列化
    json js = json::parse(buf);
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"]获取 -> 业务handler -> conn js time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}

// 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _loop(loop)
    , _server(loop, listenAddr, nameArg)
{
    // 给服务器注册用户连接断开回调
    _server.setConnectionCallback([this](const TcpConnectionPtr &conn)
                                  { this->onConnection(conn); });
    // 给服务器注册用户消息读写回调
    _server.setMessageCallback([this](const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
                               { this->onMessage(conn, buffer, time); });
    // 设置线程数量
    // 一个主reactor -> I/O线程，负责新用户的连接
    // 三个subreactor -> 工作线程，负责已连接用户的读写事件的处理
    _server.setThreadNum(4);
}

// 启动服务
void ChatServer::start()
{
    _server.start();
}

