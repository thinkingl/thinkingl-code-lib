// GiftStatusView.cpp : 实现文件
//

#include "stdafx.h"
#include "KedaGift.h"
#include "GiftStatusView.h"
#include "afxdialogex.h"

#include "KedaGiftDefine.h"
#include "EmployerGiftConfig.h"

// CGiftStatusView 对话框

IMPLEMENT_DYNAMIC(CGiftStatusView, CDialogEx)

CGiftStatusView::CGiftStatusView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGiftStatusView::IDD, pParent)
{
	this->m_pRandomPick = NULL;
}

CGiftStatusView::~CGiftStatusView()
{
}

void CGiftStatusView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GIFT, m_listGiftStatus);
}


BEGIN_MESSAGE_MAP(CGiftStatusView, CDialogEx)
	ON_BN_CLICKED(ID_CLEAR_ALL, &CGiftStatusView::OnBnClickedClearAll)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_GIFT, &CGiftStatusView::OnNMRClickListGift)
	ON_COMMAND(ID_GET_GIFT, &CGiftStatusView::OnGetGift)
END_MESSAGE_MAP()


// CGiftStatusView 消息处理程序


BOOL CGiftStatusView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_listGiftStatus.InsertColumn( ColHead_SequenceNo, _T( "序号" ), 0, 40 );
	this->m_listGiftStatus.InsertColumn( ColHead_KedaNo, _T( "工号" ), 0, 50 );
	this->m_listGiftStatus.InsertColumn( ColHead_Name, _T( "姓名" ), 0, 100 );
	this->m_listGiftStatus.InsertColumn( ColHead_Time, _T( "中奖时间" ), 0, 150 );
	this->m_listGiftStatus.InsertColumn( ColHead_GetGift, _T( "已领奖" ), 0, 50 );
	this->m_listGiftStatus.InsertColumn( ColHead_Absent, _T( "缺席" ), 0, 50 );

	// 初始化.
	CEmployerGiftConfig config;
	TEmployerList allGifted = config.GetLuckies();

	for ( int i=0; i<allGifted.size(); ++i )
	{
		CString strSn;
		strSn.Format( _T( "%d" ), i+1 );
		int nItem = m_listGiftStatus.InsertItem( 0, strSn );
		m_listGiftStatus.SetItemText( nItem, ColHead_KedaNo, allGifted[i].m_strKedaNo );
		m_listGiftStatus.SetItemText( nItem, ColHead_Name, allGifted[i].m_strName );
		CString strTime = allGifted[i].m_timeLuck.Format( "%y/%m/%d-%H:%M:%S" );
		m_listGiftStatus.SetItemText( nItem, ColHead_Time, strTime );

		CString strGetGift = allGifted[i].m_timeGetGift.GetTime() == 0 ? _T( "" ) : _T( "是" );
		m_listGiftStatus.SetItemText( nItem, ColHead_GetGift, strGetGift );

		CString strIsAbsent = allGifted[i].m_bAbsent ? "是" : "" ;
		this->m_listGiftStatus.SetItemText( nItem, ColHead_Absent, strIsAbsent );
	}

	m_listGiftStatus.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CGiftStatusView::OnBnClickedClearAll()
{
	// TODO: 在此添加控件通知处理程序代码
// 	CEmployerGiftConfig config;
// 	config.Reset();
	if ( IDYES == MessageBox( _T( "获奖列表清空后,获奖列表中的人就可以重新得到抽奖机会,是否继续?" ), _T( "提示" ), MB_YESNO ) )
	{
		m_listGiftStatus.DeleteAllItems();

		m_pRandomPick->ResetGift();
	}	
}

void CGiftStatusView::SetRandomPicker( CRandomPick* pPicker )
{
	this->m_pRandomPick = pPicker;
}

void CGiftStatusView::OnNMRClickListGift(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	POSITION ps = this->m_listGiftStatus.GetFirstSelectedItemPosition();
	if ( ps )
	{
		int nSelItem = this->m_listGiftStatus.GetNextSelectedItem( ps );

		CString strSN = this->m_listGiftStatus.GetItemText( nSelItem, 0 );
		if ( !strSN.IsEmpty() )
		{
			CMenu giftMenu;
			giftMenu.LoadMenu( IDR_MENU_GIFT );
			CMenu *menuPop = giftMenu.GetSubMenu( 0 );
			POINT pt;
			::GetCursorPos(&pt);
			menuPop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

			giftMenu.DestroyMenu();
		}
	}

	*pResult = 0;
}


void CGiftStatusView::OnGetGift()
{
	// TODO: 在此添加命令处理程序代码

	// 选中的人领奖.....
	CString strKedaNO;
	POSITION ps = this->m_listGiftStatus.GetFirstSelectedItemPosition();
	if ( ps )
	{
		int nSelItem = this->m_listGiftStatus.GetNextSelectedItem( ps );

		CString strKedaNO = this->m_listGiftStatus.GetItemText( nSelItem, ColHead_KedaNo );

		CEmployerGiftConfig config;
		CEmployer manGetGift;
		BOOL bOk = config.GetEmployerLucky( strKedaNO, manGetGift );
		_ASSERT( bOk );
		if ( bOk )
		{
			manGetGift.m_timeGetGift = CTime::GetCurrentTime();
			config.AddLuckMan( manGetGift );
		}

		// 更新显示..
		this->m_listGiftStatus.SetItemText( nSelItem, ColHead_GetGift, _T( "是" ) );

	}
}
