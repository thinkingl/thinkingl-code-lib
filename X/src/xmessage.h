#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include "xpackage.h"

using namespace std;
using namespace nlohmann;

class XMessage
{
public:
    static const string MessageTCPConnect;
    static const string MessageTCPDisconnect;
    static const string MessageTCPData;

    static const string MessageChannelHello;
    static const string MessageChannelNodeInfo;
    static const string MessageChannelPayload;

    static const string KeyType;
    static const string KeyToNode;
    static const string KeyToService;
    static const string KeyFromNode;
    static const string KeyFromService;
    static const string KeySession;
    static const string KeyData;
public:
    XMessage();
    XMessage(string type, string session, std::shared_ptr<XPackage> dataPackage );
    XMessage(string type, string fromNode, string fromService, string toNode, string toService, string session, std::shared_ptr<XPackage> dataPackage );

    json toJson() const;

    std::shared_ptr<XPackage> toXPackage() const;


    static std::shared_ptr<XMessage> fromXPackageJsonBody( shared_ptr<XPackage> package );

    string getType() const;
    void setType(string t);

    string getToNode() const;
    void setToNode( string dst );

    string getToService() const;
    void setToService( string dst );

    string getFromNode() const;
    void setFromNode( string n );

    string getFromService() const;
    void setFromService( string s );

    string getSession() const;
    void setSession(string s);

    //string getData() const;
    //void setData( string d );
    void setData( std::shared_ptr<XPackage> package );
    std::shared_ptr<XPackage> getData() const;

private:
    string type;
    string toNode;
    string toService;
    string fromNode;
    string fromService;
    string session;
    shared_ptr<XPackage> data;
};