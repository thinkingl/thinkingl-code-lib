#pragma once
#include <memory>

class XTransConnection
{
public:
    XTransConnection(XLinkList xlinkList );
    
};

typedef std::shared_ptr<XTransConnection> PXTransConnection;