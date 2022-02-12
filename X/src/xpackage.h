#pragma once

/**
 * @brief 一个基本消息
 * 
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "asio.hpp"
#include <string>
#include "base64.h"
#include <glog/logging.h>

using namespace std;

class XPackage
{
public:
    enum { header_length = 4 };
    enum { max_body_length = 1000*1024 };

    XPackage()
    {
        memset(packageData,0, sizeof(packageData));
    }

    XPackage( const void* data, size_t len )
    {
        memset(packageData,0, sizeof(packageData));
        this->body( data, len );
    }

    std::size_t bodyLength() const
    {
        int bodyLen = ntohl( *(int*)packageData );
        if( bodyLen > max_body_length )
        {
            LOG(ERROR) << "Out of bound of XPackage!!";
        }
        return bodyLen;
    }

    void bodyLength( int len )
    {
        this->body( this->body(), len );
    }

    const void* body() const
    {
        return packageData + header_length;
    }

    void body( const void* data, int len )
    {
        if ( len > max_body_length )
        {
            //throw EXCEPTION_ARRAY_BOUNDS_EXCEEDED;
            throw "body is out of bounds!";
            return;
        }
        int netLen = htonl( len );
        memcpy( packageData, &netLen, sizeof(netLen) );
        if( data != packageData + header_length )
        {
            memcpy( packageData + header_length, data, len );
        }
    }

    void body( std::string data )
    {
        unsigned char* ud = (unsigned char*)data.c_str();
        int len = data.length() + 1;
        this->body( ud, len );
    }

    string encodeBody() const
    {
        // todo
        string b64 = base64Encode( (const unsigned char*)this->body(), this->bodyLength() );
        return b64;
    }

    const unsigned char* data()const
    {
        return this->packageData;
    }

private:    
    unsigned char packageData[header_length + max_body_length];

};