// GiftStatusView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KedaGift.h"
#include "GiftStatusView.h"
#include "afxdialogex.h"

#include "KedaGiftDefine.h"
#include "EmployerGiftConfig.h"

// CGiftStatusView �Ի���

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


// CGiftStatusView ��Ϣ�������


BOOL CGiftStatusView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->m_listGiftStatus.InsertColumn( ColHead_SequenceNo, _T( "���" ), 0, 40 );
	this->m_listGiftStatus.InsertColumn( ColHead_KedaNo, _T( "����" ), 0, 50 );
	this->m_listGiftStatus.InsertColumn( ColHead_Name, _T( "����" ), 0, 100 );
	this->m_listGiftStatus.InsertColumn( ColHead_Time, _T( "ʱ��" ), 0, 150 );

	// ��ʼ��.
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CGiftStatusView::OnBnClickedClearAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	CEmployerGiftConfig config;
// 	config.Reset();
	if ( IDYES == MessageBox( _T( "���б���պ�,���б��е��˾Ϳ������µõ��齱����,�Ƿ����?" ), _T( "��ʾ" ), MB_YESNO ) )
	{
		m_listGiftStatus.DeleteAllItems();

		m_pRandomPick->ResetGift();
	}	
}

void CGiftStatusView::SetRandomPicker( CRandomPick* pPicker )
{
	this->m_pRandomPick = pPicker;
}