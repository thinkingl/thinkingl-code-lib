#pragma once

#include "xservice.h"
#include <memory>
#include <nlohmann/json.hpp>
#include "route.h"
#include <asio.hpp>

using namespace nlohmann;

class XServiceFactory
{
public:
    static std::shared_ptr<XService> createService( json serviceCfg, shared_ptr<XNode> node, asio::io_context& ctx );
};