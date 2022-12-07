#include "xaddr.h"

XAddr::XAddr()
: port(0)
{

}

XAddr::XAddr( std::string h, int p )
: host(h), port(p)
{}

