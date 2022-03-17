#include "xservicefactory.h"
#include <glog/logging.h>
#include "xservicetcpsourcepoint.h"
#include "xservicetcpendpoint.h"
#include "xserviceecho.h"
#include "xservicetcpchannelclient.h"
#include "xservicetcpchannelserver.h"

std::shared_ptr<XService> XServiceFactory::createService( json serviceConfig, shared_ptr<XNode> node, asio::io_context& ctx)
{
    std::shared_ptr<XService> service;
    string type = serviceConfig["type"];
    string name = serviceConfig["name"];
    LOG(INFO) << "will create xservice, type: " << type << " name:" << name;
    if( XService::ServiceTCPSourcepoint == type )
    {
        service = std::make_shared<XServiceTCPSourcepoint>( serviceConfig, node, ctx );
    }
    else if( XService::ServiceTCPEndpoint == type )
    {
        service = std::make_shared<XServiceTCPEndpoint>(serviceConfig, node, ctx );
    }
    else if( XService::ServiceEcho == type )
    {
        service = std::make_shared<XServiceEcho>( serviceConfig, node );
    }
    else if( XService::ServiceNodeChannelTCPClient == type )
    {
        service = std::make_shared<XServiceTCPChannelClient>(serviceConfig, node, ctx );
    }
    else if( XService::ServiceNodeChannelTCPServer == type )
    {
        service = std::make_shared<XServiceTCPChannelServer>(serviceConfig, node, ctx );
        LOG(INFO) << "make share tcp channel server " << service;
    }
    else    
    {
        LOG(ERROR) << "Unkown service type:[" << type << "] service cfg: " << serviceConfig.dump();
    }

    return service;
}