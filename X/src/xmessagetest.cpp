#include "xmessage.h"
#include "xpackage.h"
#include <gtest/gtest.h>

TEST( XMessage, transXPackage )
{
    // XMessage(string type, string fromNode, string fromService, string toNode, string toService, string session, std::shared_ptr<XPackage> dataPackage );
    // 
    string testData = "abc123!#$%中文谢谢";
    auto package = make_shared<XPackage>( testData.c_str(), testData.length() +1 );
    auto msg = make_shared<XMessage>( XMessage::MessageTCPData, "fromNode", "fromService", "toNdoe", "toService", "sessionId", package );
  
    auto packageJson = msg->toXPackage();

    std::cout << "package body: " << (char*)packageJson->body() << std::endl;

    auto msgRestore = XMessage::fromXPackageJsonBody( packageJson );

    auto packageRestore = msgRestore->getData();
    std::cout << "msgRestore data: " << packageRestore->bodyLength() << " - " << (char*)(packageRestore->body()) << std::endl;

    EXPECT_EQ( msg->toJson().dump(), msgRestore->toJson().dump() );

    auto msg2 = make_shared<XMessage>( XMessage::MessageTCPData, "fromNode", "fromService", "toNdoe", "toService", "sessionId", nullptr );
    EXPECT_FALSE( msg2->getData() );


}

TEST( XMessage, bigText )
{
    vector<unsigned char> testData;
    for( int i=0; i< 1000*1024; ++i )
    {
        testData.push_back( rand() );
    }

    auto package = make_shared<XPackage>( testData.data(), testData.size() );

    auto msg = make_shared<XMessage>( XMessage::MessageTCPData, "fromNode", "fromService", "toNdoe", "toService", "sessionId", package );
  
    auto packageJson = msg->toXPackage();
    auto msgRestore = XMessage::fromXPackageJsonBody( packageJson );

    EXPECT_EQ( msg->toJson().dump(), msgRestore->toJson().dump() );


    auto packageRestore = msgRestore->getData();

    EXPECT_EQ( testData.size(), packageRestore->bodyLength() );

    vector<unsigned char> restoreData;
    for( int i=0; i<packageRestore->bodyLength(); ++i )
    {
        restoreData.push_back( ((unsigned char*)packageRestore->body())[i] );
    }
    EXPECT_EQ( testData, restoreData );


}

