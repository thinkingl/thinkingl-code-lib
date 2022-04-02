#include "xservicetcpchannelsession.h"
#include <glog/logging.h>
#include "xservicetcpchannelserver.h"
#include "xnode.h"
#include <iomanip>


XServiceTCPChannelSession::XServiceTCPChannelSession(json cfg, shared_ptr<XNode> node, shared_ptr<XServiceTCPChannelServer> channelServer, asio::ip::tcp::socket s, asio::io_context& ctx )
:XService(cfg, node )
,asioContext( ctx )
,socket(std::move(s))
,sessionId( cfg["id"] )
,tcpChannelServer( channelServer )
{
    LOG(INFO) << "construct of XServiceTCPChannelSession, sessionId:" << this->sessionId;
}

XServiceTCPChannelSession::~XServiceTCPChannelSession()
{
    LOG(INFO) << "unconstruct of XServiceTCPChannelSession, sessionId:" << this->sessionId;
}

bool XServiceTCPChannelSession::start()
{
    this->sendHello();
    this->doRead();
    return true;
}

void XServiceTCPChannelSession::sendHello()
{
    LOG(INFO) << "will say hello to tcp channel peer, local id: " << this->getId() << " - " << this->getName();
    auto helloMsg = this->createServiceMessage( XMessage::MessageChannelHello, "", "", "" );

    bool sending = !this->sendMessageQueue.empty();
    this->sendMessageQueue.push( helloMsg );
    if( !sending )
    {
        this->doWrite();
    }

    /*
    auto helloPackage = helloMsg->toXPackage();

    auto self = this->shared_from_this();
    asio::async_write( this->socket,
        asio::buffer( helloPackage->data(), helloPackage->header_length + helloPackage->bodyLength() ),
        [this,self,helloPackage](std::error_code ec, std::size_t length)
        {
            if( !ec )
            {
                LOG(INFO) << "tcp channel session hello success!";
            }
            else
            {
                LOG(ERROR) << "tcp channel session hello fail! ec: " << ec;
            }
        });
        */
}

void XServiceTCPChannelSession::doRead()
{
    this->doReadHeader();
}

void XServiceTCPChannelSession::doReadHeader()
{
    auto self( this->shared_from_this() );
    auto header = new XPackageHeader();
    asio::async_read( this->socket,
        asio::buffer((void*)header, XPackage::header_length ),
        [this, self, header](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                doReadBody( *header );
                LOG_FIRST_N(INFO,100) << "tcp channal session read header, bodyLen:[" << header->bodyLength() << "]";
            }
            else
            {
                LOG(ERROR) << "xpackage stream socket read head fail! ec:[" << ec << "]";
                this->onFail( ec );
            }
            delete header;
        }
    );
}

void XServiceTCPChannelSession::doReadBody( XPackageHeader header )
{
    if( !header.checkValid() )
    {
        LOG(ERROR) << "tcp channel session recv Invalid header!";
        char dumpHeader[ XPackage::header_length+1] = {0};
        memcpy(dumpHeader, &header, XPackage::header_length );
        LOG(ERROR) << "dump invalid header:[" << dumpHeader << "]";
        cout << "-----------------" << endl;
        for( int i=0; i<XPackage::header_length; ++i )
        {
            cout << setw(2) << setfill('0') << hex << (int)((unsigned char*)(&header))[i] << " ";
        }
        cout << endl << "-----------------" << endl;

        LOG(ERROR) << "------------dump last messages start ------------";
        for( auto msg : this->lastRecvMessages )
        {
            LOG(ERROR) << msg->toJson();
        }
        LOG(ERROR) << "------------dump last messages end ------------";
        
        LOG(ERROR) << "------------dump last package start ------------";
        for( auto package : this->lastRecvPackages )
        {
            LOG(ERROR) << "body len:" << package->bodyLength() << " body: " << (char*)package->body();
        }
        LOG(ERROR) << "------------dump last package end ------------";
        std::error_code ec;
        this->onFail( ec );
        return;
    }
    auto package = std::make_shared<XPackage>( header );
    auto self( this->shared_from_this() );
    asio::async_read( this->socket,
        asio::buffer( package->body(), package->bodyLength() ),
        [this, self, package](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                LOG_FIRST_N(INFO,100) << "tcp channel session read body len:[" << length << "] body:[" << (char*)package->body() << "]";
                // 将读到的package解析后送出. 
                this->onChannelPackage( package );

                this->doReadHeader(); // 读下一个消息.
            }
            else
            {
                LOG(ERROR) << "xpackage stream socket read body fail! ec:[" << ec << "]";
                this->onFail( ec );
            }
        }
    );
}

void XServiceTCPChannelSession::onChannelPackage( shared_ptr<XPackage> package )
{
    auto msg = XMessage::fromXPackageJsonBody( package );

    // debug
    this->lastRecvMessages.push_back( msg );
    this->lastRecvPackages.push_back( package );
    while( lastRecvMessages.size() > 10 )
    {
        lastRecvMessages.pop_front();
    }
    while( lastRecvPackages.size() > 10 )
    {
        lastRecvPackages.pop_front();
    }

    if( XMessage::MessageChannelHello == msg->getType() )
    {
        this->onHello( msg );
    }
    else if( XMessage::MessageChannelNodeInfo == msg->getType() )
    {
        this->onNodeInfo( msg );
    }
    else if( XMessage::MessageChannelPayload == msg->getType() )
    {
        // 包了一层.

        this->onChannelPayloadMessage( msg );
    }
}

void XServiceTCPChannelSession::onHello( shared_ptr<XMessage> msg )
{
    // 
    string nodeId = msg->getFromNode();
    LOG(INFO) << "tcp channel session recv hello from node " << nodeId;
    this->regRemoteNode( nodeId );

    // 收到hello,发送自己的节点信息.
    this->sendNodeInfo();
}

void XServiceTCPChannelSession::onNodeInfo( shared_ptr<XMessage> msg )
{
    // todo
    // 根据对方的信息,更新路由信息.
}

void XServiceTCPChannelSession::onFail( std::error_code ec )
{
    LOG(ERROR) << "tcp channel session fail! ec:" << ec;

    set<string> tmpRegedNodeIds = this->regedNodeIds;
    for( auto n : tmpRegedNodeIds )
    {
        this->unregRemoteNode(n);
    }
    this->regedNodeIds.clear();

    this->tcpChannelServer->removeSession( this->sessionId );
}

void XServiceTCPChannelSession::onChannelPayloadMessage( shared_ptr<XMessage> msg )
{
    auto payloadMsg = XMessage::fromXPackageJsonBody( msg->getData() );
    //VLOG(5) << "channel recv payload msg: " << payloadMsg->toJson();
    //LOG_FIRST_N(INFO,100) << "tcp channel session recv payload msg:" << payloadMsg->toJson();
    if( payloadMsg->getToService() == XService::ServiceEcho || payloadMsg->getFromService() == XService::ServiceEcho )
    {
        LOG(INFO) << "tcp channel session recv echo payload: " << payloadMsg->toJson();
    }
    this->deliverRawMessage( payloadMsg );
}

void XServiceTCPChannelSession::sendNodeInfo()
{
    LOG(INFO) << "todo! tcp channel session will send node info to server.";
    // todo
}

void XServiceTCPChannelSession::onSessionMessage( shared_ptr<XMessage> msg )
{

}

void XServiceTCPChannelSession::input( shared_ptr<XMessage> payloadMsg )
{
    auto payloadPackage = payloadMsg->toXPackage();
    auto msg = make_shared<XMessage>(XMessage::MessageChannelPayload, "", payloadPackage );

    bool sending = !this->sendMessageQueue.empty();
    this->sendMessageQueue.push( msg );
    if( !sending )
    {
        this->doWrite();
    }
}

void XServiceTCPChannelSession::doWrite()
{
    auto msg = this->sendMessageQueue.front();
    auto package = msg->toXPackage();

    //LOG_FIRST_N(INFO, 100) << "doWrite package, bodyLen:" << package->bodyLength();
    auto self(shared_from_this());
    asio::async_write( this->socket,
        asio::buffer( package->data(), package->header_length + package->bodyLength() ),
        [this, self, package](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            this->sendMessageQueue.pop();
            if (!sendMessageQueue.empty())
            {
              doWrite();
            }
          }
          else
          {
              LOG(ERROR) << "tcp channel session doWrite fail! ec:[" << ec << "]";
              this->onFail( ec );
          }
    });
}

void XServiceTCPChannelSession::regRemoteNode( string remoteNodeId )
{
    LOG(INFO) << "tcp channel session reg remote node: " << remoteNodeId;
    this->getXNode()->regRemoteNodeRouteService( remoteNodeId, this->shared_from_this() );
    this->regedNodeIds.insert( remoteNodeId );
}

void XServiceTCPChannelSession::unregRemoteNode( string remoteNodeId )
{
    LOG(INFO) << "tcp channel session unreg remote node: " << remoteNodeId;
    this->getXNode()->unregRemoteNodeRouteService( remoteNodeId, this->shared_from_this() );
    this->regedNodeIds.erase( remoteNodeId );
}