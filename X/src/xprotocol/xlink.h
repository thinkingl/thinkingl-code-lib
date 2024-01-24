#pragma once

#include <functional>
#include <system_error>
#include <memory>
#include "xaddr.h"

/**
 * @brief 一个网络链接的接口
 * 网络链接是指能发送/接收数据的链路
 * 对于传输数据是否到达, 是否有效, 延时等等 不做任何保证, 只尽力而为.
 * 这样可以用任意协议或其它方式实现.
 * 可靠传输交给更上层.
 */

class XLink
{
public:
    // 初始化, 开始工作.
    virtual bool init() = 0;

    // 是否初始化
    virtual bool isInited() = 0;

    // 销毁.
    virtual void destroy() = 0;

    // 发送数据.
    virtual void send( XAddr remote, unsigned char* data, size_t len ) = 0;

    // 获取mtu(Maximum Transmission Unit)
    virtual size_t getMtu() = 0;

    // 调试接口.
    virtual std::string debug( std::string cmd ) = 0;

    // 接收数据的回调函数.
    using callback_recv = std::function<void(unsigned char* data, size_t len, std::error_code ec, XAddr remote )>;
    virtual void setRecvCB( callback_recv onRecv );

protected:
    virtual void onRecv( unsigned char* data, size_t len, std::error_code ec, XAddr remote );

private:
    callback_recv onRecvFun;
};

typedef std::shared_ptr<XLink> PXLink;
typedef std::vector<PXLink> XLinkList;