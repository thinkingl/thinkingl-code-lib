#include "xlink.h"

void XLink::setRecvCB( callback_recv onRecv )
{
    this->onRecvFun = onRecv;
}

void XLink::onRecv( unsigned char* data, size_t len, std::error_code ec, XAddr remote )
{
    if( this->onRecvFun )
    {
        this->onRecvFun( data, len, ec, remote );
    }
}