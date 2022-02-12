#include "xservicetcpchannelserver.h"
#include <glog/logging.h>

XServiceTCPChannelServer::XServiceTCPChannelServer(json cfg, shared_ptr<XNode> node,asio::io_context& context)
    :XService( cfg, node )
    ,asioContext( context )
    ,acceptor( context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), cfg["port"].get<int>() ) )
{
    LOG(INFO) << "tcp channel server on port " << cfg["port"] << " start!";
}

bool XServiceTCPChannelServer::start()
{
    this->doAccept();
    return true;
}

void XServiceTCPChannelServer::input( std::shared_ptr<XMessage> msg )
{
    LOG(ERROR) << "Shouldn't input any msg to tcp channel server! unexpected msg: " << msg->toJson();
}

void XServiceTCPChannelServer::onSessionMessage( shared_ptr<XMessage> msg )
{
    LOG(ERROR) << "Shouldn't onSessionMessage for tcp channel server! unexpected msg: " << msg->toJson();
}


void XServiceTCPChannelServer::doAccept()
{
    this->acceptor.async_accept( 
        [this](std::error_code er, asio::ip::tcp::socket s )
        {
            if(!er)
            {
                LOG(INFO) << "XServiceTCPChannelServer accept client:[" << s.remote_endpoint() << "]";


                this->onNewConnect( std::move(s) );
            }
            else
            {
                LOG(ERROR) << "XServiceTCPChannelServer accept fail! er:[" << er << "]";
            }

            this->doAccept();
        }
    );
}

void XServiceTCPChannelServer::onNewConnect( asio::ip::tcp::socket s )
{
    asio::ip::tcp::endpoint remote = s.remote_endpoint();
    string session = this->getSessionId( remote );
    LOG(INFO) << "session [" << session << "] new connect!";

    // 创建一个session，接收、发送 数据
    json cfg;
    cfg["id"] = session;
    cfg["name"] = session;
    shared_ptr<XServiceTCPChannelSession> sessionService = std::make_shared<XServiceTCPChannelSession>( cfg, this->getXNode(), this->shared_from_this(), std::move(s), this->asioContext );
    sessionService->start();
    this->sessionTable[session] = sessionService;
}

string XServiceTCPChannelServer::getSessionId( asio::ip::tcp::endpoint endpoint )
{
    stringstream ss;
    ss << "tcp_channel_" << endpoint << "-" << time(0);
    return ss.str();
}

void XServiceTCPChannelServer::removeSession(string sessionId)
{
    LOG(INFO) << "tcp channel server remove session: " << sessionId;
    this->sessionTable.erase( sessionId );
}