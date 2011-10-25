
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "DigitalImageProcessing.h"
#include "DigitalImage.h"
#include "MFCPropertyGridDialogProperty.h"

#include "HistogramDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_pPropFilePath = 0;
	m_pPropFileLength = 0;
	m_pPropImageType = 0;
	m_pPropImageIntensiveLevel = 0;
	m_pPropImageBPP = 0;

	m_pPropHistogram = 0;
	m_pHistogramDialog = 0;

}

CPropertiesWnd::~CPropertiesWnd()
{
	if ( m_pHistogramDialog&& NULL == m_pHistogramDialog->GetSafeHwnd() )
	{
		delete m_pHistogramDialog;
		m_pHistogramDialog = 0;
	}
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
// 	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
// 	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
// 	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
// 	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 消息处理程序

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

//	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = 0;
	if ( m_wndToolBar.GetSafeHwnd() )
	{
		cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
		m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	}
	

//	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

// 	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
// 	{
// 		TRACE0("未能创建属性组合 \n");
// 		return -1;      // 未能创建
// 	}

// 	m_wndObjectCombo.AddString(_T("应用程序"));
// 	m_wndObjectCombo.AddString(_T("属性窗口"));
// 	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}
	
	this->InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

// void CPropertiesWnd::OnProperties1()
// {
// 	// TODO: 在此处添加命令处理程序代码
// }
// 
// void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
// {
// 	// TODO: 在此处添加命令更新 UI 处理程序代码
// }
// 
// void CPropertiesWnd::OnProperties2()
// {
// 	// TODO: 在此处添加命令处理程序代码
// }
// 
// void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
// {
// 	// TODO: 在此处添加命令更新 UI 处理程序代码
// }

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroupFile = new CMFCPropertyGridProperty(_T("文件"));

	m_pPropFilePath = new CMFCPropertyGridProperty( _T( "路径" ), _T( "" ), _T( "原始图片文件的完整路径" ) );
	m_pPropFilePath->AllowEdit( FALSE );
	pGroupFile->AddSubItem( m_pPropFilePath );

	m_pPropFileLength = new CMFCPropertyGridProperty( _T( "长度" ), _T( "" ), _T( "原始图片文件的长度" ) );
	m_pPropFileLength->AllowEdit( FALSE );
	pGroupFile->AddSubItem( m_pPropFileLength );
	
	m_wndPropList.AddProperty( pGroupFile );


	CMFCPropertyGridProperty* pGroupImage = new CMFCPropertyGridProperty(_T("图片"));

	this->m_pPropImageType = new CMFCPropertyGridProperty( _T( "类型" ), _T( "" ), _T( "图片的类型 RGB 或 Gray " ) );
	this->m_pPropImageType->AllowEdit( FALSE );
	pGroupImage->AddSubItem( m_pPropImageType );

	this->m_pPropImageIntensiveLevel = new CMFCPropertyGridProperty( _T( "灰度阶数" ), _T( "" ), _T( "图片灰度的阶数(1-8) RGB彩色图片指每个原色的灰度阶数." ) );
	m_pPropImageIntensiveLevel->AllowEdit( FALSE );
	pGroupImage->AddSubItem( m_pPropImageIntensiveLevel );

	this->m_pPropImageBPP = new CMFCPropertyGridProperty( _T( "BPP" ), _T( "" ), _T( "Bits Per Pixel. 每像素占用字节数." ) );
	m_pPropImageBPP->AllowEdit( FALSE );
	pGroupImage->AddSubItem( m_pPropImageBPP );

	m_pHistogramDialog = new CHistogramDialog();
	this->m_pPropHistogram = new CMFCPropertyGridDialogProperty( _T("Histogram"), m_pHistogramDialog, CHistogramDialog::IDD, 
		0, _T("Histogram of the image. 图像的亮度直方图") );
	pGroupImage->AddSubItem( m_pPropHistogram );

	m_wndPropList.AddProperty( pGroupImage );
// 
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
//	m_wndObjectCombo.SetFont(&m_fntPropList);
}

void CPropertiesWnd::UpdateProperties( CDigitalImage * pImg )
{
	ASSERT( pImg );

	ASSERT( m_pPropFilePath );
	if ( m_pPropFilePath )
	{
		tstring strPath = pImg->GetFilePath();
		this->m_pPropFilePath->SetValue( strPath.c_str() );
	}
	
	ASSERT( m_pPropFileLength );
	if ( m_pPropFileLength )
	{
		int fileLength = pImg->GetFileLength();
		tstringstream fl;
		fl << fileLength;
		this->m_pPropFileLength->SetValue( fl.str().c_str() );
	}

	ASSERT( m_pPropImageType );
	if ( m_pPropImageType )
	{
		tstring imgTypeStr;
		CDigitalImage::EImageType imgType = pImg->GetImageType();
		switch( imgType )
		{
		case CDigitalImage::DIT_Gray:
			imgTypeStr = _T( "Gray" );
			break;
		case CDigitalImage::DIT_RGB:
			imgTypeStr = _T( "RGB" );
		    break;
		default:
			imgTypeStr = _T( "Unknown" );
		    break;
		}
		m_pPropImageType->SetValue( imgTypeStr.c_str() );
	}

	ASSERT( m_pPropImageIntensiveLevel );
	if ( m_pPropImageIntensiveLevel )
	{
		int il = pImg->GetIntensityLevel();
		tstringstream ssIl;
		ssIl << il;
		m_pPropImageIntensiveLevel->SetValue( ssIl.str().c_str() );
	}

	ASSERT( m_pPropImageBPP );
	if ( m_pPropImageBPP )
	{
		int bpp = pImg->GetBitsPerPixel();
		tstringstream ssBpp;
		ssBpp << bpp;
		m_pPropImageBPP->SetValue( ssBpp.str().c_str() );
	}

//	ASSERT( m_pHistogramDialog->GetSafeHwnd() );
	ASSERT( m_pPropHistogram );
	if ( m_pHistogramDialog )
	{
		CDigitalImage::THistogramData grayH, rh, gh, bh;
		pImg->GetHistogramData( grayH, rh, gh, bh );
		m_pHistogramDialog->SetHistoramData( grayH );
	}
}