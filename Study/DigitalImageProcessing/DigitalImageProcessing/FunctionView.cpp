
#include "stdafx.h"
#include "MainFrm.h"
#include "Resource.h"
#include "DigitalImageProcessing.h"
#include "FunctionView.h"

#include "portabledefine.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CFunctionView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

CFunctionView::CFunctionView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	this->m_curFunction = DIP_Invalid;
}

CFunctionView::~CFunctionView()
{
}

BEGIN_MESSAGE_MAP(CFunctionView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(TVN_SELCHANGED, ID_FunctionTree, OnFunctionTreeSelChange )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFunctionView 消息处理程序

int CFunctionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndFunctionView.Create(dwViewStyle, rectDummy, this, ID_FunctionTree))
	{
		TRACE0("未能创建功能视图\n");
		return -1;      // 未能创建
	}

	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	// 创建所有的功能对话框. 并放入到 功能 -> 功能对话框 的映射表中.
	m_dlgIntensityLevel.Create( CGrayscaleDialog::IDD, this );
	m_dlgIntensityLevel.SetDIPFunction( DIP_IntensityLevels );
	this->m_functionDlgTable[ DIP_IntensityLevels ] = &m_dlgIntensityLevel;

	m_dlgRGB2GrayHSI.Create( CGrayscaleDialog::IDD, this );
	m_dlgRGB2GrayHSI.SetDIPFunction( DIP_RGB2Gray_HSI );
	this->m_functionDlgTable[ DIP_RGB2Gray_HSI ] = &m_dlgRGB2GrayHSI;

	m_dlgRGB2GrayYUV.Create( CGrayscaleDialog::IDD, this );
	m_dlgRGB2GrayYUV.SetDIPFunction( DIP_RGB2Gray_YUV );
	this->m_functionDlgTable[ DIP_RGB2Gray_YUV ] = &m_dlgRGB2GrayYUV;

	m_dlgSpatialRotation.Create( CSpatialRotationDialog::IDD, this );
	this->m_functionDlgTable[ DIP_Spatial_Rotation ] = &m_dlgSpatialRotation;

	m_dlgSpatialTranslation.Create( CSpatialTranslationDialog::IDD, this );
	this->m_functionDlgTable[ DIP_Spatial_Translation ] = &m_dlgSpatialTranslation;

	m_dlgSpatialScaling.Create( CSpatialScalingDialog::IDD, this );
	this->m_functionDlgTable[ DIP_Spatial_Scaling ] = &m_dlgSpatialScaling;

	m_dlgImageNegative.Create( CImageNegativeDialog::IDD, this );
	this->m_functionDlgTable[ DIP_IntensityNegitive ] = &m_dlgImageNegative;

	m_dlgIntensityLogTransformation.Create( CIntensityLogTransformationDialog::IDD, this );
	this->m_functionDlgTable[ DIP_IntensityLog ] = &m_dlgIntensityLogTransformation;

	m_dlgIntensityPowerTransformation.Create( CIntensityPowerTransformationDialog::IDD, this );
	this->m_functionDlgTable[ DIP_IntensityPower ] = &m_dlgIntensityPowerTransformation;

	m_dlgIntensityBitplaneSlicing.Create( CIntensityBitplaneSlicingDialog::IDD, this );
	this->m_functionDlgTable[ DIP_IntensityBitsPlaneReconstruct ] = &m_dlgIntensityBitplaneSlicing;

	m_dlgIntensityBitplaneSlicShow.Create( CIntensityBitplaneSliceShowDialog::IDD, this );
	this->m_functionDlgTable[ DIP_IntensityBitsPlaneShow ] = &m_dlgIntensityBitplaneSlicShow;

	m_dlgHistogramEqualization.Create( CIntensityHistogramEqualizationDialog::IDD, this );
	this->m_functionDlgTable[ DIP_IntensityHistogramEqualization ] = &m_dlgHistogramEqualization;

	m_dlgNoiseAddGaussian.Create( CNoiseAddGaussianDialog::IDD, this );
	this->m_functionDlgTable[ DIP_NoiseGaussian ] = &m_dlgNoiseAddGaussian;

	m_dlgNoiseAddSaltAndPepper.Create( CNoiseAddSaltAndPepperDialog::IDD, this );
	this->m_functionDlgTable[ DIP_NoiseSaltAndPepper ] = &m_dlgNoiseAddSaltAndPepper;
	
	m_dlgNoiseSpatialMeanFilter.Create( CNoiseSpatialMeanFilterDialog::IDD, this );
	this->m_functionDlgTable[ DIP_NoiseSpatialMeanFilter ] = &m_dlgNoiseSpatialMeanFilter;

	m_dlgNoiseSpatialMedianFilter.Create( CSpatialMedianFilterDialog::IDD, this );
	this->m_functionDlgTable[ DIP_NoiseSpatialMedianFilter ] = &m_dlgNoiseSpatialMedianFilter;

	m_dlgFrequencyFourierTransform.Create( CFrequencyFourierTransformDialog::IDD, this );
	this->m_functionDlgTable[ DIP_FrequencyFourierTransform ] = &m_dlgFrequencyFourierTransform;

	m_dlgCompressionDCT.Create( CCompressionDCTDialog::IDD, this );
	this->m_functionDlgTable[ DIP_Compression_DCT ] = &m_dlgCompressionDCT;

	m_dlgEdgeDetect.Create( CEdgeDetectionDialog::IDD, this );
	this->m_functionDlgTable[ DIP_EdgeDetection ] = &m_dlgEdgeDetect;

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	FillDIPFunctionTree();

	return 0;
}

void CFunctionView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFunctionView::FillDIPFunctionTree()
{
	HTREEITEM hRoot = m_wndFunctionView.InsertItem(_T("功能"), 0, 0);
	m_wndFunctionView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hRGB2Gray = m_wndFunctionView.InsertItem(_T("RGB->Gray"), 2, 2, hRoot);
	HTREEITEM hAvrRGB = m_wndFunctionView.InsertItem(_T("RGB平均(HSI)"), 3, 3, hRGB2Gray);
	m_wndFunctionView.SetItemData( hAvrRGB, DIP_RGB2Gray_HSI );

	HTREEITEM hYUV = m_wndFunctionView.InsertItem(_T("YUV方式"), 3, 3, hRGB2Gray);
	m_wndFunctionView.SetItemData( hYUV, DIP_RGB2Gray_YUV );

	// 图像空间操作.
	HTREEITEM hSpatialOperatisons = m_wndFunctionView.InsertItem(_T("2.6.5 Spatial Operations"), 2, 2, hRoot);
	HTREEITEM hRotation = m_wndFunctionView.InsertItem( _T( "Rotation" ), 3, 3, hSpatialOperatisons );
	m_wndFunctionView.SetItemData( hRotation, DIP_Spatial_Rotation );

	HTREEITEM hTranslation = m_wndFunctionView.InsertItem( _T( "Translation" ), 3, 3, hSpatialOperatisons );
	m_wndFunctionView.SetItemData( hTranslation, DIP_Spatial_Translation );

	HTREEITEM hScaling = m_wndFunctionView.InsertItem( _T( "Scaling" ), 3, 3, hSpatialOperatisons );
	m_wndFunctionView.SetItemData( hScaling, DIP_Spatial_Scaling );

	// 图像灰度转换.
	HTREEITEM hIntensityFun = m_wndFunctionView.InsertItem(_T("3 Intensity Transformation"), 2, 2, hRoot);

	HTREEITEM hGrayscaleLevel = m_wndFunctionView.InsertItem(_T("灰阶调节"), 3, 3, hIntensityFun );
	m_wndFunctionView.SetItemData( hGrayscaleLevel, DIP_IntensityLevels );

	HTREEITEM hNegitive = m_wndFunctionView.InsertItem(_T("3.2.1 Image Negitive"), 3, 3, hIntensityFun );
	m_wndFunctionView.SetItemData( hNegitive, DIP_IntensityNegitive );
	
	HTREEITEM hLog = m_wndFunctionView.InsertItem(_T("3.2.2 Log Transformations"), 3, 3, hIntensityFun );
	m_wndFunctionView.SetItemData( hLog, DIP_IntensityLog );

	HTREEITEM hPower = m_wndFunctionView.InsertItem(_T("3.2.3 Power-Law Transformations"), 3, 3, hIntensityFun );
	m_wndFunctionView.SetItemData( hPower, DIP_IntensityPower );

	HTREEITEM hBitReconstruct = m_wndFunctionView.InsertItem( _T("3.2.4 Bit planes reconstruct"), 3, 3, hIntensityFun );
	m_wndFunctionView.SetItemData( hBitReconstruct, DIP_IntensityBitsPlaneReconstruct );

	HTREEITEM hBitShow = m_wndFunctionView.InsertItem( _T("3.2.4 Bit planes show"), 3, 3, hIntensityFun );
	m_wndFunctionView.SetItemData( hBitShow, DIP_IntensityBitsPlaneShow );

	HTREEITEM hHistogramEqualization = m_wndFunctionView.InsertItem( _T( "3.3.1 Histogram Equalization" ), 3, 3, hIntensityFun );
	m_wndFunctionView.SetItemData( hHistogramEqualization, DIP_IntensityHistogramEqualization );

	// 噪声.
	HTREEITEM hNoise = m_wndFunctionView.InsertItem( _T( "Noise" ), 2, 2, hRoot );
	HTREEITEM hAddGaussianNoise = m_wndFunctionView.InsertItem( _T( "Add Gaussian noise" ), 3, 3, hNoise );
	m_wndFunctionView.SetItemData( hAddGaussianNoise, DIP_NoiseGaussian );

	HTREEITEM hAddSaltAndPepperNoise = m_wndFunctionView.InsertItem( _T( "Add Salt and Pepper noise" ), 3, 3, hNoise );
	m_wndFunctionView.SetItemData( hAddSaltAndPepperNoise, DIP_NoiseSaltAndPepper );

	HTREEITEM hSpatianMeanFilter = m_wndFunctionView.InsertItem( _T( "Spatian mean filter" ), 3, 3, hNoise );
	m_wndFunctionView.SetItemData( hSpatianMeanFilter, DIP_NoiseSpatialMeanFilter );

	HTREEITEM hSpatianMedianFilter = m_wndFunctionView.InsertItem( _T( "Spatian median filter" ), 3, 3, hNoise );
	m_wndFunctionView.SetItemData( hSpatianMedianFilter, DIP_NoiseSpatialMedianFilter );

	// 频域
	HTREEITEM hFrequencyDomain = m_wndFunctionView.InsertItem( _T( "4 Filtering in the Frequency Domain" ), 2, 2, hRoot );
	HTREEITEM hFourierTransform = m_wndFunctionView.InsertItem( _T( "Fourier Transform" ), 3, 3, hFrequencyDomain );
	m_wndFunctionView.SetItemData( hFourierTransform, DIP_FrequencyFourierTransform );

	// 图像压缩
	HTREEITEM hImageCompression = m_wndFunctionView.InsertItem( _T( "Image compression" ), 2, 2, hRoot );
	HTREEITEM hDCT = m_wndFunctionView.InsertItem( _T( "DCT" ), 3, 3, hImageCompression );
	m_wndFunctionView.SetItemData( hDCT, DIP_Compression_DCT );

	// 图像修复
	HTREEITEM hImageRestoration = m_wndFunctionView.InsertItem( _T( "Image Restoration" ), 2, 2, hRoot );
	HTREEITEM hRestorationProblem = m_wndFunctionView.InsertItem( _T( "作业题目" ), 3, 3, hImageRestoration );
	m_wndFunctionView.SetItemData( hRestorationProblem, DIP_Restoration_Problem );

	// 图像边界检测
	HTREEITEM hImageEdgeDetection = m_wndFunctionView.InsertItem( _T( "Image Edge Detection" ), 2, 2, hRoot );
	HTREEITEM hEdgeDetection = m_wndFunctionView.InsertItem( _T( "Edge Detection Algorithms" ), 3, 3, hImageEdgeDetection );
	m_wndFunctionView.SetItemData( hEdgeDetection, DIP_EdgeDetection );

	// 默认全部展开.
	m_wndFunctionView.Expand(hRoot, TVE_EXPAND);
	m_wndFunctionView.Expand( hRGB2Gray, TVE_EXPAND );
	m_wndFunctionView.Expand( hSpatialOperatisons, TVE_EXPAND );
	m_wndFunctionView.Expand( hIntensityFun, TVE_EXPAND );
	m_wndFunctionView.Expand( hNoise, TVE_EXPAND );
	m_wndFunctionView.Expand( hFrequencyDomain, TVE_EXPAND );
	m_wndFunctionView.Expand( hImageCompression, TVE_EXPAND );
	m_wndFunctionView.Expand( hImageRestoration, TVE_EXPAND );
	m_wndFunctionView.Expand( hImageEdgeDetection, TVE_EXPAND );
	
}

void CFunctionView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndFunctionView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CFunctionView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	CWnd *pCurFunWnd =  NULL;
	for( TFunctionDlgTable::const_iterator it = m_functionDlgTable.begin(); it != m_functionDlgTable.end(); ++it )
	{
		if ( it->second && it->second->GetSafeHwnd() )
		{
			if ( it->first == this->m_curFunction )
			{
				pCurFunWnd = it->second;
			}	
			else
			{
				it->second->ShowWindow( SW_HIDE );
			}
		}		
	}

	CRect rcFunDlg;
	if ( pCurFunWnd )
	{
		pCurFunWnd->ShowWindow( SW_SHOW );
		pCurFunWnd->GetWindowRect( rcFunDlg );
		pCurFunWnd->SetWindowPos( NULL, rectClient.left, rectClient.bottom - rcFunDlg.Height(), rectClient.Width(), rcFunDlg.Height(), SWP_NOACTIVATE | SWP_NOZORDER );
	}	

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	m_wndFunctionView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - rcFunDlg.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CFunctionView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CFunctionView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CFunctionView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CFunctionView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

void CFunctionView::OnClassAddMemberVariable()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFunctionView::OnClassDefinition()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFunctionView::OnClassProperties()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFunctionView::OnNewFolder()
{
	AfxMessageBox(_T("新建文件夹..."));
}

void CFunctionView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndFunctionView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFunctionView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFunctionView.SetFocus();
}

void CFunctionView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndFunctionView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 锁定*/);
}


void CFunctionView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDockablePane::OnLButtonDown(nFlags, point);
}


HRESULT CFunctionView::accSelect(long flagsSelect, VARIANT varChild)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDockablePane::accSelect(flagsSelect, varChild);
}


BOOL CFunctionView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDockablePane::OnChildNotify(message, wParam, lParam, pLResult);
}


BOOL CFunctionView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( ID_FunctionTree == wParam )
	{
		LPNMHDR pNMHDR = (LPNMHDR)lParam;
		if ( TVN_SELCHANGED == pNMHDR->code )
		{
			LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
			int dipFunId = pNMTreeView->itemNew.lParam;
// 			switch( dipFunId )
// 			{
// 			case DIP_RGB2Gray_HSI:
// 
// 				break;
// 			case DIP_RGB2Gray_YUV:
// 				break;
// 			default:
// 				
// 				break;
// 			}

			this->m_curFunction = (EDIPFunction)dipFunId;

			this->AdjustLayout();
		}
	}


	return CDockablePane::OnNotify(wParam, lParam, pResult);
}


BOOL CFunctionView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( TVN_SELCHANGED == wParam )
	{
		int fsdfe = 32;
	}
	tstringstream ssMsg;
	ssMsg << _T( "on command: ") << wParam << _T( " lp: ") << lParam << endl;
	OutputDebugString( ssMsg.str().c_str() );

	return CDockablePane::OnCommand(wParam, lParam);
}

void CFunctionView::OnFunctionTreeSelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码



	*pResult = 0;
}


BOOL CFunctionView::Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, BOOL bHasGripper, UINT nID, DWORD dwStyle, DWORD dwTabbedStyle /* = AFX_CBRS_REGULAR_TABS */, DWORD dwControlBarStyle /* = AFX_DEFAULT_DOCKING_PANE_STYLE */, CCreateContext* pContext /* = NULL */)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRet = CDockablePane::Create(lpszCaption, pParentWnd, rect, bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, pContext);



	return bRet;
}
