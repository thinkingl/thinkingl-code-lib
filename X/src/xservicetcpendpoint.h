#pragma once

#include "xservice.h"
#include <string>
#include <asio.hpp>
#include "sessionstream.h"
#include <map>

using namespace std;

class XServiceTCPEndpoint : public XService, public std::enable_shared_from_this<XServiceTCPEndpoint>
{
public:

    XServiceTCPEndpoint( json cfg, shared_ptr<XNode> node, asio::io_context& context);

    virtual string getType()
    {
        return XService::ServiceTCPEndpoint;
    }

    virtual bool start();

    virtual void input( std::shared_ptr<XMessage> );

    virtual void onSessionMessage( shared_ptr<XMessage> msg );

private:
    void doConnect( shared_ptr<XMessage> msg );

    void removeSession( string sessionId );

private:
    asio::io_context& ioContext;

    string endpointHost;
    string endpointService;

    std::map<std::string, std::shared_ptr<SessionStream>> sessionsTable;
};