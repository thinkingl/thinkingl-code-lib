#include "markup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//�հ��ַ�
const char szBlankChar[] =
{
	0x08, //0x20, 0x0a, 0x0d,
};
//����ַ�
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

//�����հ��ַ���*ppszXMLָ���*ppszXML��ʼ�ĵ�һ���ǿ��ַ�
//���ر��������ַ���
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
	// ��һ���ڵ㲻����ʵ�ڵ㣬Ӧ��ָ���һ����ʵ�ڵ�
    m_ptCurNode = m_atAllNode;
    m_ptLastNode = NULL;//added by lqp
	if (m_ptCurNode->pFirstChild != NULL)
	{
		m_ptCurNode = m_ptCurNode->pFirstChild;
	}
}

BOOL32 CMarkUp::IntoElem()
{
	// �����һ�β����Ľڵ����ڵ�ǰ�ڵ���ӽڵ㣬��ô������һ�εĽڵ㣻��������һ���ӽڵ�
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

//��xml�ַ���ǰλ�ÿ�ʼ���ҵ�һ�� "<******>"
//����ɹ��򷵻�1��*ppszBeginָ���ַ�'<',*ppszEndָ���ַ�'>'*ppszXML Ҳ�Զ�����ָ��'>'֮���λ��
//���ʧ���򷵻�0��*ppszBegin��*ppszEndֵ���䣬*ppszXMLָ��ǰλ�ÿ�ʼ�ĵ�һ���ǿ��ַ�
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
				*ppszXML = pszTmp+1;//ָ��'>'֮��
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

    //ͨ��ѭ���㶨����ʹ�õݹ�      
    TXmlNode* pCurNode = m_atAllNode;
    
    char szEndTag[XML_MAX_TAG_LEN] = {0};    
    char *pszBegin = NULL;
	char *pszEnd = NULL;
    
    //���ҽڵ����ӵ����ڵ������
    while (this->FindItem(&pXml, &pszBegin, &pszEnd))
    {
        if (*(pszBegin+1) == '/')// '</' �����ݵĽ�����
		{	
			//û��'< >'ֻ��'</ >'����xml��ʽ������Ҫ��պ󷵻�
			 //ȡ��'</'��Ľڵ��� ����Ϊ0���쳣���                        
            s32 nLength = pszEnd - pszBegin + 1;  //<...> ���ܳ���
                                    
            memcpy(szEndTag, pszBegin+2, nLength-3);
            szEndTag[nLength-3] = '\0';
            
            //�����ͬ����ľͲ�����
            if (strcmp(pCurNode->pTagStart, szEndTag) == 0) 
            {
                //Item ��end ������ʲô����ֻҪ����ǰ�ڵ㣬����Ϊ���ڵ�Ϳ�����                                                
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
            //Item �Ǹ��µĽڵ㣬 ����֮         
            //assert(m_nUsedSize < XML_MAX_NODE);
            TXmlNode* pNewNode =  &m_atAllNode[m_nUsedSize++];           
            pNewNode->Clear();
            pNewNode->pParent = pCurNode;
            //�Ƿ��ǵ�һ���ӽڵ�
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
            
            //����begin end�õ��ڵ�tag��Ϣ  
            memcpy(m_pcurXml, pszBegin+1, pszEnd - pszBegin-1);
            m_pcurXml[pszEnd - pszBegin-1] = '\0';
            
            pNewNode->pTagStart = m_pcurXml;
            pNewNode->nTagLen = pszEnd - pszBegin-1;
            m_pcurXml += pNewNode->nTagLen+1;

            //��end�����õ����ڵ�value����Ϣ
            char* pChar = NULL;
            s32 nLen = this->GetContent(pszEnd+1, &pChar);//�����Ƿ���Data            
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
	//�ҵ���һ���ǿ��ַ���λ��
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
    s32      byMask; // 0��ʾtagû�д���� 1��ʾtagͷ�Ѿ������
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
    //ջ��size �� nStartInx +1;
    //����push();

    while (nStartInx >= 0/*!mystack.empty()*/) 
    {
        //stack top element
        tStackEle tEle = aStack[nStartInx];
        
        if (tEle.byMask == 0) 
        {
            //���tagͷ
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
            //�޸�ջ��mask��Ϣ 
            aStack[nStartInx].byMask = 1;            
            
            //�����ӽڵ㣬�ȴ�ӡ�ӽڵ�
            if (tEle.pNode->pFirstChild)
            {                                
                //������ջ                                
                nStartInx += 1;
                aStack[nStartInx].pNode = tEle.pNode->pFirstChild;
                aStack[nStartInx].byMask = 0;
                //mystack.push(tEle);
                continue;
            }  
        }
        else if (1 == tEle.byMask) 
        {
            //���tagβ �����˳�ջ
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
            //��ջ
            nStartInx -= 1;
            //mystack.pop();   
            
            //����Ǹ��ڵ㣬�Ͳ�Ҫ�ٴ�ӡ���ֵ���
            if (tEle.pNode != tNode && tEle.pNode->pNextBrother)
            {                
                //������ջ
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
    //���ַ�������ת��    
    //assert(NULL != m_ptCurNode);
    //assert(m_nUsedSize < XML_MAX_NODE);        
    
    TXmlNode* pNewNode = &m_atAllNode[m_nUsedSize++];
    pNewNode->pTagStart = m_pcurXml;    
    strcpy(m_pcurXml, strEle);

    //ʵ�ʽ���ת����ĳ���
    pNewNode->nTagLen = strlen(strEle);
    m_pcurXml += pNewNode->nTagLen+1;
    
    if (strValue)
    {
        pNewNode->pDataStart = m_pcurXml;
        //����ת�忽��
        pNewNode->nDataLen = this->xmlStrcpy(m_pcurXml, strValue);
        m_pcurXml += pNewNode->nDataLen+1;
    }
    
    

    pNewNode->pParent = m_ptCurNode;
    if (NULL == m_ptCurNode->pFirstChild) 
    {
        //���õ�һ���ӽڵ�
        m_ptCurNode->pFirstChild = pNewNode;
        m_ptCurNode->pEndChild = pNewNode;
    }
    else
    {
        //���ӽڵ㵽����β��
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
    //ȫ���Ƚ���ת�壬�����Ҫ�ж�3��ת�Ʒ��� & < > 
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
        //�ȶ�&����ת��������< > ����ת����
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
        //�����ַ����ı߽�
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
                //�����ľ��Ǹ�& 
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
