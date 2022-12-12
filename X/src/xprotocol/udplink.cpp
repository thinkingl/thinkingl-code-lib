#include "udplink.h"
#include <glog/logging.h>
#include <asio.hpp>
#include <sstream>

UDPLink::UDPLink( asio::io_context& ctx, int localPort )
    :_localPort( localPort ), _ctx(ctx), _socket( ctx ), _inited( false ), _recvBuff(0)
{

}

UDPLink::~UDPLink()
{
    this->destroy();
}

bool UDPLink::init()
{
    if( this->_inited )
    {
        return true;
    }
    asio::ip::udp::endpoint ep(asio::ip::address_v4::any(), _localPort );
    asio::error_code ec;
    _socket.open( asio::ip::udp::v4() );
    _socket.bind( ep, ec );
    LOG(INFO) << "UDPLink bind to port [" << _localPort << "] ec:[" << ec << "]";

    if( ec )
    {
        LOG(ERROR) << "UDPLink bind to port [" << _localPort << "] fail! ec:[" << ec << "]";
        return false;
    }

    if( _recvBuff == 0 )
    {
        _recvBuff = new unsigned char[max_buff_size];
    }

    this->_inited = true;

    //_socket.async_receive_from()
    this->doRecv();

    return true;
}

void UDPLink::doRecv()
{
    auto self = this->shared_from_this();
    this->_socket.async_receive_from( asio::buffer( this->_recvBuff, max_buff_size), _recvEndpoint, [self](asio::error_code ec,
        std::size_t len ){
            if( ec )
            {
                if( ec.value() == 10061 )
                {
                    // ICMP的对端不可达会返回这个错误, 忽略.
                }
                else
                {
                    LOG(ERROR) << "udplink recv fail! ec:[" << ec << "]";
                }
            }
            else
            {
                // 处理收到的数据.
                LOG(INFO) << "udp link recv data, len:[" << len << "] recvData:[" << (char*)self->_recvBuff << "]";
            }

            

            // 继续收.
            self->doRecv();
        });
}


bool UDPLink::isInited()
{
    return _inited;
}

void UDPLink::destroy()
{
    if( this->_inited )
    {
        this->_socket.close();

        delete[] this->_recvBuff;
        this->_recvBuff = 0;
    }
}

void UDPLink::send( XAddr remote, unsigned char* data, size_t len)
{
    std::stringstream ss;
    ss << remote.port;
    auto remoteEndpoints = asio::ip::udp::resolver( this->_ctx ).resolve( remote.host, ss.str() );
    if( remoteEndpoints.size() > 0 )
    {
        auto remoteEndpoint = remoteEndpoints.begin()->endpoint();
        asio::error_code ec;
        this->_socket.send_to( asio::buffer(data, len), remoteEndpoint, 0, ec );
        if( ec )
        {
            LOG(ERROR) << "send fail! ec:[" << ec << "]";
        }
    }
    else
    {
        LOG(ERROR) << "Can not resolve remote! [" << remote.host << " : " << remote.port << "]";
    }
}

size_t UDPLink::getMtu()
{
    return 1454;
}

std::string UDPLink::debug( std::string cmd )
{

}

