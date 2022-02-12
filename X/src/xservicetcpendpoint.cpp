#include "xservicetcpendpoint.h"
#include <nlohmann/json.hpp>
#include <glog/logging.h>
#include "sessionstream.h"

XServiceTCPEndpoint::XServiceTCPEndpoint(json cfg, shared_ptr<XNode> node, asio::io_context& ctx )
:XService( cfg, node )
,ioContext( ctx )
{
    this->endpointHost = cfg["endpoint"]["host"];
    this->endpointService = cfg["endpoint"]["service"];
}

bool XServiceTCPEndpoint::start()
{
    return true;
}

void XServiceTCPEndpoint::input( shared_ptr<XMessage> message )
{
    if( message->getType() == XMessage::MessageTCPConnect )
    {
        this->doConnect( message );        
    }
    else
    {
        string sessionId = message->getSession();
        auto it = this->sessionsTable.find( sessionId );
        if( it != this->sessionsTable.end() )
        {
            it->second->input( message );
        }
    }
}

void XServiceTCPEndpoint::onSessionMessage( shared_ptr<XMessage> msg )
{
    msg->setFromService( this->getId() );
    this->deliverMessage( msg );

    if( msg->getType() == XMessage::MessageTCPDisconnect )
    {
        this->removeSession( msg->getSession() );
    }
}

void XServiceTCPEndpoint::doConnect( shared_ptr<XMessage> message )
{
    asio::ip::tcp::resolver resolver( ioContext );
    auto endpoints = resolver.resolve( this->endpointHost, this->endpointService );
    //asio::ip::tcp::socket s( this->ioContext );
    auto s = std::make_shared<asio::ip::tcp::socket>( this->ioContext );

    auto self = this->shared_from_this();
    asio::async_connect( *s, endpoints,
        [this,self, message, s]( std::error_code ec, asio::ip::tcp::endpoint )
        {
            string sessionId = message->getSession();
            if (!ec)
            {
                // 
                string toNode = message->getFromNode();
                string toService = message->getFromService();

                LOG(INFO) << "connect for session [" << sessionId << "] success!";

                auto ss = std::make_shared<SessionStream>( sessionId, std::move(*s), this->shared_from_this(), toNode, toService );
                ss->start();

                this->sessionsTable[ sessionId ] = ss;
            }
            else
            {
                LOG(ERROR) << "connect for session [" << sessionId << "] fail! ec:[" << ec << "]";

                auto msg = std::make_shared<XMessage>(XMessage::MessageTCPDisconnect, sessionId, nullptr);
                this->onSessionMessage(msg);
            }
        });

}

void XServiceTCPEndpoint::removeSession(string sessionId)
{
    auto it = this->sessionsTable.find( sessionId );
    if( it != this->sessionsTable.end() )
    {
        it->second->stop();
    }
    this->sessionsTable.erase( sessionId );
}