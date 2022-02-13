#include "xservicetcpsourcepoint.h"

#include <asio.hpp>
#include <glog/logging.h>
#include <sstream>
#include <string>

#include "sessionstream.h"
#include "xpackage.h"

XServiceTCPSourcepoint::XServiceTCPSourcepoint( json cfg, shared_ptr<XNode> node,asio::io_context& context )
    :XService( cfg, node )
    ,acceptor( context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), cfg["port"].get<int>() ) )
{
    LOG(INFO) << "service " << this->getType() << " port: " << cfg["port"].get<int>() << " created! cfg: " << cfg.dump();

    this->toNode = cfg["dst_node"];
    this->toService = cfg["dst_service"];

    //asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 2284);
    //asio::ip::tcp::endpoint e() 

}

bool XServiceTCPSourcepoint::start()
{
    this->doAccept();
    return true;
}

void XServiceTCPSourcepoint::doAccept()
{
    this->acceptor.async_accept( 
        [this](std::error_code er, asio::ip::tcp::socket s )
        {
            if(!er)
            {
                LOG(INFO) << "XServiceTCPSourcepoint accept remote:[" << s.remote_endpoint() << "]";


                this->onNewConnect( std::move(s) );
            }
            else
            {
                LOG(ERROR) << "XServiceTCPSourcepoint accept fail! er:[" << er << "]";
            }

            this->doAccept();
        }
    );
}

void XServiceTCPSourcepoint::input(std::shared_ptr<XMessage> msg)
{
    // 将msg中的原始数据复原后发出去。
    //VLOG(5) << "tcp source point service input msg: " << msg->toJson();

    // 不需要处理连接请求. 这个只能是echo返回回来的.
    if( msg->getType() == XMessage::MessageTCPConnect )
    {
        return;
    }
    else if( msg->getType() == XMessage::MessageTCPDisconnect )
    {
        this->removeSession( msg->getSession() );
        return;
    }
    else if( msg->getType() == XMessage::MessageTCPData )
    {
        string sessionId = msg->getSession();
        auto it = this->sessionTable.find( sessionId );
        if( it != this->sessionTable.end() )
        {
            it->second->input( msg );
        }
        else
        {
            LOG(ERROR) << "Can't find session for msg: " << msg->toJson();
        }
    }
    else
    {
        LOG(ERROR) << "Unkown msg! msg: " << msg->toJson();
    }
}

string XServiceTCPSourcepoint::getSessionId( asio::ip::tcp::endpoint endpoint )
{
    stringstream ss;
    ss << endpoint << "-" << time(0);
    return ss.str();
}

void XServiceTCPSourcepoint::onNewConnect( asio::ip::tcp::socket s )
{
    asio::ip::tcp::endpoint remote = s.remote_endpoint();
    string session = this->getSessionId( remote );
    LOG(INFO) << "session [" << session << "] new connect!";

    // 创建一个session，接收、发送 数据
    shared_ptr<SessionStream> sessionStream = std::make_shared<SessionStream>( session, std::move(s), this->shared_from_this(), this->toNode, this->toService );
    sessionStream->start();
    this->sessionTable[session] = sessionStream;

    std::shared_ptr<XMessage> msg = this->createServiceMessage( XMessage::MessageTCPConnect, session, this->toNode, this->toService );
    this->deliverMessage( msg );
}

void XServiceTCPSourcepoint::onDisconnect( string session )
{
    LOG(INFO) << "session [" << session << "] disconnect!";
    std::shared_ptr<XMessage> msg = this->createServiceMessage( XMessage::MessageTCPDisconnect, session, this->toNode, this->toService );
    this->deliverMessage( msg );

    this->removeSession( session );
}

void XServiceTCPSourcepoint::onSessionMessage( shared_ptr<XMessage> msg )
{
    if( XMessage::MessageTCPData == msg->getType() )
    {
        this->onRecvData( msg );
    }
    else if( XMessage::MessageTCPDisconnect == msg->getType() )
    {
        this->onDisconnect( msg->getSession() );
    }
}

void XServiceTCPSourcepoint::onRecvData( shared_ptr<XMessage> msg )
{
    msg->setFromService( this->getId() );
    msg->setToNode( this->toNode );
    msg->setToService( this->toService );

    this->deliverMessage( msg );
}

void XServiceTCPSourcepoint::removeSession( string session )
{
    auto it = this->sessionTable.find( session );
    if( it != this->sessionTable.end() )
    {
        it->second->stop();
    }
    this->sessionTable.erase( session );
}