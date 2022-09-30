#pragma once

#include "xservice.h"
#include <queue>
#include <set>
#include <deque>

class XServiceTCPChannelClient : public XService, public std::enable_shared_from_this<XServiceTCPChannelClient>
{
public:
     XServiceTCPChannelClient( json cfg, shared_ptr<XNode> node,asio::io_context& context );
     ~XServiceTCPChannelClient();

    virtual string getType()
    {
        return ServiceNodeChannelTCPClient;
    }

    virtual bool start();

    virtual void input( std::shared_ptr<XMessage> );

    virtual void onSessionMessage( shared_ptr<XMessage> msg );

private:
    enum Status
    {
        Idle,       // 空闲状态. 定时器超时动作是doConnect,并进入Connecting状态. 这是初始状态.
        Connecting, // 连接状态. 定时器超时动作是连接失败, 进入Idle状态. 连接失败进入Idle状态.
        Service,    // 工作状态. 定时器超时动作是doKeepalive. doRead/doWrite 失败, 进入Idle状态.
    };
    Status status;
    std::shared_ptr<asio::steady_timer> timer;   // 定时器,状态机超时用.
    void onTimeout();
    void nextStatus( Status nextStatus );
private:
    void doConnect();
    void doRead();
    void doReadHeader();
    void doReadBody( XPackageHeader header );

    void doWrite();
    void doKeepalive();

    void sendHello();
    void sendNodeInfo();

    void onChannelPackage( shared_ptr<XPackage> package );

    void onHello( shared_ptr<XMessage> msg );
    void onNodeInfo( shared_ptr<XMessage> msg);
    void onChannelPayloadMessage( shared_ptr<XMessage> msg );

    void onFail( std::error_code ec );

    void regRemoteNode( string remoteNodeId );
    void unregRemoteNode( string remoteNodeId );
private:
    asio::io_context& asioContext;
    asio::ip::tcp::socket socket;

    string serverHost;
    int serverPort;
    json config;

    std::deque<shared_ptr<XMessage>> sendMessageQueue;

    std::set<string> regedNodeIds;

private:
    // 调试用.

};