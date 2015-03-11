#pragma once

/**
*	Э��ͷ�ļ�����.
*	+by lizhixing@2015-02-19
*/

#include <string>
#include <vector>

#include <QXmlStreamWriter>

using namespace std;

// ��������.

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
	vector<string> m_urlList;	// ��ַ�б�(��IP�����), ����ͨ����Щ��ַ��������û�.
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
	// ���ɺͽ���XML.
	void ToXML( QXmlStreamWriter& xml );
	bool FromXml( );
};



