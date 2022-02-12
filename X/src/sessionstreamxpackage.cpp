#include "sessionstreamxpackage.h"
#include <glog/logging.h>

SessionStreamXPackage::SessionStreamXPackage( string sessionId, asio::ip::tcp::socket s, std::shared_ptr<XService> service )
:SessionStream( sessionId, std::move(s), service, "", "" )
{

}

void SessionStreamXPackage::doRead()
{
    this->doReadHeader();
}

void SessionStreamXPackage::doReadHeader()
{
    auto self( this->shared_from_this() );
    auto package = std::make_shared<XPackage>();
    asio::async_read( this->socket,
        asio::buffer((void*)package->data(), XPackage::header_length ),
        [this, self, package](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                doReadBody( package );
            }
            else
            {
                LOG(ERROR) << "xpackage stream socket read fail! ec:[" << ec << "]";
                this->onFail( ec );
            }
        }
    );
}

void SessionStreamXPackage::doReadBody( shared_ptr<XPackage> package )
{
    auto self( this->shared_from_this() );
    asio::async_read( this->socket,
        asio::buffer((void*)package->body(), package->bodyLength() ),
        [this, self, package](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                // 将读到的package解析后送出. 
                this->onReadData( package );

                this->doReadHeader();
            }
            else
            {
                LOG(ERROR) << "xpackage stream socket read fail! ec:[" << ec << "]";
                this->onFail( ec );
            }
        }
    );
}

shared_ptr<XMessage> SessionStreamXPackage::parseXPackage(shared_ptr<XPackage> package )
{
    auto message = XMessage::fromXPackageJsonBody( package );
    return message;
}
