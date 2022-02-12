#pragma once

#include <asio.hpp>
#include "xpackage.h"
#include <string>
#include "route.h"
#include "xservice.h"
#include <deque>

using namespace std;
    
class SessionStream : public std::enable_shared_from_this<SessionStream>
{
public:
    enum {  MaxReadLen = 1024, /* XPackage::max_body_length / 4,*/ };
public:
    SessionStream( string sessionId, asio::ip::tcp::socket s, std::shared_ptr<XService> service, string toNode, string toService );
    ~SessionStream();

    bool start();

    void stop();

    void input( shared_ptr<XMessage> msg );
protected:
    //
    virtual void doRead();

    virtual void doWrite();

    virtual void onFail( std::error_code ec );

    virtual void onReadData( shared_ptr<XPackage> package );
    
    virtual shared_ptr<XMessage> parseXPackage( shared_ptr<XPackage> package );

protected:
    asio::ip::tcp::socket socket;
private:
    //XPackage readPackage;
    string sessionId;
    string toNode;
    string toService;
    std::shared_ptr<XService> service;

    std::deque< shared_ptr<XMessage> > sendMsgQueue;
};