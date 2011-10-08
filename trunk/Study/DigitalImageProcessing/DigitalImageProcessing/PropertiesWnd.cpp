
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
// CResourceViewBar ��Ϣ�������

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

	// �������:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ���������� \n");
		return -1;      // δ�ܴ���
	}

	m_wndObjectCombo.AddString(_T("Ӧ�ó���"));
	m_wndObjectCombo.AddString(_T("���Դ���"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ�����������\n");
		return -1;      // δ�ܴ���
	}
	
	this->InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ������*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ����*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
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
	// TODO: �ڴ˴���������������
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: �ڴ˴����������� UI ����������
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: �ڴ˴���������������
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: �ڴ˴����������� UI ����������
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroupFile = new CMFCPropertyGridProperty(_T("�ļ�"));

	m_pPropFilePath = new CMFCPropertyGridProperty( _T( "·��" ), _T( "" ), _T( "ԭʼͼƬ�ļ�������·��" ) );
	m_pPropFilePath->AllowEdit( FALSE );
	pGroupFile->AddSubItem( m_pPropFilePath );

	m_pPropFileLength = new CMFCPropertyGridProperty( _T( "����" ), _T( "" ), _T( "ԭʼͼƬ�ļ��ĳ���" ) );
	m_pPropFileLength->AllowEdit( FALSE );
	pGroupFile->AddSubItem( m_pPropFileLength );
	
	m_wndPropList.AddProperty( pGroupFile );


	CMFCPropertyGridProperty* pGroupImage = new CMFCPropertyGridProperty(_T("ͼƬ"));

	this->m_pPropImageType = new CMFCPropertyGridProperty( _T( "����" ), _T( "" ), _T( "ͼƬ������ RGB �� Gray " ) );
	this->m_pPropImageType->AllowEdit( FALSE );
	pGroupImage->AddSubItem( m_pPropImageType );

	this->m_pPropImageIntensiveLevel = new CMFCPropertyGridProperty( _T( "�ҶȽ���" ), _T( "" ), _T( "ͼƬ�ҶȵĽ���(1-8) RGB��ɫͼƬָÿ��ԭɫ�ĻҶȽ���." ) );
	m_pPropImageIntensiveLevel->AllowEdit( FALSE );
	pGroupImage->AddSubItem( m_pPropImageIntensiveLevel );

	this->m_pPropImageBPP = new CMFCPropertyGridProperty( _T( "BPP" ), _T( "" ), _T( "Bits Per Pixel. ÿ����ռ���ֽ���." ) );
	m_pPropImageBPP->AllowEdit( FALSE );
	pGroupImage->AddSubItem( m_pPropImageBPP );

	m_wndPropList.AddProperty( pGroupImage );
// 
// 		CMFC
// 	
// 		dProperty* pProp = new CMFCPropertyGridProperty(_T("�߿�"), _T("�Ի������"), _T("����֮һ:���ޡ�����ϸ�������ɵ�����С���򡰶Ի������"));
// 		pProp->Ad
// 		n(_T("��"));
// 		pProp->Ad
// 		n(_T("ϸ"));
// 		pProp->Ad
// 		n(_T("�ɵ�����С"));
// 		pProp->Ad
// 		n(_T("�Ի������"));
// 		pProp->Al
// 		t(FALSE);
// 	
// 		pGro
// 	
// 		Item(pProp);
// 		pGroup1->
// 		Item(new CMFCPropertyGridProperty(_T("����"), (_variant_t) _T("����"), _T("ָ�����ڱ���������ʾ���ı�")));
// 	
// 		m_wn
// 	
// 		ddProperty(pGroup1);
// 	
// 		CMFC
// 	
// 		dProperty* pSize = new CMFCPropertyGridProperty(_T("���ڴ�С"), 0, TRUE);
// 	
// 		pPro
// 	
// 		CPropertyGridProperty(_T("�߶�"), (_variant_t) 250l, _T("ָ�����ڵĸ߶�"));
// 		pProp->En
// 		inControl(TRUE, 50, 300);
// 		pSize->Ad
// 		em(pProp);
// 	
// 		pPro
// 	
// 		CPropertyGridProperty( _T("���"), (_variant_t) 150l, _T("ָ�����ڵĿ��"));
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
// 		dProperty* pGroup2 = new CMFCPropertyGridProperty(_T("����"));
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
// 		ceName, _T("����, Arial"));
// 	
// 		pGro
// 	
// 		Item(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ�����ڵ�Ĭ������")));
// 		pGroup2->
// 		Item(new CMFCPropertyGridProperty(_T("ʹ��ϵͳ����"), (_variant_t) true, _T("ָ������ʹ�á�MS Shell Dlg������")));
// 	
// 		m_wn
// 	
// 		ddProperty(pGroup2);
// 	
// 		CMFC
// 	
// 		dProperty* pGroup3 = new CMFCPropertyGridProperty(_T("����"));
// 		pProp = n
// 		CPropertyGridProperty(_T("(����)"), _T("Ӧ�ó���"));
// 		pProp->En
// 		ALSE);
// 		pGroup3->
// 		Item(pProp);
// 	
// 		CMFC
// 	
// 		dColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), RGB(210, 192, 254), NULL, _T("ָ��Ĭ�ϵĴ�����ɫ"));
// 		pColorPro
// 		bleOtherButton(_T("����..."));
// 		pColorPro
// 		bleAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
// 		pGroup3->
// 		Item(pColorProp);
// 	
// 		stat
// 	
// 		HAR szFilter[] = _T("ͼ���ļ�(*.ico)|*.ico|�����ļ�(*.*)|*.*||");
// 		pGroup3->
// 		Item(new CMFCPropertyGridFileProperty(_T("ͼ��"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("ָ������ͼ��")));
// 	
// 		pGro
// 	
// 		Item(new CMFCPropertyGridFileProperty(_T("�ļ���"), _T("c:\\")));
// 	
// 		m_wn
// 	
// 		ddProperty(pGroup3);
// 	
// 		CMFC
// 	
// 		dProperty* pGroup4 = new CMFCPropertyGridProperty(_T("��νṹ"));
// 	
// 		CMFC
// 	
// 		dProperty* pGroup41 = new CMFCPropertyGridProperty(_T("��һ���Ӽ�"));
// 		pGroup4->
// 		Item(pGroup41);
// 	
// 		CMFC
// 	
// 		dProperty* pGroup411 = new CMFCPropertyGridProperty(_T("�ڶ����Ӽ�"));
// 		pGroup41-
// 		bItem(pGroup411);
// 	
// 		pGro
// 	
// 		ubItem(new CMFCPropertyGridProperty(_T("�� 1"), (_variant_t) _T("ֵ 1"), _T("��Ϊ˵��")));
// 		pGroup411
// 		ubItem(new CMFCPropertyGridProperty(_T("�� 2"), (_variant_t) _T("ֵ 2"), _T("��Ϊ˵��")));
// 		pGroup411
// 		ubItem(new CMFCPropertyGridProperty(_T("�� 3"), (_variant_t) _T("ֵ 3"), _T("��Ϊ˵��")));
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