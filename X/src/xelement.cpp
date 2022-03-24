#include "xelement.h"
#include <glog/logging.h>

void XElement::addElement( Direction direction, SPXElement element )
{
    if( !element )
    {
        LOG(ERROR) << "element is null!";
        return;
    }
    LOG(INFO) << "addelement [" << element->name() << "] direction:[" << direction << "]";

    auto* elements = &this->sources;
    if( Forward == direction )
    {
        elements = &this->sinks;
    }
    elements->insert( element );
}

void XElement::removeElement( Direction direction, SPXElement element )
{
    if( !element )
    {
        LOG(ERROR) << "element is null!";
        return;
    }
    LOG(INFO) << "removeElement [" << element->name() << "] direction:[" << direction << "]";

    auto* elements = &this->sources;
    if( Forward == direction )
    {
        elements = &this->sinks;
    }
    elements->erase( element );
}

void XElement::sendDataForwardNext( SPData data )
{
    for( auto s : this->sinks )
    {
        s->pushForward( data );
    }
}

void XElement::sendDataBackwardNext( SPData data )
{
    for( auto s : this->sources )
    {
        s->pushBackward( data );
    }
}

void XElement::sendErrorNext(SPError err )
{
    if( !err )
    {
        LOG(ERROR) << "error is null!";
        return;
    }

    auto* elements = &this->sources;
    if( Forward == err->direction )
    {
        elements = &this->sinks;
    }

    err->previousElement = this->shared_from_this();
    for( auto e : *elements )
    {
        e->onError( err );
    }
}