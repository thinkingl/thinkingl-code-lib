/*****************************************************************************
  ģ����      : 
  �ļ���      : MarkUp.h
  ����ļ�    : 
  �ļ�ʵ�ֹ���: 
  ����        : ������
  �汾        : V1.0  Copyright(C) 1997-2006 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:  
��ϸ����ĵ�����VSKDM_VOB\10-Common\ doc\design\cbb\xmlparser 
��  ��      �汾        �޸���      �޸�����
2010/2/5   1.0         ������      ����
******************************************************************************/
// MarkUp.h: interface for the CMarkUp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XML_MARK_UP_
#define _XML_MARK_UP_

#include "cmstypes.h"
#pragma warning(disable:4786)

//XML���������Ľڵ���
#define XML_MAX_NODE                 4096

//xml tag�����Ŀ
#define XML_MAX_TAG_LEN              1024

//xml data�������
#define XML_MAX_DATA_LEN             1024*3
//xml ������󳤶�
#define XML_MAX_XML_LEN              1024*64

//xml����Ĳ㼶��������ջ����
#define  XML_MAX_LEVEL               33

//INT ����ַ���
#define XML_INT_MAX_LEN              33 

//#include "assert.h"  ȥ��assert����

struct TXmlNode;
struct TXmlNode
{
    char* pTagStart;  //tag�ڵ����ʼλ��
    s32 nTagLen;    //tag�ڵ�ĳ���,����������0����
    char* pDataStart; //data����ʼλ��
    s32 nDataLen;   //data�ĳ���
    
    TXmlNode* pParent;      //���ڵ�
    TXmlNode* pFirstChild;  //�ӽڵ�
    TXmlNode* pEndChild;    //���һ���ӽڵ�
    TXmlNode* pNextBrother; //�ֵܽڵ�

    TXmlNode()
    {
        Clear();
    }
    void Clear()
    {
        pParent = NULL;  
        pFirstChild = NULL;
        pNextBrother = NULL;
        pEndChild = NULL;
        
        pTagStart = NULL;
        nTagLen = 0;
        pDataStart = NULL;
        nDataLen = 0;            
    };
};


class CMarkUp  
{
    //��ֵ��Ҫ��ָ�����m_ptCurNode�Ƚ������¼��㣬�ݲ�֧��
    CMarkUp(const CMarkUp& cRef);
    CMarkUp& operator = (const CMarkUp& cRef); 
public:
    
	CMarkUp();
	virtual ~CMarkUp();

    //�ָ�ʹ�õ�������Ϣ
    BOOL32 Clear();

    //����xml�ַ���Ϣ
    BOOL32 SetDoc(char* pDoc);

    //�ַ����ܳ���ʱ�򣬲��˷ѿռ�
    BOOL32 GetDoc(char* pszDoc);
    //����xml��Ϣ���ļ���  Internet Explorer��֧�ֶ��root�ڵ����ʾ
    BOOL32 Save(char* pfilePath);
 
    //���õ�ǰ�Ĳ����Ľڵ���Ϣ,���нڵ���ƶ�
    void ResetMainPos(); 
    BOOL32 IntoElem();
	BOOL32 OutOfElem();

    BOOL32 AddElem(const char* strEle, s32 nValue);
    BOOL32 AddElem(const char* strEle, const char* strValue = NULL);

    //�Ὣ��ǰ�ڵ�ָ�룬ָ����ҵĽڵ㣬����ҵ��Ļ�
    BOOL32 FindElem(const char* strEle);
    
    //��ȡ��ǰ�ڵ����Ϣ
    char* GetTagName() const;
    //�ظ�����ʱ�����ص�char*���ݻᱻ���ǣ������鱣��
    char* GetData();
    s32   GetDataInt() const;
    

    //ѭ����������
    BOOL32 First();
	BOOL32 NextSibling();
	BOOL32 IsHasChild();
    
    void PrintNodeInfo(char* pszInfo, BOOL32 bReturn = true); //bReturn �Ƿ���س����ţ����ڴ�ӡ��������
protected:
    //�����ַ���ת��ͷ�ת��
    s32 xmlStrcpy(char* szDst, const char* szSrc);
    s32 xmlRevStrcpy(char* szDst, const char* szSrc);

    //��ӡ�ڵ���Ϣ��xml
    void PrintNodeInfo(char* pszInfo, const TXmlNode* tNode, BOOL32 bReturn = false);	

    //����xml��Ϊ�ڵ�
    BOOL32 ParserXml(char* pDoc); 
    u32 FindItem(char **ppszXML, char **ppszBegin, char **ppszEnd);
    s32  GetContent(char *pszXML, char **ppszData);    

    s32 m_nUsedSize;  //�ڵ��ʹ������
    TXmlNode m_atAllNode[XML_MAX_NODE]; //��һ���ڵ�Ϊroot�ڵ㣬���ڱ���xml���ж�����ڵ�����

    TXmlNode* m_ptCurNode;   //��ǰ�Ľڵ�ָ��
    TXmlNode* m_ptLastNode;  //������һ�β��Һ���ӵĽڵ㣬���ڽ���Into��Out�л�    
    char m_pszXml[XML_MAX_XML_LEN];     
    char m_szCurData[XML_MAX_DATA_LEN];//���浱ǰ���ҵ���data��ת�������Ϣ��ԭʼdata��Ҫ����ת�壩
    char* m_pcurXml;         //��ǰ��ָ��xmlĩβ��ָ��
    
};

#endif
