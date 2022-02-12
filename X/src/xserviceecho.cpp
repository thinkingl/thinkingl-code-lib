#include "xserviceecho.h"
#include <glog/logging.h>

XServiceEcho::XServiceEcho(json cfg, shared_ptr<XNode> node )
:XService( cfg, node )
{
    LOG(INFO) << "echo service construct, cfg: " << cfg;
}

bool XServiceEcho::start()
{
    LOG(INFO) << "echo service start!";
    return true;
}

void XServiceEcho::input( shared_ptr<XMessage> msg )
{
    auto echoMsg = std::make_shared<XMessage>(*msg);
    echoMsg->setToNode( msg->getFromNode() );
    echoMsg->setToService( msg->getFromService() );
    echoMsg->setFromNode( "" );
    echoMsg->setFromService( this->getId() );
    this->deliverMessage( echoMsg );
}

void XServiceEcho::onSessionMessage( shared_ptr<XMessage> msg )
{

}
