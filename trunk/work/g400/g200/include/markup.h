/*****************************************************************************
  模块名      : 
  文件名      : MarkUp.h
  相关文件    : 
  文件实现功能: 
  作者        : 王晓辉
  版本        : V1.0  Copyright(C) 1997-2006 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:  
详细设计文档见：VSKDM_VOB\10-Common\ doc\design\cbb\xmlparser 
日  期      版本        修改人      修改内容
2010/2/5   1.0         王晓辉      创建
******************************************************************************/
// MarkUp.h: interface for the CMarkUp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XML_MARK_UP_
#define _XML_MARK_UP_

#include "cmstypes.h"
#pragma warning(disable:4786)

//XML报文中最大的节点数
#define XML_MAX_NODE                 4096

//xml tag的最长数目
#define XML_MAX_TAG_LEN              1024

//xml data的最长长度
#define XML_MAX_DATA_LEN             1024*3
//xml 报文最大长度
#define XML_MAX_XML_LEN              1024*64

//xml最深的层级数，用于栈遍历
#define  XML_MAX_LEVEL               33

//INT 的最长字符串
#define XML_INT_MAX_LEN              33 

//#include "assert.h"  去处assert错误

struct TXmlNode;
struct TXmlNode
{
    char* pTagStart;  //tag节点的起始位置
    s32 nTagLen;    //tag节点的长度,不包含最后的0在内
    char* pDataStart; //data的起始位置
    s32 nDataLen;   //data的长度
    
    TXmlNode* pParent;      //父节点
    TXmlNode* pFirstChild;  //子节点
    TXmlNode* pEndChild;    //最后一个子节点
    TXmlNode* pNextBrother; //兄弟节点

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
    //赋值需要对指针变量m_ptCurNode等进行重新计算，暂不支持
    CMarkUp(const CMarkUp& cRef);
    CMarkUp& operator = (const CMarkUp& cRef); 
public:
    
	CMarkUp();
	virtual ~CMarkUp();

    //恢复使用的数组信息
    BOOL32 Clear();

    //设置xml字符信息
    BOOL32 SetDoc(char* pDoc);

    //字符串很长的时候，不浪费空间
    BOOL32 GetDoc(char* pszDoc);
    //保存xml信息到文件中  Internet Explorer不支持多个root节点的显示
    BOOL32 Save(char* pfilePath);
 
    //设置当前的操作的节点信息,进行节点的移动
    void ResetMainPos(); 
    BOOL32 IntoElem();
	BOOL32 OutOfElem();

    BOOL32 AddElem(const char* strEle, s32 nValue);
    BOOL32 AddElem(const char* strEle, const char* strValue = NULL);

    //会将当前节点指针，指向查找的节点，如果找到的话
    BOOL32 FindElem(const char* strEle);
    
    //获取当前节点的信息
    char* GetTagName() const;
    //重复调用时，返回的char*内容会被覆盖，不建议保存
    char* GetData();
    s32   GetDataInt() const;
    

    //循环遍历函数
    BOOL32 First();
	BOOL32 NextSibling();
	BOOL32 IsHasChild();
    
    void PrintNodeInfo(char* pszInfo, BOOL32 bReturn = true); //bReturn 是否带回车符号，用于打印到窗口上
protected:
    //进行字符串转义和反转义
    s32 xmlStrcpy(char* szDst, const char* szSrc);
    s32 xmlRevStrcpy(char* szDst, const char* szSrc);

    //打印节点信息到xml
    void PrintNodeInfo(char* pszInfo, const TXmlNode* tNode, BOOL32 bReturn = false);	

    //解析xml到为节点
    BOOL32 ParserXml(char* pDoc); 
    u32 FindItem(char **ppszXML, char **ppszBegin, char **ppszEnd);
    s32  GetContent(char *pszXML, char **ppszData);    

    s32 m_nUsedSize;  //节点的使用索引
    TXmlNode m_atAllNode[XML_MAX_NODE]; //第一个节点为root节点，用于保存xml中有多个根节点的情况

    TXmlNode* m_ptCurNode;   //当前的节点指针
    TXmlNode* m_ptLastNode;  //保存上一次查找和添加的节点，用于进行Into和Out切换    
    char m_pszXml[XML_MAX_XML_LEN];     
    char m_szCurData[XML_MAX_DATA_LEN];//保存当前查找到的data的转换后的信息（原始data需要进行转义）
    char* m_pcurXml;         //当前的指向xml末尾的指针
    
};

#endif
