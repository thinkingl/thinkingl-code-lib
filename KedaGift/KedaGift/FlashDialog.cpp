// FlashDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "KedaGift.h"
#include "FlashDialog.h"
#include "afxdialogex.h"

#include <vector>

// CFlashDialog 对话框

IMPLEMENT_DYNAMIC(CFlashDialog, CDialogEx)

CFlashDialog::CFlashDialog(CWnd* pParent, CRandomPick* pRandomPic )
	: CDialogEx(CFlashDialog::IDD, pParent)
{
	this->m_pRandomPick = pRandomPic;
	this->m_eState = State_Ready;
}

CFlashDialog::~CFlashDialog()
{
}

void CFlashDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH_BACKGROUND, m_flashBackground);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH_NUM_FIRST, m_flashNumberFirst);
	DDX_Control(pDX, ID_TEST, m_btnTest);
	DDX_Control(pDX, IDC_STATIC_PIC_K, m_picK);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH_NUM_SECOND, m_flashNum2);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH_NUM_THIRD, m_flashNum3);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH_NUM_4TH, m_flashNum4);
	DDX_Control(pDX, IDC_STATIC_PIC_D, m_picD);
	DDX_Control(pDX, IDC_STATIC_PIC_NUM_1, m_picNum1);
	DDX_Control(pDX, IDC_STATIC_PIC_NUM_2, m_picNum2);
	DDX_Control(pDX, IDC_STATIC_PIC_NUM_3, m_picNum3);
	DDX_Control(pDX, IDC_STATIC_PIC_NUM_4, m_picNum4);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
}


BEGIN_MESSAGE_MAP(CFlashDialog, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_TEST, &CFlashDialog::OnBnClickedTest)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_MESSAGE( WM_REFRESH_EMPLOY, &CFlashDialog::OnEmployRefresh)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CFlashDialog 消息处理程序


BOOL CFlashDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strCurrentDir;
	LPSTR lpBufCurDir = strCurrentDir.GetBuffer( MAX_PATH * 2 );
	::GetCurrentDirectory( MAX_PATH*2, lpBufCurDir );
	strCurrentDir.ReleaseBuffer();
	strCurrentDir += _T( "\\" );

	LPCTSTR FLASH_BACKGROUND_FILE_NAME =  _T( "defBG.swf" );
	LPCTSTR FLASH_NUMBER_FILE_NAME = _T( "defsym.swf" );

	CRect rcScreen;
	CWnd::GetDesktopWindow()->GetWindowRect( rcScreen );
	this->MoveWindow( rcScreen );

	// TODO:  在此添加额外的初始化
//	this->m_flashBackground.put_Movie( strCurrentDir + FLASH_BACKGROUND_FILE_NAME );

// 	this->m_tAllNumberFlashControls.push_back( &m_flashNumberFirst );
// 	this->m_tAllNumberFlashControls.push_back( &m_flashNum2 );
// 	this->m_tAllNumberFlashControls.push_back( &m_flashNum3 );
// 	this->m_tAllNumberFlashControls.push_back( &m_flashNum4 );

// 	for ( int i=0; i<m_tAllNumberFlashControls.size(); ++i )
// 	{
// 		m_tAllNumberFlashControls[i]->put_Movie( strCurrentDir + FLASH_NUMBER_FILE_NAME );
// 		m_tAllNumberFlashControls[i]->StopPlay();
// 	}
	
	this->m_picK.SetPicture( strCurrentDir + "KedaGiftTextK.png" );
	this->m_picD.SetPicture( strCurrentDir + "KedaGiftTextD.png" );

	this->m_tAllNumberFlashControls.push_back( &m_picNum1 );
	this->m_tAllNumberFlashControls.push_back( &m_picNum2 );
	this->m_tAllNumberFlashControls.push_back( &m_picNum3 );
	this->m_tAllNumberFlashControls.push_back( &m_picNum4 );
	for ( int i=0; i<m_tAllNumberFlashControls.size(); ++i )
	{
		m_tAllNumberFlashControls[i]->SetPicture( strCurrentDir + "KedaGiftTextNull.png" );
	}

//	this->SetTimer( Timer_Refresh, 1, 0 );
	::SetTimer( this->GetSafeHwnd(), Timer_Refresh, 10, CFlashDialog::TimerCB );

	this->SetBackgroundColor( RGB( 255,255,255 ) );

	this->m_fontShowName.CreateFont(
		100,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("楷体"));                 // lpszFacename


	this->m_staticName.SetFont( &m_fontShowName );


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFlashDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ( this->GetSafeHwnd() 
		&& this->m_flashBackground.GetSafeHwnd() 
		&& this->m_flashNumberFirst.GetSafeHwnd() )
	{
		int nBottom = cy - 60;

		CRect rcBackGround( 0, 0, cx, nBottom );
		this->m_flashBackground.MoveWindow( rcBackGround );

		CRect rcButton;
		this->m_btnTest.GetWindowRect( rcButton );

		CRect rcNewButton;
		rcNewButton = rcButton;
		rcNewButton.bottom = cy - 10;
		rcNewButton.top = rcNewButton.bottom - rcButton.Height();
		this->m_btnTest.MoveWindow( rcNewButton );

		// 6个文字一行排开.
		typedef std::vector< CWnd* > TWndList;
		TWndList tAllWnd;
		tAllWnd.push_back( &m_picK );
		tAllWnd.push_back( &m_picD );

		for ( int i=0; i<m_tAllNumberFlashControls.size(); ++i )
		{
			tAllWnd.push_back( m_tAllNumberFlashControls[i] );
		}

		const int NUMBER_HEIGHT = 365 / 2;
		const int NUMBER_WIDTH = 280 / 2;	// 340

		int BLANK_SIDE = 200;	// 边上留的空白.

		int nDistance = ( cx - BLANK_SIDE * 2 ) / tAllWnd.size() - NUMBER_WIDTH;
		int nTop = float( cy ) * 0.3;
		for ( int i=0; i<tAllWnd.size(); ++i )
		{
			int nLeft = BLANK_SIDE + i*( NUMBER_WIDTH + nDistance );
			CRect rcFirstNum( nLeft, nTop, nLeft+ NUMBER_WIDTH, nTop + NUMBER_HEIGHT );			
			tAllWnd[i]->MoveWindow( rcFirstNum );
		}

		CRect rcName( 0, nTop + 200, cx, cy - 200 );;
		this->m_staticName.MoveWindow( rcName );

		
	}

	// TODO: 在此处添加消息处理程序代码
}


void CFlashDialog::OnBnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
// 	long nNum = this->m_flashNumberFirst.get_TotalFrames ();
// 
// 	static int s_frameindex = 0;
// 	s_frameindex ++;
// 	s_frameindex %= nNum;
// 
// 	m_flashNumberFirst.GotoFrame( s_frameindex );
	int dfwef = 3;
}
BEGIN_EVENTSINK_MAP(CFlashDialog, CDialogEx)
	ON_EVENT(CFlashDialog, IDC_SHOCKWAVEFLASH_BACKGROUND, 197, CFlashDialog::FlashCallShockwaveflashBackground, VTS_BSTR)
	ON_EVENT(CFlashDialog, IDC_SHOCKWAVEFLASH_BACKGROUND, 150, CFlashDialog::FSCommandShockwaveflashBackground, VTS_BSTR VTS_BSTR)
	ON_EVENT(CFlashDialog, IDC_SHOCKWAVEFLASH_BACKGROUND, 1958, CFlashDialog::OnProgressShockwaveflashBackground, VTS_I4)
	ON_EVENT(CFlashDialog, IDC_SHOCKWAVEFLASH_BACKGROUND, DISPID_READYSTATECHANGE, CFlashDialog::OnReadyStateChangeShockwaveflashBackground, VTS_I4)
END_EVENTSINK_MAP()


void CFlashDialog::FlashCallShockwaveflashBackground(LPCTSTR request)
{
	// TODO: 在此处添加消息处理程序代码
}


void CFlashDialog::FSCommandShockwaveflashBackground(LPCTSTR command, LPCTSTR args)
{
	// TODO: 在此处添加消息处理程序代码
}


void CFlashDialog::OnProgressShockwaveflashBackground(long percentDone)
{
	// TODO: 在此处添加消息处理程序代码
}


void CFlashDialog::OnReadyStateChangeShockwaveflashBackground(long newState)
{
	// TODO: 在此处添加消息处理程序代码
}


void CFlashDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	this->GoOrStop();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CFlashDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

//	this->GoOrStop();

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CFlashDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYDOWN
		/*|| pMsg->message == WM_LBUTTONDOWN */)
	{
		this->GoOrStop();
		OutputDebugString( "!!!!!!!!!!!!!CFlashDialog::PreTranslateMessage\n" );
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CFlashDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( Timer_Refresh == nIDEvent )
	{
		

	
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CFlashDialog::ShowAEmployer( const CEmployer& employer )
{
	CString strKedaNo = employer.m_strKedaNo;
	_ASSERT( strKedaNo.GetLength() == 6 );	// 只能处理 KD1234 这样的工号.

	this->m_staticName.SetWindowText( employer.m_strName );
	this->m_staticName.Invalidate();

	CRect rcName;
	this->m_staticName.GetWindowRect( rcName );
	rcName.top -= 30;
	this->InvalidateRect( rcName );

	for ( int i=0; i<4; ++i )
	{
		int nNum = strKedaNo.GetAt( i+2 ) - '0';
//		int nFrame = nNum + 3;
		// m_tAllNumberFlashControls[i]->GotoFrame( nFrame );

//  		CString strMov = m_tAllNumberFlashControls[i]->get_Movie();
// // 		m_tAllNumberFlashControls[i]->put_Movie( strMov );
//  		m_tAllNumberFlashControls[i]->GotoFrame( nFrame );
// 		m_tAllNumberFlashControls[i]->Invalidate();

 		CString strCurrentDir;
 		LPSTR lpBufCurDir = strCurrentDir.GetBuffer( MAX_PATH * 2 );
 		::GetCurrentDirectory( MAX_PATH*2, lpBufCurDir );
 		strCurrentDir.ReleaseBuffer();
 		strCurrentDir += _T( "\\" );
		CString strPath;
		strPath.Format( _T( "%sKedaGiftText%d.png" ), strCurrentDir, nNum );

		m_tAllNumberFlashControls[i]->SetPicture( strPath );

		m_tAllNumberFlashControls[i]->Invalidate( FALSE );
	}
//	this->Invalidate();
}

void CFlashDialog::GoOrStop()
{
	// 防止段时间误操作..
	static CTime s_timeLastOp = CTime::GetCurrentTime();
	CTime timeNow = CTime::GetCurrentTime();
	CTimeSpan timeSpan = timeNow - s_timeLastOp;

	if ( timeSpan.GetTotalSeconds() < 2 )
	{
		return;
	}
	else
	{
		s_timeLastOp = timeNow;
	}
	
	

	if ( m_eState == State_Ready )
	{
		m_eState = State_Running;
		
// 		for ( int i=0; i<m_tAllNumberFlashControls.size(); ++i )
// 		{
// 			CString strMov = m_tAllNumberFlashControls[i]->get_Movie();
// 			m_tAllNumberFlashControls[i]->GotoFrame( 0 );
// 		}
	}
	else
	{

		m_eState = State_Ready;

		// 刷新.....
		CEmployer randomShowEmployer = this->m_pRandomPick->RandomPickOneNoGiftToShow();

		this->ShowAEmployer( randomShowEmployer );
	}
}

void CALLBACK CFlashDialog::TimerCB(HWND hWnd, UINT , UINT_PTR timerId, DWORD de)
{
//	if ( Timer_Refresh == timerId )
	{
		::PostMessage( hWnd, WM_REFRESH_EMPLOY, 0, 0 );
	}

}

LRESULT CFlashDialog::OnEmployRefresh( WPARAM, LPARAM )
{
	// 如果需要刷新,则刷新显示.....
	if ( State_Running == m_eState )
	{
		CEmployer oneMan = this->m_pRandomPick->RandomPickOneNoGiftToShow();
		this->ShowAEmployer( oneMan );

		static int s_count = 0;
		s_count ++;
		CString strMsg;
		strMsg.Format( "++%d+++++++++++++++++++++++\n", s_count );
		OutputDebugString( strMsg );
		//		this->m_flashNum2.Forward();
		// 			static int s_ttt = 0;
		// 			s_ttt ++;
		// 
		// 			CString strCurrentDir;
		// 			LPSTR lpBufCurDir = strCurrentDir.GetBuffer( MAX_PATH * 2 );
		// 			::GetCurrentDirectory( MAX_PATH*2, lpBufCurDir );
		// 			strCurrentDir.ReleaseBuffer();
		// 			strCurrentDir += _T( "\\" );

		// 			if ( s_ttt % 2 )
		// 			{
		// 				this->m_picK.SetPicture( strCurrentDir + "KedaGiftTextK.png" );
		// 			}
		// 			else
		// 			{
		// 				this->m_picK.SetPicture( strCurrentDir + "KedaGiftTextD.png" );
		// 			}
	}
	return 0;
}

void CFlashDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->GoOrStop();

	CDialogEx::OnLButtonDblClk(nFlags, point);
}
