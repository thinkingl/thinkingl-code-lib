
// XPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XPlayer.h"
#include "XPlayerDlg.h"
#include "afxdialogex.h"
#include "Log.h"
#include "error.h"
#include "dsfun.h"

// 文档参见 https://msdn.microsoft.com/en-us/library/windows/desktop/dd375454(v=vs.85).aspx

#include <dshow.h>

#pragma comment( lib, "Strmiids.lib")	// Exports class identifiers (CLSIDs) and interface identifiers (IIDs).
#pragma comment( lib, "Quartz.lib")		// Exports the AMGetErrorText function. If you do not call this function, this library is not required.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CXPlayerDlg 对话框



CXPlayerDlg::CXPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXPlayerDlg::IDD, pParent)
	, m_pGraph(NULL)
	, m_pControl(NULL)
	, m_pEvent(NULL)
	, m_rotRegister(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_eidtLog);
}

BEGIN_MESSAGE_MAP(CXPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_PLAY, &CXPlayerDlg::OnBnClickedPlay)
	ON_BN_CLICKED(ID_SHOWLOG, &CXPlayerDlg::OnBnClickedShowlog)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_STOP, &CXPlayerDlg::OnBnClickedStop)
	ON_BN_CLICKED(ID_ENUM_FILTER, &CXPlayerDlg::OnBnClickedEnumFilter)
END_MESSAGE_MAP()


// CXPlayerDlg 消息处理程序

BOOL CXPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	// 设置定时器.
	this->SetTimer(0, 1, 0);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CXPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CXPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CXPlayerDlg::OnBnClickedPlay()
{
	// TODO:  在此添加控件通知处理程序代码
	
	if ( m_pGraph )
	{
		LOG() << "Already playing.";
		return;
	}

	HRESULT hr = CoInitialize(NULL);
	if ( FAILED( hr ) )
	{
		MessageBox(L"Initialize COM fail!");
		return;
	}



	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraph);
	if ( FAILED( hr ) )
	{
		MessageBox(L"Create the Filter Graph Manager fail!");
		return;
	}

	// 添加到ROT,启用Graph Edit的DirectShow filter调试功能.
#ifdef _DEBUG
	hr = AddToRot( m_pGraph, &m_rotRegister);
#endif

	hr = m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pControl);
	hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void**)&m_pEvent);


	CFileDialog dlg(TRUE, NULL, NULL, NULL, NULL, this);
	if (IDOK == dlg.DoModal())
	{
		CString filePath = dlg.GetPathName();

		hr = m_pGraph->RenderFile(filePath, NULL);
		if ( SUCCEEDED(hr))
		{
			// Run the graph
			hr = m_pControl->Run();
			if (SUCCEEDED(hr))
			{
//				LOG() << "Play complete, hr: " << (void*)hr << " mean: " << CError::ErrorMsg(hr) << " event code: " << evCode;
			}
		}
		else
		{
			LOG() << "Render file fail! hr: " << (void*)hr << " meaning: " << CError::ErrorMsg(hr);
			VFW_E_INVALID_FILE_FORMAT;
		}
	}


}


void CXPlayerDlg::OnBnClickedShowlog()
{
	// TODO:  在此添加控件通知处理程序代码
	std::wstring allLog;
	s_log.toString( allLog );
	m_eidtLog.SetWindowTextW(allLog.c_str());
}


void CXPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	// 处理播放器事件.
	if ( m_pEvent )
	{
		long evCode;
		LONG_PTR param1, param2;
		HRESULT hr;
		while ( hr = m_pEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr))
		{
			LOG() << "Get event: " << CError::EVMsg(evCode) << " [" << evCode << "] p1: " << param1 << " p2: " << param2;

			hr = m_pEvent->FreeEventParams(evCode, param1, param2);
		}
	}


	CDialogEx::OnTimer(nIDEvent);
}


void CXPlayerDlg::OnBnClickedStop()
{
	// TODO:  在此添加控件通知处理程序代码
	if ( m_pGraph )
	{
#ifdef _DEBUG
		RemoveFromRot(m_rotRegister);
#endif
		m_pControl->Release();
		m_pControl = NULL;
		m_pEvent->Release();
		m_pEvent = NULL;
		m_pGraph->Release();
		m_pGraph = NULL;
		CoUninitialize();
	}
}


void CXPlayerDlg::OnBnClickedEnumFilter()
{
	// TODO:  在此添加控件通知处理程序代码
	IFilterMapper2 *pMapper = NULL;
	IEnumMoniker *pEnum = NULL;

	HRESULT hr = CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC, IID_IFilterMapper2, (void**)&pMapper);
	if ( FAILED(hr))
	{
		LOG() << "Create filter mapper fail! hr: " << CError::ErrorMsg(hr) << "[" << (void*)hr << "]";
	}

	GUID arrayInTypes[2] = { NULL, NULL };
	MEDIATYPE_Video;
	MEDIASUBTYPE_dvsd;

	hr = pMapper->EnumMatchingFilters(
		&pEnum,
		0,	// Reserved.
		FALSE, //TRUE,	// Use exact match.
		MERIT_DO_NOT_USE+1, // Minimum merit.
		TRUE,	// At least one input pin?
		1,		// Number of major type/subtype pairs for input.
		arrayInTypes,	// Array of major type/subtype pairs for input.
		NULL,	// Input medium.
		NULL,	// Input pin category
		FALSE,	// Must be a render?
		TRUE,	// At least one output pin?
		0,			// Number of major type/subtype paires for output.
		NULL,	// Array of major type/subtype pairs for output.
		NULL,	// Output medium.
		NULL		// Output pin Category.
		);

	// Emumerate the monikers.
	IMoniker *pMoniker = 0;
	ULONG cFetched;
	while (pEnum->Next(1, &pMoniker, &cFetched) == S_OK)
	{
		IPropertyBag* pPropBag = NULL;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);
		if ( SUCCEEDED(hr))
		{
			// To retrieve the friendly name of the filter, do the following.
			VARIANT varName;
			VariantInit(&varName);
			hr = pPropBag->Read(L"FriendlyName", &varName, 0);
			if (SUCCEEDED(hr))
			{
				// Display the name.
				LOG() << "Filter name: " << varName.bstrVal;
			}
			VariantClear(&varName);

			// To Create an instance of the filter
			//IBaseFilter* pFilter;
			//hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pFilter);

			// Clean up.
			pPropBag->Release();
		}

		pMoniker->Release();
	}

	// Clean up.
	pMapper->Release();
	pEnum->Release();
}
