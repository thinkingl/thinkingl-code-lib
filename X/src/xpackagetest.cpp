#include "gtest/gtest.h"
#include "xpackage.h"
#include <string>

using namespace std;

TEST( XPackage, construct )
{
    XPackage package;
    EXPECT_EQ(0, package.bodyLength() );
    EXPECT_TRUE(package.body() );

    package;
}

TEST( XPackage, body )
{
    XPackage package;

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