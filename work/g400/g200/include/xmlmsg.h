#ifndef _XML_MSG_H
#define _XML_MSG_H
#include "markup.h"

#define  MAX_XMLDOC_LEN    30*1024
#define  COMMAND_BUF_LEN    5
#define  VALUE_BUF_LEN      128

#define  MAX_PORT_BUF     1024
#define  MAX_PORTNO_BUF   10
#define  COMMAND         "command"
#define  CONTENT         "content"

class CXmlMsg
{
public:
    CXmlMsg(LPSTR pszXml = "<?xml version='1.0' encoding='GB2312'?><kedacomxmldata></kedacomxmldata>");
    s32 GetCommand();
    void SetCommand(s32 cmd);
    void Go2Content();
    BOOL32 GoUpstairs();
    BOOL32 GoDownstairs();
    BOOL32 NextSibling();
    /*
        <parent>...<node></node></parent>
    */
    void BuildNode(LPCSTR pszNodeName);
    /*
        <node>value</node>
    */
    void BuildNode(LPCSTR pszNodeName,LPCSTR szFormat,...);
    LPCSTR GetNodeStringValue(const s8* strEle = NULL);
    s32 GetNodeIntValue(const s8* strEle = NULL);
    LPCSTR GetMsgStr(s32 &nLength);
private:
    s8 m_achCmd[COMMAND_BUF_LEN];
    s8 m_achValue[VALUE_BUF_LEN];
    s8 m_achXmlDoc[MAX_XMLDOC_LEN];

    CMarkUp m_cMarkUp;
};


#endif/*_XML_MSG_H*/

