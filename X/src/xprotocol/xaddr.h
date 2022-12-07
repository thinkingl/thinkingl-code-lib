#pragma once
#include <string>


class XAddr
{
public:
    XAddr();
    XAddr( std::string h, int p );

    std::string host;
    int port;
};