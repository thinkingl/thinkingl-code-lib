#pragma once

/**
 * @brief 一个基本消息
 * 内存改造为动态. @2022-02-13 by thinkingl
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "asio.hpp"
#include <string>
#include "base64.h"
#include <glog/logging.h>
#include <memory>

using namespace std;

struct XPackageHeader
{
private:
    int ver_;
    int type_;
    int bodyLength_;
    int checkCode_;

public:
    XPackageHeader()
    {
        memset(this,0,sizeof(XPackageHeader) );
        this->ver(1);
        this->genCheckCode();
    }

    int ver() const {    return ntohl(this->ver_); }
    void ver( int v ) { 
        this->ver_ = htonl(v);  
        this->genCheckCode();
        }

    int type() const    {   return ntohl(this->type_);  }
    void type( int t )  {   
        this->type_ = htonl(t); 
        this->genCheckCode();
        }

    int bodyLength() const {    return ntohl( this->bodyLength_ );   }
    void bodyLength( int len )  {   
        this->bodyLength_ = htonl( len );
        this->genCheckCode();
        }

    int checkCode() const   {   return ntohl( this->checkCode_ );    }

    bool checkValid() const {   return this->checkCode() == (this->ver() + this->type() + this->bodyLength()); }
    void genCheckCode() {   this->checkCode_ = htonl( this->ver() + this->type() + this->bodyLength() );  }

};

class XPackage
{
public:
    enum { header_length = sizeof(XPackageHeader) };
    //enum { max_body_length = 5*1000*1024 };

    XPackage( const XPackage& another )
    :packageData(0)
    {
        if( this != &another )
        {
            this->packageData = new unsigned char[ header_length + another.bodyLength() ];
            memcpy( this->packageData, another.data(), header_length + another.bodyLength() );
        }
    }

    XPackage( const void* data, size_t len )
    :packageData(0)
    {
        this->body( data, len );
    }

    XPackage( const XPackageHeader& head )
    :packageData(0)
    {
        this->packageData = new unsigned char[ header_length + head.bodyLength() ];
        memset( packageData, 0, header_length + head.bodyLength() );
        *this->pHeader() = head;
    }

    ~XPackage()
    {
        if( this->packageData )
        {
            delete this->packageData;
            this->packageData = 0;
        }
    }

    XPackageHeader header() const
    {
        if(this->packageData)
        {
            return *(XPackageHeader*)this->packageData;
        }
        else
        {
            return XPackageHeader();
        }
    }

    std::size_t bodyLength() const
    {
        if( packageData == 0 )
        {
            return 0;
        }

        return this->pHeader()->bodyLength();
    }

    //void bodyLength( int len )
    //{
    //    this->pHeader()->bodyLength( len );
    //}

    std::size_t totalLength() const
    {
        return header_length + this->bodyLength();
    }

    const void* body() const
    {
        return packageData + header_length;
    }

    void* body()
    {
        return packageData + header_length;
    }

    void body( const void* data, int len )
    {
        auto oldHeader = this->pHeader();
        if( !oldHeader ||  this->bodyLength() < len )
        {
            this->packageData = new unsigned char[header_length + len];
            memset( this->packageData, 0, header_length + len );
            if( oldHeader )
            {
                *this->pHeader() = *oldHeader;
            }
            else
            {
                *this->pHeader() = XPackageHeader();
            }
        }
        this->pHeader()->bodyLength( len );

        if( data && data != this->body() )
        {
            memcpy( this->body(), data, len );
        }
    }

    void body( std::string data )
    {
        unsigned char* ud = (unsigned char*)data.c_str();
        int len = data.length() + 1;
        this->body( ud, len );
    }

    string toBase64() const
    {
        string b64 = base64Encode( this->data(), this->totalLength() );
        return b64;
    }

    static shared_ptr<XPackage> fromBase64( const string& str )
    {
        string data = base64Decode( str );
        if( data.length() >= XPackage::header_length )
        {
            XPackageHeader header = *(XPackageHeader*)(data.data());
            if( !header.checkValid() )
            {
                LOG(ERROR) << "invalid base64 header! str:" << str;
                return nullptr;
            }
            if( header.bodyLength() + XPackage::header_length != data.length() )
            {
                LOG(ERROR) << "invalid base64 length! expect:" << header.bodyLength() + XPackage::header_length << "str len: " << str.length() << " str:" << str;
                return nullptr;
            }
            auto package = make_shared<XPackage>( header );
            package->body( data.data() + XPackage::header_length, header.bodyLength() );
            return package;
        }
    }

    const unsigned char* data()const
    {
        return this->packageData;
    }

    bool checkValid() const
    {
        if( this->packageData )
        {
            return this->pHeader()->checkValid();
        }
        else
        {
            return false;
        }
    }

private:
    XPackageHeader* pHeader() const
    {
        return (XPackageHeader*)this->packageData;
    }
    
private:    
    //unsigned char packageData[header_length + max_body_length];
    unsigned char* packageData;
};