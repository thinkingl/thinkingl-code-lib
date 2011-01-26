/*****************************************************************************
������      : KTM2.0
ģ����      : CMU
�ļ���      : kdmlist.h
����ļ�    : 
�ļ�ʵ�ֹ���: 
����        : luxz
�汾        : V1.0  Copyright(C) 1997-2005 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��        �汾        �޸���      �޸�����
2006/02/19    1.0         luxz       ����
******************************************************************************/
#ifndef _KDM_LIST_H_
#define _KDM_LIST_H_

template <class cls>
class CKdmItem
{	//�ڵ���
public:
    CKdmItem() : next(NULL), pre(NULL), bIsValid(FALSE)
    {
        data = new cls;
    }
    
    ~CKdmItem()
    {
        if(data != NULL)
        {
            delete data;
            data = NULL;
            next = NULL;
            pre = NULL;
        }
    }
    cls* data;	//��������
    BOOL32 bIsValid;	//�Ƿ���Ч
    CKdmItem* next;	//�½ڵ�
    CKdmItem* pre;	//�Ͻڵ�
};

template <class cls, u16 wNewItem> class CKdmListEx;

template <class cls, u16 wNewItem = 0>
class CKdmList
{	//list����
// public:
//     typedef struct CKdmItem
//     {	//�ڵ���
//         CKdmItem() : next(NULL), pre(NULL), bIsValid(FALSE)
//         {
//             data = new cls;
//         }
//         
//         ~CKdmItem()
//         {
//             if(data != NULL)
//             {
//                 delete data;
//                 data = NULL;
//                 next = NULL;
//                 pre = NULL;
//             }
//         }
//         cls* data;	//��������
//         BOOL32 bIsValid;	//�Ƿ���Ч
//         CKdmItem* next;	//�½ڵ�
//         CKdmItem* pre;	//�Ͻڵ�
//     } CKdmItem;
    typedef CKdmItem<cls> CItemType;
public:
    CKdmList() : head(NULL), tail(NULL), curPos(NULL), m_wItemCount(0)
    {
        OspSemBCreate(&m_hListSem);
    }
    
    /*====================================================================
    ������      : Lock
    ����        : list����
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/
    void Lock()
    {	//����
        OspSemTake(m_hListSem);
    }
    
    /*====================================================================
    ������      : Unlock
    ����        : list����
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/
    void Unlock()
    {	//����
        OspSemGive(m_hListSem);
    }
    
    virtual ~CKdmList()
    {
        DelAllItem();	//ɾ�����нڵ�
        OspSemDelete(m_hListSem);	//ɾ����
    }
    
    /*====================================================================
    ������      : PrintList
    ����        : ��ӡ���нڵ���Ϣ
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: u8 byOffsetSize-��ӡƫ��tab��
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/
//     virtual void PrintList(u8 byOffsetSize = 0) const
//     {	
//         CItemType* cur = head;
//         while(cur)
//         {
//             CItemType* tmp = cur->next;		    
//             
//             OspPrintf(TRUE, FALSE, "***************************************************************\n");
//             cur->data->PrintData(byOffsetSize);
//             
//             cur = tmp;
//         }
//         OspPrintf(TRUE, FALSE, "��ǰ��:%d\n", GetItemCount());
//     }
    
    /*====================================================================
    ������      : AddItem
    ����        : ���ӽڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/
    virtual cls* AddItem()
    {        
        CItemType* pcItem = new CItemType;
        if(pcItem == NULL)
        {
            return NULL;
        }
        
        RegItem(pcItem, tail, NULL);
        
        m_wItemCount++;
        return pcItem->data;
    }
    
    /*====================================================================
    ������      : DelItem
    ����        : ɾ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/    
    virtual void DelItem(cls* data)
    {
        CItemType* pcTemp = head;
        while(pcTemp)
        {
            if(pcTemp->data == data)
            {
                UnregItem(pcTemp);
                delete pcTemp;
                m_wItemCount--;
                return;
            }		    
            pcTemp = pcTemp->next;
        }
    }
    
    /*====================================================================
    ������      : GetNextItem
    ����        : ��ȡָ���ڵ����һ�ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const cls* curData-ָ���ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    cls* GetNextItem(const cls* curData) const
    {
        CItemType* pcItem = FindItem(curData);
        if(pcItem == NULL) return NULL;
        
        pcItem = pcItem->next;
        if(pcItem == NULL) return NULL;
        
        return pcItem->data;
    }
    
    /*====================================================================
    ������      : GetPreItem
    ����        : ��ȡָ���ڵ����һ�ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const cls* curData-ָ���ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    cls* GetPreItem(const cls* curData) const
    {
        CItemType* pcItem = FindItem(curData);
        if(pcItem == NULL) return NULL;
        
        pcItem = pcItem->pre;
        if(pcItem == NULL) return NULL;
        
        return pcItem->data;
    }
    
    /*====================================================================
    ������      : SetCurPos
    ����        : �����б�ĵ�ǰ�ڵ�Ϊָ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const cls* curData-ָ���ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    void SetCurPos(const cls* curData)
    {		
        if(curPos!=NULL&&curData==curPos->data)
        {	//�����ǰ�ڵ�Ϊָ���Ľڵ�
            return;
        }
        if(curPos!=NULL&&curPos->next!=NULL&&curData==curPos->next->data)
        {	//ָ���ڵ�Ϊ��ǰ�ڵ��½ڵ�
            curPos=curPos->next;
            return;
        }
        if(curPos!=NULL&&curPos->pre!=NULL&&curData==curPos->pre->data)
        {	//ָ���ڵ�Ϊ��ǰ�ڵ��Ͻڵ�
            curPos=curPos->pre;
            return;
        }
        
        //�������ָ���ڵ�
        CItemType* pcItem = FindItem(curData);	 
        if(pcItem == NULL)
        {
            curPos = head;
        }
        else
        {
            curPos=pcItem;
        }
    }
   /*====================================================================
    ������      : SetNextPos
    ����        : �����б�ĵ�ǰ�ڵ�Ϊָ���ڵ����һ�ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const cls* curData-ָ���ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ������        ����
    ====================================================================*/ 
    cls* SetNextPos(const cls* curData)
    {		
        if(curPos!=NULL&&curData==curPos->data)
        {	//�����ǰ�ڵ�Ϊָ���Ľڵ�
			curPos=curPos->next;
            return curPos == NULL ? NULL : curPos->data;
        }
        if(curPos!=NULL&&curPos->next!=NULL&&curData==curPos->next->data)
        {	//ָ���ڵ�Ϊ��ǰ�ڵ��½ڵ�
            curPos=curPos->next;
			curPos=curPos->next;
            return curPos == NULL ? NULL : curPos->data;
        }
        if(curPos!=NULL&&curPos->pre!=NULL&&curData==curPos->pre->data)
        {	//ָ���ڵ�Ϊ��ǰ�ڵ��Ͻڵ�
            curPos=curPos->pre;
			curPos=curPos->next;
            return curPos == NULL ? NULL : curPos->data;
        }
        
        //�������ָ���ڵ�
        CItemType* pcItem = FindItem(curData);	 
        if(pcItem == NULL)
        {
            curPos = head;
			return curPos == NULL ? NULL : curPos->data;
        }
        else
        {
            curPos=pcItem;
			curPos=curPos->next;
			return curPos == NULL ? NULL : curPos->data;
        }
    }   
    /*====================================================================
    ������      : GetCurItem
    ����        : ��ȡָ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const cls* curData-ָ���ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    CItemType* GetCurItem(const cls* curData) const
    {
        return FindItem(curData);
    }
    
    /*====================================================================
    ������      : PosFirst
    ����        : ��ȡ��һ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��:
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    cls*  PosFirst()
    {
        curPos = head;
        return curPos == NULL ? NULL : curPos->data;
    }
    
    /*====================================================================
    ������      : PosNext
    ����        : ��ȡָ���ڵ����һ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: cls* pcCur--ָ���ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    cls* PosNext(cls* pcCur)
    {
        if(curPos != NULL)
        {
            if(curPos->data == pcCur)
            {
                curPos = curPos->next;
            }
            else
            {
                curPos = FindItem(pcCur);
				if (NULL != curPos)
				{
					curPos = curPos->next;
				}
            }
        }
        
        return curPos == NULL ? NULL : curPos->data;
    }
    
    /*====================================================================
    ������      : PosPre
    ����        : ��ȡָ���ڵ����һ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: cls* pcCur--ָ���ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    cls* PosPre(cls* pcCur)
    {
        if(curPos != NULL)
        {
            if(curPos->data == pcCur)
            {
                curPos = curPos->pre;
            }
            else
            {
                curPos = FindItem(pcCur);
				if (NULL != curPos)
				{
					curPos = curPos->pre;
				}
            }
        }
        
        
        return curPos == NULL ? NULL : curPos->data;
    }
    
    /*====================================================================
    ������      : PosLast
    ����        : ��ȡ�б����ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    cls* PosLast()
    {
        curPos = tail;
        
        return curPos == NULL ? NULL : curPos->data;
    }
    
    /*====================================================================
    ������      : PosCur
    ����        : ��ȡ�б�ǰ�ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    cls* PosCur()
    {
        return curPos == NULL ? NULL : curPos->data;
    }
    
    /*====================================================================
    ������      : IsEmpty
    ����        : �б��Ƿ�Ϊ��
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    BOOL32 IsEmpty()
    {
        return head == NULL;
    }
    
    /*====================================================================
    ������      : DelAllItem
    ����        : ����б�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    BOOL32 DelAllItem()
    {
        CItemType* cur = head;
        CItemType* tmp = head;
        while(cur)
        {
            tmp = cur->next;		    
            delete cur;
            
            cur = NULL;
            cur = tmp;
        }
        head = tail = NULL;
        return TRUE;
    }
    
    /*====================================================================
    ������      : GetItemCount
    ����        : ��ȡ�ڵ���
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    u16 GetItemCount() const
    {
        return m_wItemCount;
    }
protected:		
    /*====================================================================
    ������      : FindItem
    ����        : ����ָ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const cls* data--ָ���ڵ�����
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
        ====================================================================*/ 
    CItemType* FindItem(const cls* data) const
    {
        CItemType* pcTemp = head;
        while(pcTemp)
        {
            if(pcTemp->data == data)
            {
                return pcTemp;
            }		    
            pcTemp = pcTemp->next;
        }
        return NULL;
    }	
    
    /*====================================================================
    ������      :   Push
    ����        :   �ڵ��б����ӽڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��:   CItemType* pcItem--����ڵ��б�ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    cls* Push(CItemType* pcItem)
    {
        RegItem(pcItem, tail, NULL);
        
        m_wItemCount++;
        return pcItem->data;
    }
    
    /*====================================================================
    ������      :   Pop
    ����        :   �ڵ��б�ɾ��ͷ�ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 	
    CItemType* Pop()
    {
        if(head == NULL) return NULL;
        
        CItemType* cur = head;
        UnregItem(cur);
        m_wItemCount--;
        return cur;
    } 
    
    /*====================================================================
    ������      :   RegItem
    ����        :   ע��ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��:   CItemType *pcCur--��ǰע��ڵ�
                    CItemType *pcPre--ǰһ�ڵ�
                    CItemType *pcNext--��һ�ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    void RegItem(CItemType *pcCur, CItemType *pcPre, CItemType *pcNext)
    {
        pcCur->pre = pcPre;
        pcCur->next = pcNext;
        pcCur->bIsValid = TRUE;
        if(pcPre != NULL)
        {
            pcPre->next = pcCur;
        }
        if(pcNext != NULL)
        {
            pcNext->pre = pcCur;
        } 
        
        //����Ϊhead
        if(pcNext == head)
        {
            head = pcCur;
        }
        //ǰ��Ϊtail
        if(pcPre == tail)
        {
            tail = pcCur;
        }
    }
    
    /*====================================================================
    ������      : UnregItem
    ����        : ע���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: CItemType *pcCur--��ǰע���ڵ�
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/ 
    void UnregItem(CItemType *pcCur)
    {
        pcCur->bIsValid = FALSE;
        if(pcCur == head)
        {
            if(head != tail)
            {
                head = pcCur->next;
            }
            else
            {
                tail = head = pcCur->next;
            }
        }
        else
        {
            if(pcCur == tail)
            {
                tail = pcCur->pre;
            }
        }
        
        //���pcCur == curPos,��curPos�Ƶ���һ�ڵ�
        if(curPos == pcCur)
        { 
            curPos = curPos->next;
        }
        
        if(pcCur->pre != NULL)
        {
            pcCur->pre->next = pcCur->next;
        }
        
        if(pcCur->next != NULL)
        {
            pcCur->next->pre = pcCur->pre;
        }
        pcCur->pre = NULL;
        pcCur->next = NULL;
    }
    
    CItemType* head;	//ͷ�ڵ�
    CItemType* tail;	//β�ڵ�
    CItemType* curPos;	//�б�ǰ�ڵ�
    
    SEMHANDLE m_hListSem;	//�б�����
    
    u16 m_wItemCount;	//�б�ڵ���
    
    friend class CKdmListEx<cls, wNewItem>;
};

//list��չ��,ʵ���б�ڵ��
template <class cls, u16 wNewItem = 0>
class CKdmListEx : public CKdmList<cls, wNewItem>
{
    typedef CKdmItem<cls> CItemType;
public:
    CKdmListEx()
    {
        u16 wItemCount = wNewItem;
        
        while(wItemCount > 0)
        {
            m_cItemPool.AddItem();
            wItemCount--;
        }
    }
    
    ~CKdmListEx()
    {
        this->DelAllItem();
        m_cItemPool.DelAllItem();
    }
    
    /*====================================================================
    ������      : AddItem
    ����        : ���ӽڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/
    cls* AddItem()
    {
        //���ȴӽڵ����ȡ����
        CItemType *pcItem = m_cItemPool.Pop();
        if(pcItem == NULL)
        {	//�粻���ڣ������ڵ�
            return CKdmList<cls, wNewItem>::AddItem();
        }
        
        return Push(pcItem);
    }
    
    /*====================================================================
    ������      : DelItem
    ����        : ɾ��ָ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: cls* data--ָ���ڵ�����
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/
    void DelItem(cls* data)
    {
        CItemType* pcTemp = this->head;
        while(pcTemp)
        {
            if(pcTemp->data == data)
            {
                UnregItem(pcTemp);
                this->m_wItemCount--;
                //ɾ���ڵ���ڵ��
                m_cItemPool.Push(pcTemp);
                return;
            }		    
            pcTemp = pcTemp->next;
        }
    }
    
    /*====================================================================
    ������      : DelItem
    ����        : ɾ��ָ���ڵ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: cls* data--ָ���ڵ�����
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    06/03/16    3.9         ½ѩ��        ����
    ====================================================================*/
    void Clear()
    {
        while(this->head != NULL)
        {	
            //����ɾ���ڵ���ڵ��
            DelItem(this->head->data);
        }
    }
private:
    CKdmList<cls, wNewItem> m_cItemPool;	//�ڵ��
};

template <class cls, u16 wNewItem>
void PrintList(CKdmList<cls, wNewItem>& pcList, u32 dwStart = 1, u32 dwEnd = 0xffffffff, u8 byOffsetSize = 0)
{
    OspPrintf(TRUE, FALSE, "��ǰ��:%d\n", pcList.GetItemCount());

	cls* cur = pcList.PosFirst();
	u32 i = 1;
	
    while(cur && (i <= dwEnd))
    {
        if(i >= dwStart)
		{
			OspPrintf(TRUE, FALSE, "***************************************************************\n");
			cur->PrintData(byOffsetSize);
		}

        cur = pcList.PosNext(cur);
		i++;
    }
}

#endif
