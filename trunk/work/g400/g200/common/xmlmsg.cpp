#include "xmlmsg.h"

CXmlMsg::CXmlMsg(LPSTR pszXml)
{
    m_cMarkUp.SetDoc(pszXml);
}
s32 CXmlMsg::GetCommand()
{
    m_cMarkUp.ResetMainPos();
    m_cMarkUp.IntoElem();
    
    const s8 *pszCmd = m_cMarkUp.GetData();
    return atoi(pszCmd);
}
void CXmlMsg::SetCommand(s32 cmd)
{   
    m_cMarkUp.ResetMainPos();
	sprintf(m_achCmd,"%d",cmd);
    m_cMarkUp.AddElem("command",m_achCmd);
    m_cMarkUp.AddElem("content");
}

void CXmlMsg::Go2Content()
{
    m_cMarkUp.ResetMainPos();
    m_cMarkUp.IntoElem();
    m_cMarkUp.NextSibling();
}

BOOL32 CXmlMsg::GoUpstairs()
{
    return m_cMarkUp.OutOfElem();
}

BOOL32 CXmlMsg::GoDownstairs()
{
    return m_cMarkUp.IntoElem();
}

BOOL32 CXmlMsg::NextSibling()
{
    return m_cMarkUp.NextSibling();
}

/*
    <parent>...<node></node></parent>
*/
void CXmlMsg::BuildNode(LPCSTR pszNodeName)
{
    m_cMarkUp.AddElem(pszNodeName);
}
/*
    <node>value</node>
*/
void CXmlMsg::BuildNode(LPCSTR pszNodeName,LPCSTR szFormat,...)
{  
    va_list pvList;
    va_start(pvList,szFormat);
    vsprintf(m_achValue,szFormat,pvList);
    va_end(pvList);
    m_cMarkUp.AddElem(pszNodeName,m_achValue);
}

s32 CXmlMsg::GetNodeIntValue(const s8* strEle)
{
    if (strEle == NULL)
	{
		return m_cMarkUp.GetDataInt();
	}
	else
	{
		if(m_cMarkUp.FindElem(strEle))
			return m_cMarkUp.GetDataInt();
	}
    return 0;
}
LPCSTR CXmlMsg::GetNodeStringValue(const s8* strEle)
{
    if (strEle == NULL)
	{
		return m_cMarkUp.GetData();
	}
	else
	{
		if(m_cMarkUp.FindElem(strEle))
			return m_cMarkUp.GetData();
	}
	return 0;
}

LPCSTR CXmlMsg::GetMsgStr(s32 &nLength)
{
    BOOL32 bRet;
    s8 achTemp[MAX_XMLDOC_LEN];
    bRet = m_cMarkUp.GetDoc(achTemp);
    if (bRet)
    {
        sprintf(m_achXmlDoc,"%s%s","<?xml version = \"1.0\" encoding = \"gb2312\"?>",achTemp);
        nLength = strlen(m_achXmlDoc);
        return m_achXmlDoc;
    }
    else
    {
        nLength = 0;
        return NULL;
    }
}
