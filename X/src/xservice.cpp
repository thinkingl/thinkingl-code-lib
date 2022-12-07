#include "xservice.h"
#include "xnode.h"
#include <glog/logging.h>

const string XService::ServiceTCPEndpoint = "tcp_endpoint";       // 一个tcp的终点，建立和维持与最终tcp服务的数据中转通道。包括 连接、断开，发送，接收。
const string XService::ServiceTCPSourcepoint = "tcp_sourcepoint";

const string XService::ServiceUDPEndpoint = "udp_endpoint";
const string XService::ServiceUDPSourcepoint = "udp_sourcepoint";

const string XService::ServiceEcho = "echo";     // 回声服务，原样返回收到的数据，用于测试。




const string XService::ServiceNodeChannelTCPClient = "tcp_channel_client";
const string XService::ServiceNodeChannelTCPServer = "tcp_channel_server";
const string XService::ServiceNodeChannelTCPSession = "tcp_channel_session";


XService::XService( json cfg, shared_ptr<XNode> node )
    :xnode( node )
{
    this->id = cfg["id"];
    this->name = cfg["name"];
}

void XService::deliverMessage( std::shared_ptr<XMessage> mesg )
{
    mesg->setFromService( this->getId() );
    //LOG_FIRST_N(INFO,100) << "xservice will deliver message:" << mesg->toJson();
    this->xnode->deliverMessage( mesg );
}

void XService::deliverRawMessage( shared_ptr<XMessage> msg )
{
    this->xnode->deliverRawMessage( msg );
}


std::shared_ptr<XMessage> XService::createServiceMessage( string type, string session, string toNode, string toService)
{
    //XMessage(string type, string toNode, string toService, string session, const XPackage& data );
    std::shared_ptr<XPackage> nullPackage;
    std::shared_ptr<XMessage> message = std::make_shared<XMessage>( type, this->xnode->getId(), this->getId(), toNode, toService, session, nullPackage );
    return message;
}

shared_ptr<XNode> XService::getXNode()
{
    return this->xnode;
}