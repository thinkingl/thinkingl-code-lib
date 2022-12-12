#include "xlinktest.h"
#include "udplink.h"
#include <glog/logging.h>

XLinkTest::XLinkTest( json cfg, asio::io_context& context, shared_ptr<XNode> node )
    : XService(cfg, node)
    , _context( context )
    , _echoRecv( cfg.at("echo").get<bool>())
    , _localPort( cfg.at( "localPort").get<int>())
    , _remoteAddr( cfg.at( "remoteAddr" ).get<std::string>() )
    , _remotePort( cfg.at( "remotePort" ).get<int>() )
    , _sendData( cfg.at("sendData").get<bool>() )
{
    LOG(INFO) << "construct XLinkTest! cfg:" << cfg;
    this->_xlink = std::make_shared<UDPLink>( this->_context, _localPort );

    this->_xlink->setRecvCB( [this](unsigned char* data, size_t len, std::error_code ec, XAddr remote){
        this->onRecv( data, len, ec, remote );
    } );

    this->_xlink->init();

    if( _sendData )
    {
        // 开启发送定时器.
        auto timer = std::make_shared<asio::steady_timer>( _context );
        asio::post([this, timer ](){
            this->doSendTimer( timer );
        });        
    }

}

void XLinkTest::doSendTimer( std::shared_ptr<asio::steady_timer> timer )
{
    timer->expires_after( std::chrono::seconds(1) );
    timer->async_wait([timer,this](std::error_code ec){
        if( ec )
        {
            LOG(ERROR) << "timer error! ec:[" << ec << "]";
        }
        else
        {
            //LOG(INFO) << "doSendTimer start doSendData";
            // 发送一段数据.
            this->doSendData();

            this->doSendTimer( timer );
        }
    });
}

void XLinkTest::onRecv(unsigned char* data, size_t len, std::error_code ec, XAddr remote )
{
    LOG(INFO) << "onRecv data:[" << data << "] len:[" << len << "] ec:[" << ec << "] remote:[" << remote.host << ":" << remote.port << "]";
    if( this->_echoRecv )
    {
        if( this->_xlink )
        {
            this->_xlink->send( remote, data, len );
        }
        else
        {
            LOG(ERROR) << "xlink is null!";
        }
    }
}

void XLinkTest::doSendData()
{
    if( this->_xlink )
        {
            XAddr addr;
            addr.host = _remoteAddr;
            addr.port = _remotePort;

            auto mtu = _xlink->getMtu();
            auto len = rand() % mtu;
            if( len == 0 )
            {
                len = 1;
            }
            auto data = std::make_shared<std::string>();

            static unsigned s_counter = 0;
            s_counter++;
            while( data->size() < len )
            {
                stringstream ss;
                ss << s_counter << "\t";
                data->append( ss.str() );
            }
            data->resize( len );

            this->_xlink->send( addr, (unsigned char*)data->c_str(), data->length()+1 );
        }
        else
        {
            LOG(ERROR) << "xlink is null!";
        }
}