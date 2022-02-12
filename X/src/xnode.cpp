#include "xnode.h"
#include "xservicefactory.h"
#include <glog/logging.h>

XNode::XNode(asio::io_context& io_context, json cfg, Route& r)
    :asio_io_context(io_context)
    ,config(cfg)
    ,route(r)
{
    this->id = cfg["id"];

}

bool XNode::run()
{
    LOG(INFO) << "xnode run! cfg:" << this->config.dump(4);

    for(auto& serviceCfg : config["services"] )
    {
        std::shared_ptr<XService> service = XServiceFactory::createService( serviceCfg, this->shared_from_this(), asio_io_context );
        if( service )
        {
            LOG(INFO) << "create service, id: " << serviceCfg["id"];
            service->start();
            this->servicesTable[service->getId()] = service;
        }
        else
        {
            LOG(ERROR) << "Create service fail! service cfg:" << serviceCfg;
        }
    }

    //asio::ip::tcp::acceptor acceptor()
    //asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 2284);

    //tcpSourceAPUSSH = new XServiceTCPSourcepoint( "apussh_src", "apussh source point", "apu", "apussh_dst", route, this->asio_io_context, endpoint );
    //bool bOk = tcpSourceAPUSSH->start();
    //LOG(INFO) << "start tcp source apu ssh, result:[" << bOk << "]";
    //return bOk;

    return false;
}

void XNode::deliverMessage( shared_ptr<XMessage> msg )
{
    msg->setFromNode( this->id );
    this->deliverRawMessage( msg );
}

void XNode::deliverRawMessage( shared_ptr<XMessage> msg )
{

    VLOG(5) << "node will deliver msg: " << msg->toJson();

    string toNode = msg->getToNode();
    string toService = msg->getToService();
    if( toNode == this->id )
    {
        // 自己的消息
        auto it = this->servicesTable.find( toService );
        if( it != this->servicesTable.end() )
        {
            it->second->input( msg );
        }
        else
        {
            
            LOG(ERROR) << "Can't find to service [" << toService << "] for msg: " << msg->toJson();
        }
    }
    else
    {
        auto itChn = this->remoteNode2LocalServiceTable.find( toNode );
        if( itChn != this->remoteNode2LocalServiceTable.end() )
        {
            // 随便选一个, 第一个.
            if( !itChn->second.empty() )
            {
                auto itService = itChn->second.begin();
                (*itService)->input( msg );
                return;
            }
        }

        LOG(ERROR) << "Can't find to service [" << toService << "] for remote msg: " << msg->toJson();
    }
}

string XNode::getId() const
{
    return this->id;
}

void XNode::regRemoteNodeRouteService(string nodeId, shared_ptr<XService> service )
{
    LOG(INFO) << "reg remote node " << nodeId << " to " << service->getId() << " - " << service->getName();
    this->remoteNode2LocalServiceTable[nodeId].insert( service );
}

void XNode::unregRemoteNodeRouteService(string nodeId, shared_ptr<XService> service )
{
    LOG(INFO) << "unreg remote node " << nodeId << " to " << service->getId() << " - " << service->getName();
    this->remoteNode2LocalServiceTable[nodeId].erase( service );
    if( this->remoteNode2LocalServiceTable[nodeId].empty() )
    {
        this->remoteNode2LocalServiceTable.erase( nodeId );
    }
}