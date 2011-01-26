#include "markup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//空白字符
const char szBlankChar[] =
{
	0x08, //0x20, 0x0a, 0x0d,
};
//标记字符
const char szFlagChar[] =
{
	'<', '>', //'/', '=',
};


static long IsBlank(char ch)
{
	s32 i = 0;
	for (i=0; i<sizeof(szBlankChar); i++)
	{
		if (szBlankChar[i] == ch)
		{
			return 1;
		}
	}
	return 0;
}

static long IsFlag(char ch)
{
	s32 i = 0;
	for (i=0; i<sizeof(szFlagChar); i++)
	{
		if (szFlagChar[i] == ch)
		{
			return 1;
		}
	}
	return 0;
}

//跳过空白字符将*ppszXML指向从*ppszXML开始的第一个非空字符
//返回被跳过的字符数
long IgnoreBlank(char **ppszXML)
{
	char *pszTmp = NULL;
	long lRet = 0;
	pszTmp = *ppszXML;
	while (*pszTmp)
	{
		if (IsBlank(*pszTmp))
		{
			pszTmp++;
			lRet++;
			continue;
		}
		break;
	}
	*ppszXML = pszTmp;
	return lRet;
}



CMarkUp::CMarkUp()
{
    m_nUsedSize = 1;
    m_ptCurNode = m_atAllNode; 
    m_ptLastNode = NULL;//added by lqp
    ::memset(m_pszXml, 0, XML_MAX_XML_LEN*sizeof(char));
    m_pcurXml = m_pszXml;
}

CMarkUp::~CMarkUp()
{
    m_ptCurNode = NULL;    
}

BOOL32 CMarkUp::SetDoc(char* pDoc)
{
    if (NULL == pDoc)
    {                
        //assert(NULL != pDoc);        
        return false;
    }
    this->Clear();
    

    return this->ParserXml(pDoc);    
}


void CMarkUp::ResetMainPos()
{
	// 第一个节点不是真实节点，应该指向第一个真实节点
    m_ptCurNode = m_atAllNode;
    m_ptLastNode = NULL;//added by lqp
	if (m_ptCurNode->pFirstChild != NULL)
	{
		m_ptCurNode = m_ptCurNode->pFirstChild;
	}
}

BOOL32 CMarkUp::IntoElem()
{
	// 如果上一次操作的节点是在当前节点的子节点，那么进入上一次的节点；否则进入第一个子节点
	if (m_ptLastNode != NULL && m_ptLastNode->pParent == m_ptCurNode )
	{
		m_ptCurNode = m_ptLastNode;
		return TRUE;
	}
	else
	{
		if (m_ptCurNode->pFirstChild != NULL)
		{
			m_ptCurNode = m_ptCurNode->pFirstChild;
			return TRUE;
		}
	}
	return FALSE;

//     if (NULL != m_ptLastNode) 
//     {
//         m_ptCurNode = m_ptLastNode;
//     }
//     return false;
}
BOOL32 CMarkUp::OutOfElem()
{
    if (m_ptCurNode->pParent)
    {
        m_ptCurNode = m_ptCurNode->pParent;
        return TRUE;
    }    
    return FALSE;
}

//在xml字符当前位置开始查找第一个 "<******>"
//如果成功则返回1且*ppszBegin指向字符'<',*ppszEnd指向字符'>'*ppszXML 也自动跳到指向'>'之后的位置
//如果失败则返回0且*ppszBegin和*ppszEnd值不变，*ppszXML指向当前位置开始的第一个非空字符
u32 CMarkUp::FindItem(char **ppszXML, char **ppszBegin, char **ppszEnd)
{
	char *pszTmp = NULL;
	char *pszBegin = NULL;
	char *pszEnd = NULL;
	long lRet = 0;
	pszTmp = *ppszXML;
	while (*pszTmp)
	{
		/*if (IsBlank(*pszTmp))
		{
			pszTmp++;
			*ppszXML = pszTmp;
			continue;
		}*/
		if (!pszBegin)
		{
			if (*pszTmp == '<')
			{
				pszBegin = pszTmp;
			}			
		}
		else
		{
			if (*pszTmp == '>')
			{
				pszEnd = pszTmp;
				*ppszXML = pszTmp+1;//指向'>'之后
				*ppszBegin = pszBegin;
				*ppszEnd = pszEnd;
				lRet = 1;
				break;
			}
		}
		pszTmp++;
	}	
	return lRet;
}


BOOL32 CMarkUp::ParserXml(char* pDoc)
{
    char* pXml = pDoc;

    //通过循环搞定，不使用递归      
    TXmlNode* pCurNode = m_atAllNode;
    
    char szEndTag[XML_MAX_TAG_LEN] = {0};    
    char *pszBegin = NULL;
	char *pszEnd = NULL;
    
    //查找节点增加到根节点的下面
    while (this->FindItem(&pXml, &pszBegin, &pszEnd))
    {
        if (*(pszBegin+1) == '/')// '</' 带数据的结点结束
		{	
			//没有'< >'只有'</ >'表明xml格式错误需要清空后返回
			 //取出'</'里的节点名 考虑为0的异常情况                        
            s32 nLength = pszEnd - pszBegin + 1;  //<...> 的总长度
                                    
            memcpy(szEndTag, pszBegin+2, nLength-3);
            szEndTag[nLength-3] = '\0';
            
            //如果是同级别的就不增加
            if (strcmp(pCurNode->pTagStart, szEndTag) == 0) 
            {
                //Item 是end 不管是什么样的只要将当前节点，设置为父节点就可以了                                                
                pCurNode = pCurNode->pParent;;
            }
            else
            {
                // Error
                return false;
            }
            
		}
        else if (*(pszBegin+1) == '?')
		{
			continue;
		}
        else if (*pszBegin == '<')
        {
            //Item 是个新的节点， 增加之         
            //assert(m_nUsedSize < XML_MAX_NODE);
            TXmlNode* pNewNode =  &m_atAllNode[m_nUsedSize++];           
            pNewNode->Clear();
            pNewNode->pParent = pCurNode;
            //是否是第一个子节点
            if (NULL == pCurNode->pFirstChild) 
            {
                pCurNode->pFirstChild = pNewNode;
                pCurNode->pEndChild = pNewNode;
            }
            else
            {
                pCurNode->pEndChild->pNextBrother = pNewNode;                
                pCurNode->pEndChild = pNewNode;
            }
            
            //根据begin end拿到节点tag信息  
            memcpy(m_pcurXml, pszBegin+1, pszEnd - pszBegin-1);
            m_pcurXml[pszEnd - pszBegin-1] = '\0';
            
            pNewNode->pTagStart = m_pcurXml;
            pNewNode->nTagLen = pszEnd - pszBegin-1;
            m_pcurXml += pNewNode->nTagLen+1;

            //从end后面拿到现在的value的信息
            char* pChar = NULL;
            s32 nLen = this->GetContent(pszEnd+1, &pChar);//看看是否有Data            
            if (nLen > 0)  
            {
                memcpy(m_pcurXml, pChar, nLen);
                m_pcurXml[nLen] = '\0';                
                pNewNode->pDataStart = m_pcurXml;
                pNewNode->nDataLen = nLen;
                m_pcurXml += nLen+1;
                                
            }           
            // Set Cur Node = New Node
            pCurNode = pNewNode;                        
        }
        else
        {
            //error
            return false;
        }
    }
    
    return true;
}


s32 CMarkUp::GetContent(char *pszXML, char **ppszData)
{
	char *pszRet = NULL;
	s32 nCount = 0;
	char *pszTmp = NULL;
	char ch = 0;
	pszTmp = pszXML;
	//找到第一个非空字符的位置
	IgnoreBlank(&pszTmp);
	pszRet = pszTmp;
	ch = *pszTmp;
	while (ch && !IsBlank(ch) && !IsFlag(ch))
	{
		nCount++;
		pszTmp++;
		ch = *pszTmp;
	}
	if (nCount > 0)
	{
		*ppszData = pszRet;
	}
	return nCount;
}

struct tStackEle
{
    const TXmlNode* pNode;
    s32      byMask; // 0表示tag没有处理过 1表示tag头已经处理过
    tStackEle()
    {
        pNode = NULL;
        byMask = 0;
    }
};
void CMarkUp::PrintNodeInfo(char* pszInfo, const TXmlNode* tNode, BOOL32 bReturn)
{
	if (!pszInfo)
		return;

//	pszInfo[0] = '\0';

    tStackEle aStack[XML_MAX_LEVEL];

    s32 nStartInx = 0;
    aStack[nStartInx].pNode = tNode;
    aStack[nStartInx].byMask = 0;
    //栈的size ＝ nStartInx +1;
    //类似push();

    while (nStartInx >= 0/*!mystack.empty()*/) 
    {
        //stack top element
        tStackEle tEle = aStack[nStartInx];
        
        if (tEle.byMask == 0) 
        {
            //输出tag头
            if (tEle.pNode->pTagStart)
            {
				strcat(pszInfo, "<");
                strcat(pszInfo, tEle.pNode->pTagStart);
                strcat(pszInfo, ">");
                if (bReturn && tEle.pNode->pFirstChild)
                {
                    strcat(pszInfo, "\r\n");                
                }
            }
            if (tEle.pNode->pDataStart)
            {                
                strcat(pszInfo, tEle.pNode->pDataStart);                
            }
            //修改栈顶mask信息 
            aStack[nStartInx].byMask = 1;            
            
            //还有子节点，先打印子节点
            if (tEle.pNode->pFirstChild)
            {                                
                //将子入栈                                
                nStartInx += 1;
                aStack[nStartInx].pNode = tEle.pNode->pFirstChild;
                aStack[nStartInx].byMask = 0;
                //mystack.push(tEle);
                continue;
            }  
        }
        else if (1 == tEle.byMask) 
        {
            //输出tag尾 结束了出栈
            if (tEle.pNode->pTagStart)
            {
                strcat(pszInfo, "</");
                strcat(pszInfo, tEle.pNode->pTagStart);
                strcat(pszInfo, ">");
                if (bReturn)
                {
                    strcat(pszInfo, "\r\n");
                }                
            }
            //出栈
            nStartInx -= 1;
            //mystack.pop();   
            
            //如果是根节点，就不要再打印其兄弟了
            if (tEle.pNode != tNode && tEle.pNode->pNextBrother)
            {                
                //将子入栈
                nStartInx += 1;
                aStack[nStartInx].pNode = tEle.pNode->pNextBrother;
                aStack[nStartInx].byMask = 0;
                continue;
            }                        
        }        
    }
}

void CMarkUp::PrintNodeInfo(char* pszInfo, BOOL32 bReturn)
{
	if (!pszInfo)
		return;

    pszInfo[0] = '\0';
    this->PrintNodeInfo(pszInfo, m_atAllNode, bReturn);
    return;    
}


BOOL32 CMarkUp::GetDoc(char* pszDoc)
{
    pszDoc[0] = '\0';
    this->PrintNodeInfo(pszDoc, false);
    return true;
}

BOOL32 CMarkUp::AddElem(const char*strEle, s32 nValue)
{
    //assert(NULL != m_ptCurNode);
    
    char szInfo[XML_INT_MAX_LEN] = {0};
    sprintf(szInfo, "%d", nValue);    
    this->AddElem(strEle, szInfo);
    return true;        
}
BOOL32 CMarkUp::AddElem(const char* strEle, const char* strValue)
{
    //对字符串进行转义    
    //assert(NULL != m_ptCurNode);
    //assert(m_nUsedSize < XML_MAX_NODE);        
    
    TXmlNode* pNewNode = &m_atAllNode[m_nUsedSize++];
    pNewNode->pTagStart = m_pcurXml;    
    strcpy(m_pcurXml, strEle);

    //实际进行转换后的长度
    pNewNode->nTagLen = strlen(strEle);
    m_pcurXml += pNewNode->nTagLen+1;
    
    if (strValue)
    {
        pNewNode->pDataStart = m_pcurXml;
        //调用转义拷贝
        pNewNode->nDataLen = this->xmlStrcpy(m_pcurXml, strValue);
        m_pcurXml += pNewNode->nDataLen+1;
    }
    
    

    pNewNode->pParent = m_ptCurNode;
    if (NULL == m_ptCurNode->pFirstChild) 
    {
        //设置第一个子节点
        m_ptCurNode->pFirstChild = pNewNode;
        m_ptCurNode->pEndChild = pNewNode;
    }
    else
    {
        //增加节点到队列尾部
        m_ptCurNode->pEndChild->pNextBrother = pNewNode;                
        m_ptCurNode->pEndChild = pNewNode;
    }

    m_ptLastNode = pNewNode;
    return true;    
}

BOOL32 CMarkUp::Clear()
{
    m_nUsedSize = 1;
    m_pcurXml = m_pszXml;
    this->ResetMainPos();
    return true;
}


BOOL32 CMarkUp::FindElem(const char* strEle)
{
    //assert(NULL != m_ptCurNode);
	// Out First
	TXmlNode* pNode = m_ptCurNode->pParent;
	if (pNode == NULL)
	{
		return FALSE;
	}

	// Find
    BOOL32 bFind = FALSE;
    for(TXmlNode* pFindNode = pNode->pFirstChild; NULL != pFindNode; pFindNode = pFindNode->pNextBrother)
    {
        if (0 == strcmp(strEle, pFindNode->pTagStart)) 
        {
            bFind = TRUE;            
			// Into Item
			m_ptCurNode = pFindNode;
            m_ptLastNode = m_ptCurNode;            
            break;
        }        
    }

    return bFind;
}


BOOL32 CMarkUp::First()
{    
    if (NULL != m_ptCurNode->pParent)
    {
		if (NULL != m_ptCurNode->pParent->pFirstChild)
		{
			m_ptCurNode = m_ptCurNode->pParent->pFirstChild;        
			return TRUE;
		}
    }
    return FALSE;
}
BOOL32 CMarkUp::NextSibling()
{    
    if (NULL != m_ptCurNode->pNextBrother)
    {
        m_ptCurNode = m_ptCurNode->pNextBrother;        
        return TRUE;
    }    
    return FALSE;
}
BOOL32 CMarkUp::IsHasChild()
{    
    return (NULL != m_ptCurNode->pFirstChild);
}


char* CMarkUp::GetTagName() const
{
    if (m_ptCurNode->pTagStart)
    {
        return m_ptCurNode->pTagStart;
    }
    return NULL;
}
char* CMarkUp::GetData()
{            
    //全部先进行转义，如果需要判断3个转移符号 & < > 
    s32 nLen = 0;
    if (m_ptCurNode->pDataStart)
    {
        nLen = this->xmlRevStrcpy(m_szCurData, m_ptCurNode->pDataStart);
    }    
    m_szCurData[nLen] = '\0';
    return m_szCurData;
}

s32 CMarkUp::GetDataInt() const
{
    if (m_ptCurNode->nDataLen)
    {
        return atoi(m_ptCurNode->pDataStart);
    }
    return false;    
}



s32 CMarkUp::xmlStrcpy(char* szDst, const char* szSrc)
{
    //assert(NULL != szDst);
    //assert(NULL != szSrc);
    char* szDstOri = szDst;
    while (*szSrc != '\0')
    {
        //先对&进行转换，否则< > 都被转换了
        if (*szSrc == '&')
        {
            strcpy(szDst, "&amp;");
            szDst += strlen("&amp;");
            szSrc++;
        }
        else if (*szSrc == '<')
        {
            strcpy(szDst, "&lt;");
            szDst += strlen("&lt;");
            szSrc++;
        }
        else if (*szSrc == '>') 
        {
            strcpy(szDst, "&gt;");
            szDst += strlen("&gt;");
            szSrc++;
        }
        else
        {
            *szDst++ = *szSrc++;
        }
    }
    return (szDst - szDstOri);
}

s32 CMarkUp::xmlRevStrcpy(char* szDst, const char* szSrc)
{
    //assert(NULL != szDst);
    //assert(NULL != szSrc);
    char* szDstOri = szDst;
    while (*szSrc != '\0')
    {
        //考虑字符串的边界
        if (*szSrc == '&')
        {
            if (memcmp(szSrc, "&lt;", strlen("&lt;")) == 0)
            {            
                *szDst++ = '<';
                szSrc += strlen("&lt;");
            }
            else if (memcmp(szSrc, "&gt;", strlen("&gt;")) == 0)
            {            
                *szDst++ = '>';
                szSrc += strlen("&gt;");
            }
            else if (memcmp(szSrc, "&amp;", strlen("&amp;")) == 0)
            {            
                *szDst++ = '&';
                szSrc += strlen("&amp;");
            }
            else
            {
                //单纯的就是个& 
                *szDst++ = *szSrc++;
            }
        }                
        else
        {
            *szDst++ = *szSrc++;
        }
    }
    return (szDst - szDstOri);
}

BOOL32 CMarkUp::Save(char* pfilePath)
{
    if (pfilePath)
    {        
        FILE* hFile = fopen(pfilePath, "w");        
        if (hFile)
        {
            char szDoc[XML_MAX_XML_LEN] = {0};
            this->PrintNodeInfo(szDoc, true);
            fwrite(szDoc, strlen(szDoc), 1, hFile);
            fclose(hFile);
        }        
        return true;
    }
    return true;    
}
