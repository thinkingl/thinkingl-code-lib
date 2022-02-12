#pragma once

#include "xservice.h"

class XServiceEcho : public XService, public std::enable_shared_from_this<XServiceEcho>
{
public:
    XServiceEcho( json cfg, shared_ptr<XNode> node );

    virtual string getType()
    {
        return ServiceEcho;
    }
    virtual bool start();

    virtual void input( std::shared_ptr<XMessage> );

    virtual void onSessionMessage( std::shared_ptr<XMessage> );
private:

};