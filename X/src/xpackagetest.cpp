#include "gtest/gtest.h"
#include "xpackage.h"
#include <string>
#include <iostream>

using namespace std;

TEST( XPackage, construct )
{
    XPackage package(0,0);
    EXPECT_EQ(0, package.bodyLength() );
    EXPECT_TRUE(package.body() );

    package;

    shared_ptr<XPackage> ptr;
    ptr = nullptr;

    auto ptr2 = make_shared<XPackage>(nullptr,0);
    EXPECT_EQ( 0, ptr2->bodyLength() );
    cout << "ptr2 bodyLength:" << ptr2->bodyLength() << endl;
}

TEST( XPackage, body )
{
    XPackage package(0,0);

    string testTxt = "testaabbcc1234中文哈哈";
    package.body( testTxt );
    EXPECT_EQ( testTxt.length() + 1, package.bodyLength() );
    EXPECT_EQ( testTxt, (char*)package.body() );

    unsigned char * data = (unsigned char* )testTxt.c_str();
    int len = testTxt.length();
    package.body( data, len );
    EXPECT_EQ( len, package.bodyLength() );
    EXPECT_EQ( testTxt, (char*)package.body() );
}

TEST( XPackage, header )
{
    string testTxt = "testaabbcc1234中文哈哈";
    XPackageHeader header;
    header.bodyLength( testTxt.length() +1 );
    auto ptr = make_shared<XPackage>(header);

    EXPECT_EQ( testTxt.length() + 1, ptr->bodyLength() );

    EXPECT_TRUE( ptr->checkValid() );

    //*((int*)ptr->data()) = htonl( testTxt.length() +1 );

    //EXPECT_EQ( testTxt.length() +1, ptr->bodyLength() );

    //ptr->bodyLength( ptr->bodyLength() );

    ptr->body( testTxt.c_str(), testTxt.length() + 1 );

    EXPECT_EQ( testTxt.length() + 1, ptr->bodyLength() );
    EXPECT_EQ( testTxt, (char*)ptr->body() );

    *((int*)ptr->data()) = htonl( testTxt.length() +1 );
    EXPECT_FALSE( ptr->checkValid() );
}

TEST( XPackage, base64 )
{
    string b64 = "AAAAAQAAAAAAAACoAAAAqXsiZnJvbVNlcnZpY2UiOiJlbnRyeV9mb3Jfc3NoQGFwdSIsImZyb21fbm9kZSI6IjIzNEBhbGl5dW4iLCJzZXNzaW9uIjoiMTI3LjAuMC4xOjU3MTY0LTE2NDQ3NzczMTkiLCJ0b19ub2RlIjoiYXB1QGhvbWUiLCJ0b19zZXJ2aWNlIjoi";
    auto package = XPackage::fromBase64( b64 );
}