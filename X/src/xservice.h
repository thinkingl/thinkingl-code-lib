#pragma once
#include <string>
#include "route.h"
#include "xmessage.h"
#include <memory>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

class XNode;

class XService
{
public:
    // enum ServiceType
    // {
    //     ServiceTCPSourcepoint,      // 一个tcp的起点，监听一个本地端口供客户端来连接，将这个端口的网络交互映射到对应的tcp终点。 
    //     ServiceUDPEndpoint,
    //     ServiceHttpProxyEndpoint,
    //     ServiceEcho,
    //     ServiceTCPEndpointProxy,    // tcp终点服务的代理，1）可接收client的tcp请求。 2）将收到的消息路由到下一跳，直至最终的tcp终点。
    //     ServiceUDPEndpointProxy,
    // };
    static const string ServiceTCPEndpoint;       // 一个tcp的终点，建立和维持与最终tcp服务的数据中转通道。包括 连接、断开，发送，接收。
    static const string ServiceTCPSourcepoint;

    static const string ServiceUDPEndpoint;
    static const string ServiceUDPSourcepoint;

    static const string ServiceEcho;     // 回声服务，原样返回收到的数据，用于测试。

    static const string ServiceNodeChannelTCPClient;  // node之间的传输通道,tcp的.
    static const string ServiceNodeChannelTCPServer;  // node之间的传输通道,tcp的.
    static const string ServiceNodeChannelTCPSession;  // node之间的传输通道,tcp的.

    XService( json cfg, shared_ptr<XNode> node );

    virtual string getId()
    {
        return this->id;
    }

    virtual string getName()
    {
        return this->name;
    }

    virtual string getType() = 0;

    virtual bool start() = 0;

    virtual void input( std::shared_ptr<XMessage> msg )=0;
    virtual void onSessionMessage( std::shared_ptr<XMessage> msg ) = 0;

    void deliverMessage( std::shared_ptr<XMessage> msg );
    void deliverRawMessage( shared_ptr<XMessage> msg );

    shared_ptr<XMessage> createServiceMessage( string type, string session, string toNode, string toService );

protected:
    shared_ptr<XNode> getXNode();
private:
    shared_ptr<XNode> xnode;
    string id;
    string name;
};