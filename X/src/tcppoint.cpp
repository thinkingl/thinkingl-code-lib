#include "tcppoint.h"
#include <sstream>
#include <glog/logging.h>

using namespace std;
using namespace nlohmann;


string TCPPoint::name()
{
    stringstream ss;
    ss << "TCPPoint-" << this;
    return ss.str();
}

TCPPoint::TCPPoint( EndPointType type, asio::ip::tcp::socket s, json cfg )
:type_(type)
,socket(std::move(s))
{
    LOG(INFO) << "construct of TCPPoint! this:[" << this << "]";
}

TCPPoint::~TCPPoint()
{

}

void TCPPoint::start()
{
    LOG(INFO) << this->name() << " will start!";
    this->doRead();
}

void TCPPoint::stop()
{
    LOG(INFO) << this->name() << " will stop!";
    this->socket.close();

}

void TCPPoint::pushForward( SPData data )
{
    if( Source == this->type_ )
    {
        LOG(ERROR) << this->name() << " is Source! should not pushDataForward!!";
        return;
    }
    else
    {
        if( !data )
        {
            LOG(ERROR) << this->name() << " pushForward null!";
            return;
        }
        // 通过socket发送出去.
        this->write( data );
    }
}

void TCPPoint::pushBackward( SPData data )
{
    if( Sink == this->type_ )
    {
        LOG(ERROR) << this->name() << " is Sink! should not pushDataBackward!!";
        return;
    }
    else
    {
        if( !data )
        {
            LOG(ERROR) << this->name() << " pushBackward null!";
            return;
        }
        // 通过socket发送出去.
        this->write( data );
    }
}

void TCPPoint::write( SPData data )
{
    bool needDoWrite = this->sendQueue.empty();
    this->sendQueue.push( data );
    if( needDoWrite )
    {
        this->doWrite();
    }
}

void TCPPoint::doWrite()
{
    if( !this->sendQueue.empty() )
    {
        SPData data = this->sendQueue.front();

        auto self(shared_from_this());
        asio::async_write( this->socket,
            asio::buffer( data->data(), data->size() ),
            [this, self, data](std::error_code ec, std::size_t /*length*/)
            {
                this->sendQueue.pop();

                if (!ec)
                {
                    if( !this->sendQueue.empty() )
                    {
                        this->doWrite();
                    }
                }
                else
                {
                    LOG(ERROR) << "doWrite fail! ec:[" << ec << "] name:[" << this->name() << "]";
                    this->onFail( ec, "doWrite fail!" );
                }
        });
    }
}

void TCPPoint::onError(SPError err )
{

}

void TCPPoint::doRead()
{
    auto self(this->shared_from_this());

    auto data = make_shared<Data>(DefaultBufSize);
    asio::async_read(this->socket,
        asio::buffer( data->data(), data->size() ),
        [this,self,data](asio::error_code ec, size_t len ){
            if (!ec)
            {
                if( Source == this->type_ )
                {
                    self->pushForward( data );
                }
                else
                {
                    self->pushBackward( data );
                }
                
                doRead();
            }
            else
            {
                LOG(ERROR) << "doRead fail! ec:[" << ec << "]";

                this->onFail( ec, "doRead fail!" );
            }
        });
}

void TCPPoint::onFail(asio::error_code ec, string descriptionPrefix )
{
    auto err = std::make_shared<Error>( );
    err->errorCode = ec.value();
    err->description = descriptionPrefix + string(" message: ") + ec.message();
    err->errorElement = this->shared_from_this();
    err->previousElement = this->shared_from_this();
    err->direction = this->type_ == Source ? Forward : Backward;    // 数据源, 错误向后传. 数据终点,错误向前传.
    this->sendErrorNext( err );
}