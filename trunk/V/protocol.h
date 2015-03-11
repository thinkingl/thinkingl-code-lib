#pragma once

/**
*	协议头文件定义.
*	+by lizhixing@2015-02-19
*/

#include <string>
#include <vector>

#include <QXmlStreamWriter>

using namespace std;

// 紧密排列.

const int PROTOCOL_CUR_VERSION = 1;
const int ENC_KEY_LEN = 128;
// Hello.
class CHelloReq
{
private:
	int m_version;
	string m_id;
	string m_email;
	string m_name;
	vector<string> m_urlList;	// 地址列表(多IP的情况), 可以通过这些地址连接这个用户.
	//char zEncKey[ENC_KEY_LEN];
public:
	CHelloReq();

	int GetVersion()const;
	void SetVersion( int version );

	string& Id();
	const string& Id() const;

	string& Email();
	const string& Email() const;

	string& Name();
	const string& Name() const;

	vector<string>& UrlList();
	const vector<string>& UrlList() const;
public:
	// 生成和解析XML.
	void ToXML( QXmlStreamWriter& xml );
	bool FromXml( );
};



