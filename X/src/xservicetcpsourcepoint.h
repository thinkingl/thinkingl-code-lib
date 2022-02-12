#pragma once

#include "xservice.h"
#include <string>
#include <asio.hpp>
#include "route.h"
#include <nlohmann/json.hpp>
#include "xmessage.h"
#include <map>
#include "sessionstream.h"

using namespace std;
using namespace nlohmann;

class XServiceTCPSourcepoint : public XService, public std::enable_shared_from_this<XServiceTCPSourcepoint>
{
public:
    XServiceTCPSourcepoint( json cfg, shared_ptr<XNode> node,asio::io_context& context );

    virtual string getType()
    {
        return ServiceTCPSourcepoint;
    }

    virtual bool start();

    virtual void input( std::shared_ptr<XMessage> );

    virtual void onSessionMessage( shared_ptr<XMessage> msg );
private:
    void doAccept();

    void removeSession( string session );

    void onNewConnect( asio::ip::tcp::socket s );
    void onDisconnect( string session );
    void onRecvData( std::shared_ptr<XMessage> msg );

    string getSessionId( asio::ip::tcp::endpoint endpoint );
private:

    string toNode;
    string toService;

    asio::ip::tcp::acceptor acceptor;
    std::map<string, std::shared_ptr<SessionStream>> sessionTable;
};