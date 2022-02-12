#pragma once
#include "xservice.h"
#include <queue>
#include "xservicetcpchannelsession.h"

class XServiceTCPChannelServer : public XService, public std::enable_shared_from_this<XServiceTCPChannelServer>
{
public:
     XServiceTCPChannelServer( json cfg, shared_ptr<XNode> node,asio::io_context& context );

    virtual string getType()
    {
        return ServiceNodeChannelTCPServer;
    }

    virtual bool start();

    virtual void input( std::shared_ptr<XMessage> );

    virtual void onSessionMessage( shared_ptr<XMessage> msg );

    virtual void removeSession( string sessionId );
private:
    void doAccept();
    void onNewConnect( asio::ip::tcp::socket s );

    string getSessionId( asio::ip::tcp::endpoint endpoint );
private:
    shared_ptr<XNode> xnode;
    asio::io_context& asioContext;
    asio::ip::tcp::acceptor acceptor;
    std::map<string, std::shared_ptr<XServiceTCPChannelSession>> sessionTable;

};