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
END_MESSAGE_MAP()


// CGiftStatusView 消息处理程序


BOOL CGiftStatusView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_listGiftStatus.InsertColumn( ColHead_SequenceNo, _T( "序号" ), 0, 40 );
	this->m_listGiftStatus.InsertColumn( ColHead_KedaNo, _T( "工号" ), 0, 50 );
	this->m_listGiftStatus.InsertColumn( ColHead_Name, _T( "姓名" ), 0, 100 );
	this->m_listGiftStatus.InsertColumn( ColHead_Time, _T( "时间" ), 0, 150 );

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
		CString strTime = allGifted[i].m_time.Format( "%y/%m/%d-%H:%M:%S" );
		m_listGiftStatus.SetItemText( nItem, ColHead_Time, strTime );
	}

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