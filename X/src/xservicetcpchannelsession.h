#pragma once

#include "xservice.h"
#include <queue>
#include <set>

class XServiceTCPChannelServer;

class XServiceTCPChannelSession : public XService, public std::enable_shared_from_this<XServiceTCPChannelSession>
{
public:
     XServiceTCPChannelSession(json cfg, shared_ptr<XNode> node, shared_ptr<XServiceTCPChannelServer> serviceChannelServer, asio::ip::tcp::socket s, asio::io_context& ctx);
     ~XServiceTCPChannelSession();

    virtual string getType()
    {
        return ServiceNodeChannelTCPSession;
    }

    virtual bool start();

    virtual void input( std::shared_ptr<XMessage> );

    virtual void onSessionMessage( shared_ptr<XMessage> msg );
private:
    void doRead();
    void doReadHeader();
    void doReadBody( XPackageHeader header );

    void doWrite();

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

    string sessionId;
    
    std::queue<shared_ptr<XMessage>> sendMessageQueue;

    std::set<string> regedNodeIds;

    shared_ptr<XServiceTCPChannelServer> tcpChannelServer;

    // 调试用
    std::deque<shared_ptr<XPackage>> lastRecvPackages;
    std::deque<shared_ptr<XMessage>> lastRecvMessages;

};