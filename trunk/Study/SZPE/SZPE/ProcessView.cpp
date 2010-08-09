// ProcessView.cpp : 实现文件
//

#include "stdafx.h"
#include "SZPE.h"
#include "ProcessView.h"


// CProcessView

IMPLEMENT_DYNAMIC(CProcessView, CDockablePane)

CProcessView::CProcessView()
{

}

CProcessView::~CProcessView()
{
}


BEGIN_MESSAGE_MAP(CProcessView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	//ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	//ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	//ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	//ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	//ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CProcessView 消息处理程序


BOOL CProcessView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CProcessView::AdjustLayout(void)
{
		if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = 0;// m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

//	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndProcessTreeList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);

}

void CProcessView::OnSetFocus( CWnd* pOldWnd )
{
	CDockablePane::OnSetFocus(pOldWnd);

	// 子窗口/控件获得焦点。
}

void CProcessView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndProcessTreeList.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CProcessView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CDockablePane::OnContextMenu(pWnd, point);

	//CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	//ASSERT_VALID(pWndTree);

	//if (pWnd != pWndTree)
	//{
	//	CDockablePane::OnContextMenu(pWnd, point);
	//	return;
	//}

	//if (point != CPoint(-1, -1))
	//{
	//	// 选择已单击的项:
	//	CPoint ptTree = point;
	//	pWndTree->ScreenToClient(&ptTree);

	//	UINT flags = 0;
	//	HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
	//	if (hTreeItem != NULL)
	//	{
	//		pWndTree->SelectItem(hTreeItem);
	//	}
	//}

	//pWndTree->SetFocus();
	//CMenu menu;
	//menu.LoadMenu(IDR_POPUP_SORT);

	//CMenu* pSumMenu = menu.GetSubMenu(0);

	//if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	//{
	//	CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

	//	if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
	//		return;

	//	((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
	//	UpdateDialogControls(this, FALSE);
	//}
}

void CProcessView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

int CProcessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();


	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHAREIMAGELISTS | LVS_OWNERDRAWFIXED | LVS_SHOWSELALWAYS;

	if (!m_wndProcessTreeList.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("未能创建类视图\n");
		return -1;      // 未能创建
	}

	// 刷新一下进程状态。
	this->m_processManage.StartAutoUpdate();

	Sleep( 200 );

	// 获取所有进程。
	TProcessIdList allProcessIdList;
	this->m_processManage.GetAllProcessId( allProcessIdList );
	for( size_t i=0; i<allProcessIdList.size(); ++i )
	{
		CProcess process ;
		m_processManage.GetProcess( allProcessIdList[i], process );

		this->m_wndProcessTreeList.InsertItem( process );
	}
	
	int nScroll;
	this->m_wndProcessTreeList.ExpandAll( m_wndProcessTreeList.GetRootItem(), nScroll );
	// Expand(GetRootItem(), 0 /*listview index 0*/); //

	//// 加载图像:
	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	//m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	//OnChangeVisualStyle();

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	//m_wndToolBar.SetOwner(this);

	//// 所有命令将通过此控件路由，而不是通过主框架路由:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//CMenu menuSort;
	//menuSort.LoadMenu(IDR_POPUP_SORT);

	//m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	//CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	//if (pButton != NULL)
	//{
	//	pButton->m_bText = FALSE;
	//	pButton->m_bImage = TRUE;
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
	//	pButton->SetMessageWnd(this);
	//}

	//// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	//FillClassView();

	return 0;
}