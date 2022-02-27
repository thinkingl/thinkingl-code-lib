#include "xservicetcpchannelclient.h"
#include <glog/logging.h>
#include "xnode.h"

XServiceTCPChannelClient::XServiceTCPChannelClient( json cfg, shared_ptr<XNode> node,asio::io_context& context )
:XService(cfg, node )
,asioContext(context)
,socket(context)
,serverHost(cfg["server"]["host"])
,serverPort(cfg["server"]["port"].get<int>() )
{

}

XServiceTCPChannelClient::~XServiceTCPChannelClient()
{
    LOG(INFO) << "unconstruct of XServiceTCPChannelClient, this:" << this;
}

bool XServiceTCPChannelClient::start()
{
    this->doConnect();
    return true;
}

void XServiceTCPChannelClient::doConnect()
{
    auto self = this->shared_from_this();
    stringstream ss;
    ss << this->serverPort;
    auto serverEndpoint = asio::ip::tcp::resolver( this->asioContext ).resolve( this->serverHost, ss.str() );
    LOG(INFO) << "TCP channel client will connect to: -----------" ;
    for( auto it : serverEndpoint )
    {
        LOG(INFO) << it.endpoint();
    }
    LOG(INFO) << "----------------------end";

    this->sendMessageQueue.clear();

    //serverEndpoint.port( this->serverPort );
    asio::async_connect( this->socket,
        serverEndpoint,
        [this,self]( std::error_code ec, asio::ip::tcp::endpoint endpoint )
        {
            if (!ec)
            {
                LOG(INFO) << "connect to tcp channel server [" << endpoint << "] success!";

                this->sendHello();

                this->doRead();
                this->doWrite();
            }
            else
            {
                LOG(ERROR) << "connect to tcp channel server [" << this->serverHost << ":" << this->serverPort << "] fail! ec:[" << ec << "]";

                auto timerRetry = make_shared<asio::steady_timer>(this->asioContext);
                timerRetry->expires_after( std::chrono::seconds(30) );
                timerRetry->async_wait([this,self,timerRetry](std::error_code ec)
                    {
                        if( !ec )
                        {
                            LOG(INFO) << "will reconnect to tcp channel server " << this->serverHost << ":" << this->serverPort;
                            this->doConnect();
                        }
                        else
                        {
                            LOG(ERROR) << "timer for reconnecting fail! ec:[" << ec << "]";
                        }
                        timerRetry->cancel();
                    }
                    );

            }
        } );
}

void XServiceTCPChannelClient::sendHello()
{
    LOG(INFO) << "will say hello to tcp channel server " << this->serverHost << ":" << this->serverPort;
    auto helloMsg = this->createServiceMessage( XMessage::MessageChannelHello, "", "", "" );

    bool sending = !this->sendMessageQueue.empty();
    this->sendMessageQueue.push_back( helloMsg );
    

    /*

    auto helloPackage = helloMsg->toXPackage();

    auto self = this->shared_from_this();
    asio::async_write( this->socket,
        asio::buffer( helloPackage->data(), helloPackage->totalLength() ),
        [this,self,helloPackage](std::error_code ec, std::size_t length)
        {
            if( !ec )
            {
                LOG(INFO) << "tcp channel client hello success!";
            }
            else
            {
                LOG(ERROR) << "tcp channel client hello fail! ec: " << ec;
            }
        });
    */
}

void XServiceTCPChannelClient::doRead()
{
    this->doReadHeader();
}


void XServiceTCPChannelClient::doReadHeader()
{
    auto self( this->shared_from_this() );
    XPackageHeader* header = new XPackageHeader();
    asio::async_read( this->socket,
        asio::buffer( header, XPackage::header_length ),
        [this, self, header](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                //LOG_FIRST_N(INFO,100) << "tcp channel client doReadHeader, bodyLen:" << package->bodyLength();
                doReadBody( *header );
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

void XServiceTCPChannelClient::doReadBody( XPackageHeader header )
{
    if( !header.checkValid() )
    {
        LOG(ERROR) << "tcp channel client recv Invalid header!";
        char dumpHeader[ XPackage::header_length+1] = {0};
        memcpy(dumpHeader, &header, XPackage::header_length );
        LOG(ERROR) << "dump invalid header:[" << dumpHeader << "]";
        cout << "-----------------" << endl;
        for( int i=0; i<XPackage::header_length; ++i )
        {
            cout << setw(2) << setfill('0') << hex << (int)((unsigned char*)(&header))[i] << " ";
        }
        cout << endl << "-----------------" << endl;

        this->onFail( std::error_code() );
        return;
    }

    auto package = std::make_shared<XPackage>( header );
    auto self( this->shared_from_this() );
    asio::async_read( this->socket,
        asio::buffer(package->body(), package->bodyLength() ),
        [this, self, package](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                LOG_ASSERT( package->bodyLength() == length );
                
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

void XServiceTCPChannelClient::onChannelPackage( shared_ptr<XPackage> package )
{
    auto msg = XMessage::fromXPackageJsonBody( package );
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
        auto payloadMsg = XMessage::fromXPackageJsonBody( msg->getData() );

        this->onChannelPayloadMessage( payloadMsg );
    }
}

void XServiceTCPChannelClient::onHello( shared_ptr<XMessage> msg )
{
    // 
    string nodeId = msg->getFromNode();
    LOG(INFO) << "tcp channel client recv hello from node " << nodeId;
    this->regRemoteNode( nodeId );

    // 收到hello,发送自己的节点信息.
    this->sendNodeInfo();
}

void XServiceTCPChannelClient::onNodeInfo( shared_ptr<XMessage> msg )
{
    // todo
    // 根据对方的信息,更新路由信息.
}

void XServiceTCPChannelClient::onFail( std::error_code ec )
{
    LOG(ERROR) << "tcp channel client fail! ec:" << ec;

    set<string> tmpRegedNodeIds = this->regedNodeIds;
    for( auto n : tmpRegedNodeIds )
    {
        this->unregRemoteNode(n);
    }
    this->regedNodeIds.clear();

    // 重连.
    auto self( this->shared_from_this() );
    auto timerRetry = make_shared<asio::steady_timer>(this->asioContext);
    timerRetry->expires_after( std::chrono::seconds(30) );
    timerRetry->async_wait([this,self,timerRetry](std::error_code ec)
        {
            if( !ec )
            {
                LOG(INFO) << "on fail will reconnect to tcp channel server " << this->serverHost << ":" << this->serverPort;
                this->doConnect();
            }
            else
            {
                LOG(ERROR) << "timer for reconnecting fail! ec:[" << ec << "]";
            }
            timerRetry->cancel();
        }
        );
    // 通知相关的node都断链? 还是不要做过多控制,只在两端做控制.
}

void XServiceTCPChannelClient::onChannelPayloadMessage( shared_ptr<XMessage> payloadMsg )
{
    //VLOG(5) << "channel recv payload msg: " << payloadMsg->toJson();
    if( payloadMsg->getToService() == XService::ServiceEcho || payloadMsg->getFromService() == XService::ServiceEcho )
    {
        LOG(INFO) << "tcp channel client recv echo payload: " << payloadMsg->toJson();
    }
    this->deliverRawMessage( payloadMsg );
}

void XServiceTCPChannelClient::sendNodeInfo()
{
    LOG(INFO) << "todo! tcp channel client will send node info to server.";
    // todo
}

void XServiceTCPChannelClient::onSessionMessage( shared_ptr<XMessage> msg )
{

}

void XServiceTCPChannelClient::input( shared_ptr<XMessage> payloadMsg )
{
    auto payloadPackage = payloadMsg->toXPackage();

    auto msg = make_shared<XMessage>(XMessage::MessageChannelPayload, "", payloadPackage );
    //LOG_FIRST_N(INFO,100) << "tcp channel client input msg:" << msg->toJson();
    bool sending = !this->sendMessageQueue.empty();
    this->sendMessageQueue.push_back( msg );
    if( !sending )
    {
        this->doWrite();
    }
}

void XServiceTCPChannelClient::doWrite()
{
    if( this->sendMessageQueue.empty() )
    {
        return;
    }
    auto payloadMsg = this->sendMessageQueue.front();

    auto package = payloadMsg->toXPackage();

    auto self(shared_from_this());
    asio::async_write( this->socket,
        asio::buffer( package->data(), package->totalLength() ),
        [this, self,package](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
              if( !this->sendMessageQueue.empty() )
              {
                this->sendMessageQueue.pop_front();
              }
            if (!sendMessageQueue.empty())
            {
              doWrite();
            }
          }
          else
          {
              LOG(ERROR) << "tcp channel client doWrite fail! ec:[" << ec << "]";
              this->onFail( ec );
          }
    });
}


void XServiceTCPChannelClient::regRemoteNode( string remoteNodeId )
{
    LOG(INFO) << "tcp channel client reg remote node: " << remoteNodeId;
    this->getXNode()->regRemoteNodeRouteService( remoteNodeId, this->shared_from_this() );
    this->regedNodeIds.insert( remoteNodeId );
}

void XServiceTCPChannelClient::unregRemoteNode( string remoteNodeId )
{
    LOG(INFO) << "tcp channel client unreg remote node: " << remoteNodeId;
    this->getXNode()->unregRemoteNodeRouteService( remoteNodeId, this->shared_from_this() );
    this->regedNodeIds.erase( remoteNodeId );
}