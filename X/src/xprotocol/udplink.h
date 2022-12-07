#pragma once
#include "xlink.h"
#include "xaddr.h"
#include <string>
#include <asio.hpp>


/**
 * @brief UDP链路
 * 
 */

class UDPLink : public XLink , public  std::enable_shared_from_this<UDPLink>
{
public:
    /**
     * @brief Construct a new UDPLink object
     * 
     * @param localAddr     本地监听地址
     */
    UDPLink( asio::io_context& ctx, int localPort );
    ~UDPLink();

    // 初始化, 开始工作.
    virtual bool init();

    // 是否初始化
    virtual bool isInited();

    // 销毁.
    virtual void destroy();

    // 发送数据.
    virtual void send( XAddr remote, unsigned char* data, size_t len );

    // 获取mtu(Maximum Transmission Unit)
    virtual size_t getMtu();

    // 调试接口.
    virtual std::string debug( std::string cmd );

private:
    // recv
    void doRecv();

private:
    int _localPort;

    asio::io_context& _ctx;

    asio::ip::udp::socket _socket;

    bool _inited;

    const int max_buff_size = 100*1024;
    unsigned char* _recvBuff;
    asio::ip::udp::endpoint _recvEndpoint;
};