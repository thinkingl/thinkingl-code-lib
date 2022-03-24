#pragma once

#include "xelement.h"
#include <asio.hpp>
#include <queue>
#include <string>
#include <nlohmann/json.hpp>

class TCPPoint : public XElement
{
public:
    enum EndPointType
    {
        Source,    // 收到的数据发给下一个sink
        Sink       // 收到的数据发给前一个source.
    };

    enum 
    {
        DefaultBufSize = 2*1024*1024,
    };

    TCPPoint( EndPointType type, asio::ip::tcp::socket s, nlohmann::json cfg );
    ~TCPPoint();

    virtual std::string name();

    virtual void start();
    virtual void stop();

    // 双向的数据输入.
    virtual void pushForward( SPData data );
    virtual void pushBackward( SPData data );

    // 错误
    virtual void onError( SPError err );

private:
    void write( SPData data );

    void doWrite();
    void doRead();

    void onFail( asio::error_code ec, std::string descriptionPrefix );

private:
    EndPointType type_;
    asio::ip::tcp::socket socket;

    std::queue<SPData> sendQueue;
};