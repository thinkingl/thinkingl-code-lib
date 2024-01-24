#pragma once

#include "xlink.h"
#include "xtransconnection.h"

/**
 * @brief 传输层, 实现数据的可靠传输
 * 
 */
class XTrans
{
public:
    XTrans( );

    virtual void addLink( PXLink xlink );

    virtual PXTransConnection connect( XAddr addr );

    using callback_send = std::function<void(std::error_code ec, size_t len )>;
    virtual void async_send( void* data, size_t len, callback_send cb );


};