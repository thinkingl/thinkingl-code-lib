
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "DigitalImageProcessing.h"
#include "DigitalImage.h"

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
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
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

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = 0;
	if ( m_wndToolBar.GetSafeHwnd() )
	{
		cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
		m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	}
	

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	
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

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("未能创建属性组合 \n");
		return -1;      // 未能创建
	}

	m_wndObjectCombo.AddString(_T("应用程序"));
	m_wndObjectCombo.AddString(_T("属性窗口"));
	m_wndObjectCombo.SetCurSel(0);

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

void CPropertiesWnd::OnProperties1()
{
	// TODO: 在此处添加命令处理程序代码
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: 在此处添加命令处理程序代码
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

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

	m_wndPropList.AddProperty( pGroupImage );
// 
// 		CMFC
// 	
// 		dProperty* pProp = new CMFCPropertyGridProperty(_T("边框"), _T("对话框外框"), _T("其中之一:“无”、“细”、“可调整大小”或“对话框外框”"));
// 		pProp->Ad
// 		n(_T("无"));
// 		pProp->Ad
// 		n(_T("细"));
// 		pProp->Ad
// 		n(_T("可调整大小"));
// 		pProp->Ad
// 		n(_T("对话框外框"));
// 		pProp->Al
// 		t(FALSE);
// 	
// 		pGro
// 	
// 		Item(pProp);
// 		pGroup1->
// 		Item(new CMFCPropertyGridProperty(_T("标题"), (_variant_t) _T("关于"), _T("指定窗口标题栏中显示的文本")));
// 	
// 		m_wn
// 	
// 		ddProperty(pGroup1);
// 	
// 		CMFC
// 	
// 		dProperty* pSize = new CMFCPropertyGridProperty(_T("窗口大小"), 0, TRUE);
// 	
// 		pPro
// 	
// 		CPropertyGridProperty(_T("高度"), (_variant_t) 250l, _T("指定窗口的高度"));
// 		pProp->En
// 		inControl(TRUE, 50, 300);
// 		pSize->Ad
// 		em(pProp);
// 	
// 		pPro
// 	
// 		CPropertyGridProperty( _T("宽度"), (_variant_t) 150l, _T("指定窗口的宽度"));
// 		pProp->En
// 		inControl(TRUE, 50, 200);
// 		pSize->Ad
// 		em(pProp);
// 	
// 		m_wn
// 	
// 		ddProperty(pSize);
// 	
// 		CMFC
// 	
// 		dProperty* pGroup2 = new CMFCPropertyGridProperty(_T("字体"));
// 	
// 		LOGF
// 	
// 		 	CFont* fo
// 		Font::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
// 		font->Get
// 		t(&lf);
// 	
// 		lstr
// 	
// 		ceName, _T("宋体, Arial"));
// 	
// 		pGro
// 	
// 		Item(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定窗口的默认字体")));
// 		pGroup2->
// 		Item(new CMFCPropertyGridProperty(_T("使用系统字体"), (_variant_t) true, _T("指定窗口使用“MS Shell Dlg”字体")));
// 	
// 		m_wn
// 	
// 		ddProperty(pGroup2);
// 	
// 		CMFC
// 	
// 		dProperty* pGroup3 = new CMFCPropertyGridProperty(_T("杂项"));
// 		pProp = n
// 		CPropertyGridProperty(_T("(名称)"), _T("应用程序"));
// 		pProp->En
// 		ALSE);
// 		pGroup3->
// 		Item(pProp);
// 	
// 		CMFC
// 	
// 		dColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("窗口颜色"), RGB(210, 192, 254), NULL, _T("指定默认的窗口颜色"));
// 		pColorPro
// 		bleOtherButton(_T("其他..."));
// 		pColorPro
// 		bleAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
// 		pGroup3->
// 		Item(pColorProp);
// 	
// 		stat
// 	
// 		HAR szFilter[] = _T("图标文件(*.ico)|*.ico|所有文件(*.*)|*.*||");
// 		pGroup3->
// 		Item(new CMFCPropertyGridFileProperty(_T("图标"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("指定窗口图标")));
// 	
// 		pGro
// 	
// 		Item(new CMFCPropertyGridFileProperty(_T("文件夹"), _T("c:\\")));
// 	
// 		m_wn
// 	
// 		ddProperty(pGroup3);
// 	
// 		CMFC
// 	
// 		dProperty* pGroup4 = new CMFCPropertyGridProperty(_T("层次结构"));
// 	
// 		CMFC
// 	
// 		dProperty* pGroup41 = new CMFCPropertyGridProperty(_T("第一个子级"));
// 		pGroup4->
// 		Item(pGroup41);
// 	
// 		CMFC
// 	
// 		dProperty* pGroup411 = new CMFCPropertyGridProperty(_T("第二个子级"));
// 		pGroup41-
// 		bItem(pGroup411);
// 	
// 		pGro
// 	
// 		ubItem(new CMFCPropertyGridProperty(_T("项 1"), (_variant_t) _T("值 1"), _T("此为说明")));
// 		pGroup411
// 		ubItem(new CMFCPropertyGridProperty(_T("项 2"), (_variant_t) _T("值 2"), _T("此为说明")));
// 		pGroup411
// 		ubItem(new CMFCPropertyGridProperty(_T("项 3"), (_variant_t) _T("值 3"), _T("此为说明")));
// 	
// 		pGro
// 	
// 		(FALSE);
// 		m_wndProp
// 		ddProperty(pGroup4);
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
	m_wndObjectCombo.SetFont(&m_fntPropList);
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
}