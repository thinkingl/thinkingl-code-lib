#include "xmessage.h"

const string XMessage::MessageTCPConnect = "tcp_connect";
const string XMessage::MessageTCPDisconnect = "tcp_disconnect";
const string XMessage::MessageTCPData = "tcp_data";

const string XMessage::MessageChannelHello = "chn_hello";
const string XMessage::MessageChannelNodeInfo = "chn_node_info";
const string XMessage::MessageChannelPayload = "chn_payload";

const string XMessage::KeyType = "type";
const string XMessage::KeyToNode = "to_node";
const string XMessage::KeyToService = "to_service";
const string XMessage::KeyFromNode = "from_node";
const string XMessage::KeyFromService = "fromService";
const string XMessage::KeySession = "session";
const string XMessage::KeyData = "data";

XMessage::XMessage()
{    
}
XMessage::XMessage(string type, string fromNode, string fromService, string toNode, string toService, string session, std::shared_ptr<XPackage> dataPackage )
{
    this->type = type;
    this->toNode = toNode;
    this->toService = toService;
    this->fromNode = fromNode;
    this->fromService = fromService;
    this->session = session;
    this->data = dataPackage;
}

XMessage::XMessage(string type, string session, shared_ptr<XPackage> package )
{
    this->type = type;
    this->session = session;
    this->data = package;
}

json XMessage::toJson() const
{
    json j;
    j[ KeyType ] = this->type;
    j[ KeyToNode ] = this->toNode;
    j[ KeyToService ] = this->toService;
    j[ KeySession ] = this->session;
    j[ KeyFromNode ] = this->fromNode;
    j[ KeyFromService ] = this->fromService;
    if( this->data )
    {
        j[KeyData] = this->data->toBase64();
    }
    return j;
}

std::shared_ptr<XPackage> XMessage::toXPackage() const
{
    json j = this->toJson();
    string s = j.dump();
    auto package = std::make_shared<XPackage>( s.c_str(), s.length()+1 );
    return package;
}

shared_ptr<XMessage> XMessage::fromXPackageJsonBody(shared_ptr<XPackage> package )
{
    string strJson = (char*)package->body();
    LOG_FIRST_N(INFO,100) << "fromXPackageJsonBody strJson:[" << strJson << "]";  
    json j = json::parse(strJson);

    auto msg = std::make_shared<XMessage>( j[KeyType], j[KeyFromNode], j[KeyFromService], j[KeyToNode], j[KeyToService], j[KeySession], nullptr );

    if( j[KeyData].is_string() )
    {
        string strB64 = j[KeyData];
        //std::cout << "decode result: " << data << std::endl;
        auto package = XPackage::fromBase64( strB64 );
        msg->setData( package );
    }
    return msg;
}

shared_ptr<XPackage> XMessage::getData() const
{
    return this->data;
}

void XMessage::setData( shared_ptr<XPackage> package )
{
    this->data = package;
}

string XMessage::getType() const
{
    return this->type;
}

void XMessage::setType( string t )
{
    this->type = t;
}

string XMessage::getFromNode() const
{
    return this->fromNode;
}

void XMessage::setFromNode( string node )
{
    this->fromNode = node;
}

string XMessage::getFromService() const
{
    return this->fromService;
}

void XMessage::setFromService( string from )
{
    this->fromService = from;
}

string XMessage::getToNode() const 
{
    return this->toNode;
}

void XMessage::setToNode( string to )
{
    this->toNode = to;
}

string XMessage::getToService() const
{
    return this->toService;
}

void XMessage::setToService( string to )
{
    this->toService = to;
}

string XMessage::getSession() const
{
    return this->session;
}

void XMessage::setSession( string s )
{
    this->session = s;
}

