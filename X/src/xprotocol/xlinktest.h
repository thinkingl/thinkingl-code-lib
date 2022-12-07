#pragma once

#include "xlink.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <asio.hpp>
#include "../xservice.h"

using namespace nlohmann;

class XLinkTest : public XService
{
public:
    //XLinkTest( int localPort, std::string remoteAddr, int remotePort, bool echo, bool sendData );
    XLinkTest( json cfg, asio::io_context& context, shared_ptr<XNode> node );

    void onRecv(unsigned char* data, size_t len, std::error_code ec, XAddr remote );

    void doSendTimer( std::shared_ptr<asio::steady_timer> timer );
    void doSendData();

    virtual string getType(){ return "xlinktest"; };

    virtual bool start(){ return true; };

    virtual void input( std::shared_ptr<XMessage> msg ){};
    virtual void onSessionMessage( std::shared_ptr<XMessage> msg ){};
private:
    std::shared_ptr<XLink> _xlink;

    bool _echoRecv;
    int _localPort;
    std::string _remoteAddr;
    int _remotePort;
    bool _sendData;

    asio::io_context& _context;
};