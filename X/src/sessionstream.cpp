#include "sessionstream.h"
#include "xpackage.h"
#include <glog/logging.h>
#include <asio.hpp>

SessionStream::SessionStream(string id, asio::ip::tcp::socket s, shared_ptr<XService> service, string toN, string toS )
:socket( std::move(s) )
,sessionId(id)
,service( service )
,toNode( toN )
,toService( toS )
{

}

SessionStream::~SessionStream()
{
    LOG(INFO) << "deconstruct of session [" << this->sessionId << "] of service [" << this->service->getId() << "]";
    this->stop();
    this->service = 0;
}

bool SessionStream::start()
{
    this->doRead();
    return true;
}

void SessionStream::stop()
{
    this->socket.close();
    //asio::post([this](){ this->socket.close(); });
}

void SessionStream::doRead()
{
    auto self( this->shared_from_this() );
    auto package = std::make_shared<XPackage>(nullptr,MaxReadLen);
    this->socket.async_read_some(
        asio::buffer((void*)package->body(), MaxReadLen ),
        [this, self, package](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                if( this->toService == XService::ServiceEcho )
                {
                    LOG(INFO) << "read echo in:" << (char*)package->body() << " read len: " << length;
                }
                auto bodyPackage = std::make_shared<XPackage>(package->body(), length);
 
                //package->bodyLength( length );

                if( this->toService == XService::ServiceEcho )
                {
                    LOG(INFO) << "read echo after bodyLength:" << (char*)bodyPackage->body() << " body len: " << bodyPackage->bodyLength();
                }
                //LOG(INFO) << "session [" << this->sessionId << "] recv [" << package->bodyLength() << "]-[" << package->body() << "]";
                //VLOG(5) << "session [" << this->sessionId << "] recv datalen:[" << package->bodyLength() << "]";
                this->onReadData( bodyPackage );
                
                doRead();
            }
            else
            {
                LOG(ERROR) << "socket read fail! ec:[" << ec << "]";
                
                this->onFail( ec );
            }
        }
    );
    
}

void SessionStream::onReadData( shared_ptr<XPackage> package )
{
    auto msg = this->parseXPackage( package );
    if( msg )
    {
        if( msg->getToService() == XService::ServiceEcho || msg->getFromService() == XService::ServiceEcho )
        {
            LOG(INFO) << "session stream read echo msg: " << msg->toJson();
        }
        
        this->service->deliverMessage( msg );
    }
    else
    {
        LOG(ERROR) << "parse xpackage fail! package: [" << package->bodyLength() << "] - [" << package->body() << "]";
    }
}

shared_ptr<XMessage> SessionStream::parseXPackage( shared_ptr<XPackage> package )
{
    return std::make_shared<XMessage>( XMessage::MessageTCPData, "", "", this->toNode, this->toService, this->sessionId, package );
}

void SessionStream::input( shared_ptr<XMessage> msg )
{
    bool sending = !this->sendMsgQueue.empty();
    this->sendMsgQueue.push_back( msg );
    if( !sending )
    {
        this->doWrite();
    }
}

void SessionStream::doWrite()
{
    auto msg = this->sendMsgQueue.front();
    
    auto package = msg->getData();

    if( msg->getToService() == XService::ServiceEcho || msg->getFromService() == XService::ServiceEcho )
    {
        LOG(INFO) << "session stream send echo msg: " << msg->toJson() << " dataLen: " << package->bodyLength() << " data: " << (char*)package->body();
    }

    auto self(shared_from_this());
    asio::async_write( this->socket,
        asio::buffer( package->body(), package->bodyLength() ),
        [this, self, package](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            this->sendMsgQueue.pop_front();
            if (!sendMsgQueue.empty())
            {
              doWrite();
            }
          }
          else
          {
              LOG(ERROR) << "doWrite fail! ec:[" << ec << "] session:[" << this->sessionId << "]";
              this->onFail( ec );
          }
    });

}

void SessionStream::onFail( std::error_code ec )
{
    LOG(ERROR) << "session " << this->sessionId << " of service " << this->service->getId() << " fail! ec: " << ec;
    this->service->onSessionMessage( std::make_shared<XMessage>( XMessage::MessageTCPDisconnect, "", "", this->toNode, this->toService, this->sessionId, nullptr ) );
    this->stop();
}