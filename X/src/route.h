#pragma once

#include "xnodeinfo.h"
#include <string>
#include "xpackage.h"
#include <memory>
#include "xmessage.h"

using namespace std;

class Route
{
public:
    void addNode( XNodeInfo nodeInfo );
    void removeNode( string id );

    void deliver( std::shared_ptr<XMessage> message ); 
};