#pragma once

#include <string>
#include <vector>
#include "entrypoint.h"
#include "xchannel.h"
#include <map>
#include "xservicetcpsourcepoint.h"
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include "route.h"
#include <memory>
#include <set>

using namespace std;
using namespace nlohmann;

/**
 * @brief 一个节点
 * 
 */
class XNode : public std::enable_shared_from_this<XNode>
{
    public:

    XNode(asio::io_context& io_context, json cfg, Route& r);

    bool run();

    void deliverMessage( std::shared_ptr<XMessage> msg );
    void deliverRawMessage( shared_ptr<XMessage> msg );

    string getId()const;

    void regRemoteNodeRouteService( string nodeId, std::shared_ptr<XService> service );
    void unregRemoteNodeRouteService( string nodeId, std::shared_ptr<XService> service );

private:
    string id;
    string name;

//    vector<XNode> knownNodes;
//    map<string, vector<XChannel*> > channelTable;

    asio::io_context& asio_io_context;
    json config;
    Route& route;

 
private:
    std::map< string, std::shared_ptr<XService>> servicesTable;

    // remote node id -> local service id.
    std::map<string, std::set<std::shared_ptr<XService>>> remoteNode2LocalServiceTable;
};