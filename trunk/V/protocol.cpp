#include "protocol.h"


int CHelloReq::GetVersion() const
{
	return m_version;
}

CHelloReq::CHelloReq()
	:m_version(PROTOCOL_CUR_VERSION)
{
}

void CHelloReq::SetVersion( int version )
{
	m_version = version;
}

string& CHelloReq::Id()
{
	return m_id;
}

const string& CHelloReq::Id() const
{
	return m_id;
}

string& CHelloReq::Email()
{
	return m_email;
}

const string& CHelloReq::Email() const
{
	return m_email;
}

string& CHelloReq::Name()
{
	return m_name;
}

const string& CHelloReq::Name() const
{
	return m_name;
}

vector<string>& CHelloReq::UrlList()
{
	return m_urlList;
}

const vector<string>& CHelloReq::UrlList() const
{
	return m_urlList;
}

void CHelloReq::ToXML( QXmlStreamWriter& xml )
{
	xml.writeStartDocument();

	xml.writeStartElement( "version" );
	xml.writeCharacters( QString::number( this->GetVersion() ) );
	xml.writeEndElement();

	xml.writeStartElement( "id" );
	xml.writeCharacters( this->Id().c_str() );
	xml.writeEndElement();

	xml.writeStartElement( "email" );
	xml.writeCharacters( this->Email().c_str() );
	xml.writeEndElement();

	xml.writeStartElement( "name" );
	xml.writeCharacters( this->Name().c_str() );
	xml.writeEndElement();

	for ( size_t i=0; i<this->UrlList().size(); ++i )
	{
		xml.writeStartElement( "url" );
		xml.writeCharacters( this->UrlList()[i].c_str() );
		xml.writeEndElement();
	}

}
