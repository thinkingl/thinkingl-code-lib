/*****************************************************************************
工程名      : KTM2.0
模块名      : CMU
文件名      : kdmlist.h
相关文件    : 
文件实现功能: 
作者        : luxz
版本        : V1.0  Copyright(C) 1997-2005 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期        版本        修改人      修改内容
2006/02/19    1.0         luxz       创建
******************************************************************************/
#ifndef _KDM_LIST_H_
#define _KDM_LIST_H_

template <class cls>
class CKdmItem
{	//节点类
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
    cls* data;	//保存数据
    BOOL32 bIsValid;	//是否有效
    CKdmItem* next;	//下节点
    CKdmItem* pre;	//上节点
};

template <class cls, u16 wNewItem> class CKdmListEx;

template <class cls, u16 wNewItem = 0>
class CKdmList
{	//list基类
// public:
//     typedef struct CKdmItem
//     {	//节点类
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
//         cls* data;	//保存数据
//         BOOL32 bIsValid;	//是否有效
//         CKdmItem* next;	//下节点
//         CKdmItem* pre;	//上节点
//     } CKdmItem;
    typedef CKdmItem<cls> CItemType;
public:
    CKdmList() : head(NULL), tail(NULL), curPos(NULL), m_wItemCount(0)
    {
        OspSemBCreate(&m_hListSem);
    }
    
    /*====================================================================
    函数名      : Lock
    功能        : list加锁
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/
    void Lock()
    {	//加锁
        OspSemTake(m_hListSem);
    }
    
    /*====================================================================
    函数名      : Unlock
    功能        : list解锁
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/
    void Unlock()
    {	//解锁
        OspSemGive(m_hListSem);
    }
    
    virtual ~CKdmList()
    {
        DelAllItem();	//删除所有节点
        OspSemDelete(m_hListSem);	//删除锁
    }
    
    /*====================================================================
    函数名      : PrintList
    功能        : 打印所有节点信息
    算法实现    : 
    引用全局变量: 
    输入参数说明: u8 byOffsetSize-打印偏移tab数
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
//         OspPrintf(TRUE, FALSE, "当前数:%d\n", GetItemCount());
//     }
    
    /*====================================================================
    函数名      : AddItem
    功能        : 增加节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      : DelItem
    功能        : 删除节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      : GetNextItem
    功能        : 获取指定节点的下一节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: const cls* curData-指定节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      : GetPreItem
    功能        : 获取指定节点的上一节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: const cls* curData-指定节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      : SetCurPos
    功能        : 设置列表的当前节点为指定节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: const cls* curData-指定节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/ 
    void SetCurPos(const cls* curData)
    {		
        if(curPos!=NULL&&curData==curPos->data)
        {	//如果当前节点为指定的节点
            return;
        }
        if(curPos!=NULL&&curPos->next!=NULL&&curData==curPos->next->data)
        {	//指定节点为当前节点下节点
            curPos=curPos->next;
            return;
        }
        if(curPos!=NULL&&curPos->pre!=NULL&&curData==curPos->pre->data)
        {	//指定节点为当前节点上节点
            curPos=curPos->pre;
            return;
        }
        
        //否则查找指定节点
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
    函数名      : SetNextPos
    功能        : 设置列表的当前节点为指定节点的下一节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: const cls* curData-指定节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         王冬林        创建
    ====================================================================*/ 
    cls* SetNextPos(const cls* curData)
    {		
        if(curPos!=NULL&&curData==curPos->data)
        {	//如果当前节点为指定的节点
			curPos=curPos->next;
            return curPos == NULL ? NULL : curPos->data;
        }
        if(curPos!=NULL&&curPos->next!=NULL&&curData==curPos->next->data)
        {	//指定节点为当前节点下节点
            curPos=curPos->next;
			curPos=curPos->next;
            return curPos == NULL ? NULL : curPos->data;
        }
        if(curPos!=NULL&&curPos->pre!=NULL&&curData==curPos->pre->data)
        {	//指定节点为当前节点上节点
            curPos=curPos->pre;
			curPos=curPos->next;
            return curPos == NULL ? NULL : curPos->data;
        }
        
        //否则查找指定节点
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
    函数名      : GetCurItem
    功能        : 获取指定节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: const cls* curData-指定节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/ 
    CItemType* GetCurItem(const cls* curData) const
    {
        return FindItem(curData);
    }
    
    /*====================================================================
    函数名      : PosFirst
    功能        : 获取第一个节点
    算法实现    : 
    引用全局变量: 
    输入参数说明:
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/ 
    cls*  PosFirst()
    {
        curPos = head;
        return curPos == NULL ? NULL : curPos->data;
    }
    
    /*====================================================================
    函数名      : PosNext
    功能        : 获取指定节点的下一个节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: cls* pcCur--指定节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      : PosPre
    功能        : 获取指定节点的上一个节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: cls* pcCur--指定节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      : PosLast
    功能        : 获取列表最后节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/ 
    cls* PosLast()
    {
        curPos = tail;
        
        return curPos == NULL ? NULL : curPos->data;
    }
    
    /*====================================================================
    函数名      : PosCur
    功能        : 获取列表当前节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/ 
    cls* PosCur()
    {
        return curPos == NULL ? NULL : curPos->data;
    }
    
    /*====================================================================
    函数名      : IsEmpty
    功能        : 列表是否为空
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/ 
    BOOL32 IsEmpty()
    {
        return head == NULL;
    }
    
    /*====================================================================
    函数名      : DelAllItem
    功能        : 清空列表
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      : GetItemCount
    功能        : 获取节点数
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/ 
    u16 GetItemCount() const
    {
        return m_wItemCount;
    }
protected:		
    /*====================================================================
    函数名      : FindItem
    功能        : 查找指定节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: const cls* data--指定节点数据
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      :   Push
    功能        :   节点列表增加节点
    算法实现    : 
    引用全局变量: 
    输入参数说明:   CItemType* pcItem--加入节点列表节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/ 
    cls* Push(CItemType* pcItem)
    {
        RegItem(pcItem, tail, NULL);
        
        m_wItemCount++;
        return pcItem->data;
    }
    
    /*====================================================================
    函数名      :   Pop
    功能        :   节点列表删除头节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
    函数名      :   RegItem
    功能        :   注册节点
    算法实现    : 
    引用全局变量: 
    输入参数说明:   CItemType *pcCur--当前注册节点
                    CItemType *pcPre--前一节点
                    CItemType *pcNext--后一节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
        
        //后续为head
        if(pcNext == head)
        {
            head = pcCur;
        }
        //前续为tail
        if(pcPre == tail)
        {
            tail = pcCur;
        }
    }
    
    /*====================================================================
    函数名      : UnregItem
    功能        : 注销节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: CItemType *pcCur--当前注销节点
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
        
        //如果pcCur == curPos,则curPos移到下一节点
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
    
    CItemType* head;	//头节点
    CItemType* tail;	//尾节点
    CItemType* curPos;	//列表当前节点
    
    SEMHANDLE m_hListSem;	//列表互斥量
    
    u16 m_wItemCount;	//列表节点数
    
    friend class CKdmListEx<cls, wNewItem>;
};

//list扩展类,实现列表节点池
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
    函数名      : AddItem
    功能        : 增加节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/
    cls* AddItem()
    {
        //首先从节点池中取数据
        CItemType *pcItem = m_cItemPool.Pop();
        if(pcItem == NULL)
        {	//如不存在，则分配节点
            return CKdmList<cls, wNewItem>::AddItem();
        }
        
        return Push(pcItem);
    }
    
    /*====================================================================
    函数名      : DelItem
    功能        : 删除指定节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: cls* data--指定节点数据
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
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
                //删除节点入节点池
                m_cItemPool.Push(pcTemp);
                return;
            }		    
            pcTemp = pcTemp->next;
        }
    }
    
    /*====================================================================
    函数名      : DelItem
    功能        : 删除指定节点
    算法实现    : 
    引用全局变量: 
    输入参数说明: cls* data--指定节点数据
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    06/03/16    3.9         陆雪忠        创建
    ====================================================================*/
    void Clear()
    {
        while(this->head != NULL)
        {	
            //所有删除节点入节点池
            DelItem(this->head->data);
        }
    }
private:
    CKdmList<cls, wNewItem> m_cItemPool;	//节点池
};

template <class cls, u16 wNewItem>
void PrintList(CKdmList<cls, wNewItem>& pcList, u32 dwStart = 1, u32 dwEnd = 0xffffffff, u8 byOffsetSize = 0)
{
    OspPrintf(TRUE, FALSE, "当前数:%d\n", pcList.GetItemCount());

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
