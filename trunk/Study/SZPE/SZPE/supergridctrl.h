#if !defined(AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_)
#define AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SuperGridCtrl.h : header file
//
#include <afxtempl.h>

//Feel free to modify this class to what ever info you want in the ListCtrl
class CItemInfo
{
public:
	CItemInfo():m_bCheck(0){};
	void SetItemText(const CString& strItem){ m_strItemName=strItem; }
	void AddSubItemText(const CString& strSubItem){ m_SubItems.Add(strSubItem); }
	void SetSubItemText(int iSubItem, const CString& strSubItem){ m_SubItems.SetAtGrow(iSubItem, strSubItem); }
	const CString& GetItemText(void){ return m_strItemName; }
	CString GetSubItem(int iSubItem){ return m_SubItems.GetAt(iSubItem); }
	int GetItemCount(void) const { return m_SubItems.GetSize(); }

	enum CONTROLTYPE {edit/*default*/, combobox, datecontrol/*your control*/, spinbutton/*your control*/, dropdownlistviewwhatevercontrol/*your control*/};//no need for checkbox control hence the listview provide it for me 
	//all cols in this row is default edit
	void SetControlType(CONTROLTYPE enumCtrlType, int nCol=-1){ m_controlType.SetAt(nCol, enumCtrlType); }

	BOOL GetControlType(int nCol, CONTROLTYPE& controlType)
	{
		if(!m_controlType.Lookup(nCol,controlType))
		{
			controlType=edit;//default;
			return 0;
		}
		return 1;
	}
	//helper function used if a control needs to initalize it self with a list(combobox,listbox)
	void SetListData(CStringList *strInitArr)
	{
		m_strInitArr.RemoveAll();
		m_strInitArr.AddTail(strInitArr);
	}	
	//return list
	CStringList* GetListData(void){	return &m_strInitArr;}
	//used if LVS_EX_CHECKBOXES style
	void SetCheck(BOOL bCheck){m_bCheck=bCheck;}
	BOOL GetCheck(void){return m_bCheck;}
private:
	CONTROLTYPE m_enumCtrlType; 
	CMap<int,int, CONTROLTYPE, CONTROLTYPE&> m_controlType;
	CString m_strItemName;//col 0...
	CStringArray m_SubItems;//col 1... N
	CStringList m_strInitArr;//listbox/combobox init
	BOOL m_bCheck;
};


/////////////////////////////////////////////////////////////////////////////
//
// CSuperGridCtrl 
//
/////////////////////////////////////////////////////////////////////////////
class CSuperGridCtrl : public CListCtrl
{
// Construction
public:
	CSuperGridCtrl();
protected:
	class CTreeItem;//nested class forward decl.
public:
	// OVERRIDES
	//MUST override this to make a copy of CItemInfo..see the CMySuperGrid.cpp for implementation
	//used in drag/drop operations
	virtual CItemInfo* CopyData(CItemInfo* lpSrc);
	//sets the icon state...called from within DrawItem returns valid image index
	//You MUST override this function to set your current icon, must of course be a valid CImageList index
	virtual int GetIcon(const CTreeItem* pItem);
	//override this to set the color for current cell
	virtual COLORREF GetCellRGB(void);
	//override this to update listview items, called from within OnEndlabeledit.
	virtual void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	//override this to activate any control when lButtonDown in a cell, called from within OnLButtonDown
	virtual void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);
	//override this to provide your dragimage, default: creates an image + text
	virtual CImageList *CreateDragImageEx(int nItem);
public:
	//CREATES the "TreeCtrl"..and initalize a CItemInfo
	BOOL CreateTreeCtrl(CItemInfo* lpInfo);
	//
	CTreeItem* GetRootItem(void);
	CTreeItem* GetPrevSiblingItem(CTreeItem *pItem, BOOL bInit = TRUE, BOOL bDontIncludeHidden=TRUE);
	CTreeItem* GetNextSiblingItem(CTreeItem* pNode, BOOL bInit = TRUE, BOOL bDontIncludeHidden=TRUE);
	//
	int NumChildren(const CTreeItem *pItem) const;
	//Determines if this tree item is a child of the specified parent
	BOOL IsChildOf(const CTreeItem* pParent, const CTreeItem* pChild) const;
	//hmm
	BOOL ItemHasChildren(const CTreeItem* pItem) const;
	//hmm
	BOOL IsCollapsed(const CTreeItem* pItem) const;
	//hmm
	int GetIndent(const CTreeItem* pItem) const;
	//hmm
	void SetIndent(CTreeItem *pItem, int iIndent);
	//get the pItems current listview index, 
	int GetCurIndex(const CTreeItem *pItem) const;
	//set pItems current listview index
	void SetCurIndex(CTreeItem* pItem,int nIndex);
	//hmm set the pItems parent
	void SetParentItem(CTreeItem*pItem, CTreeItem* pParent);
	//gets pItems parent
	CTreeItem* GetParentItem(const CTreeItem* pItem); 
	//return ptr to CItemInfo daaa
	CItemInfo* GetData(const CTreeItem* pItem); 
	//sets the CItemInfo ptr of pItem
	void UpdateData(CTreeItem* pItem, CItemInfo* lpInfo);
	//Insert item and return new parent node.
	//the bUpdate is here for performance reasons, when you insert say 100 node each having 10 children(1000 listview items)
	//the bUpdate should be set to FALSE(default) but when you want to insert an item, and you want to user to see it right away
	//set bUpdate to TRUE.(see the use of bUpdate in the HowToInsertItemsAfterTheGridHasBeenInitialized function in the CMySuperGridCtrl)
	CTreeItem* InsertItem(CTreeItem *pParent, CItemInfo* lpInfo, BOOL bUpdate=0);
	//collapse all children from pItem
	void Collapse(CTreeItem *pItem);
	//expand one folder and return the last index of the expanded folder
	int Expand(CTreeItem* pSelItem, int nIndex);
	//expand all items from pItem and return last index of the expanded folder
	void ExpandAll(CTreeItem *pItem, int& nScroll);
	//expand all node in pItem and stop at pStopAt
	void ExpandUntil(CTreeItem *pItem, CTreeItem* pStopAt);
	//use this if you want to select a node 
	//if the node is collapsed all items with in the node are expanded and the node is selected
	//it returns the listview index for the selected node
	int SelectNode(CTreeItem *pLocateNode);
	//Delete an item in the listview 
	//takes the node to be delete and its listview item index as arg.
	//note the item you delete must be visible, hence the nItem as arg. 
	void DeleteItemEx(CTreeItem *pSelItem, int nItem);
	//when VC5.0
	void IE4StyleEx(DWORD dwNewStyle);
	//hmm
	int GetNumCol(void);
	//does a Quicksort of the pParents children and if bSortChildren set, 
	//all items from pParent are sorted. 
	void Sort(CTreeItem* pParent, BOOL bSortChildren);
	// simpel wrapper for the CObList in CTreeItem
	POSITION GetHeadPosition(CTreeItem* pItem) const;
	POSITION GetTailPosition(CTreeItem *pItem) const;
	CTreeItem* GetNextChild(CTreeItem *pItem, POSITION& pos) const;
	CTreeItem* GetPrevChild(CTreeItem *pItem, POSITION& pos) const;
	void AddChild(CTreeItem *pParent, CTreeItem *pChild);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperGridCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CSuperGridCtrl();
protected:
	//delete pItem and all of its children
	BOOL Delete(CTreeItem *pItem, BOOL bClean=TRUE/*delete itemdata*/);
	//used in drag/drop operation..
	void CopyChildren(CTreeItem* pDest, CTreeItem* pSrc);
	//drag/drop thing....clipboard not supported.....YET!
	BOOL DoDragDrop(CTreeItem* pTarget, int nDragTarget, CTreeItem* pSelItem, int nDragItem);
	//updates internal nodes, called when ever insert,delete on listview
	void InternaleUpdateTree(void);
	//Get ListView Index from pNode
	int NodeToIndex(CTreeItem *pNode);
	//see if user clicked the [+] [-] sign, also handles LVS_EX_CHECKBOXES.
	BOOL HitTestOnSign(CPoint point, LVHITTESTINFO& ht);
	//positions an edit-control and creates it
	CEdit* EditLabelEx(int nItem, int nCol);
	int m_cxImage, m_cyImage;//icon size
	//translates column index value to a column order value.
	int IndexToOrder(int iIndex);
	//set hideflag for pItem
	void Hide(CTreeItem* pItem, BOOL bFlag);
	int GetCurSubItem(void){return m_CurSubItem;}
	#if _MSC_VER < 1200 //arrgghh
	BOOL GetCheck(int nItem) const;
	BOOL SetCheck(int nItem, BOOL fCheck = TRUE);
	#endif
private:
	CTreeItem *m_pRoot; 
	//internal helpers
	BOOL _Delete(CTreeItem* pStartAt, CTreeItem *pItem, BOOL bClean=TRUE/*delete itemdata*/);
	CTreeItem* _GetPrev(CTreeItem* pStartAt, CTreeItem *pItem, BOOL bInit = TRUE, BOOL bDontIncludeHidden=TRUE);
	CTreeItem* _GetNext(CTreeItem* pStartAt, CTreeItem* pNode, BOOL bInit = TRUE, BOOL bDontIncludeHidden=TRUE);
	//hmm
	void DrawTreeItem(CDC* pDC, CTreeItem* pSelItem, int nListItem, const CRect& rcBounds);
	//makes the dot ... thing
	LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
	//set the hideflag from pItem and all its children
	void HideChildren(CTreeItem *pItem, BOOL bHide, int iIndent);
	//checks whether a column is visible, if not scrolls to it
	void MakeColumnVisible(int nCol);
	//hmm
	void DrawFocusCell(CDC *pDC, int nItem, int iSubItem);
	//draw the down arrow if any combobox precent in the listview item
	void DrawComboBox(CDC *pDC, CTreeItem *pSelItem, int nItem, int nColumn);
	int m_CurSubItem;//keyboard handling..it is what it says
	//hmm these represents the state of my expresso machine
    int m_nDragItem, m_nDragTarget;
    BOOL m_bIsDragging;
	//helper compare fn used with Quicksort
	static int CompareChildren(const void* p1, const void* p2);
	//so we can access the m_pRoot ptr and other stuff
	friend class CTreeItem;
	//
	void CleanMe(CTreeItem *pItem);
	//
	int _NodeToIndex(CTreeItem *pStartpos, CTreeItem* pNode, int& nIndex, BOOL binit = TRUE);
	// Generated message map functions
protected:
	//{{AFX_MSG(CSuperGridCtrl)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);	
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//the nested class 
class CSuperGridCtrl::CTreeItem : public CObject
{
	CTreeItem(): m_pParent(NULL), m_bHideChildren(0), m_nIndex(-1), m_nIndent(-1){};
	~CTreeItem();
	CObList m_listChild;
	CTreeItem* m_pParent;
	CItemInfo* m_lpNodeInfo;
	BOOL m_bHideChildren;  
	int m_nIndex; //CListCtrl index
	int m_nIndent; 
	friend class CSuperGridCtrl;
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_)
