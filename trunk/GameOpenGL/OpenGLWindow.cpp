
// 类名:	COpenGLWindow
// OpenGLWindow.cpp
// 游戏主窗口类。
// 创建和管理游戏主窗口.
// 由游戏配置文件读入游戏的窗口配置信息,并据此创建游戏窗口.
// 提供全屏与窗口的模式转换.
// 创建于 2006.3.2日
// By Sinbad
// QQ: 155124089


// 游戏标题
#define GAMETITLE "My Work Demo 0.1"

// 下拉列表中的内容位置宏定义

// 游戏模式
#define WINDOWMODE	0
#define FULLSCREEN	1

// 配置文件名
#define SETUP_FILENAME "ini\\setup.ini"

#include ".\openglwindow.h"
#using <mscorlib.dll>

OpenGLWindow::OpenGLWindow(void)
{
	m_iniWindowSet.Open( SETUP_FILENAME );
	m_Width = m_iniWindowSet.ReadInt ( "Setup" , "Width");
	m_Height = m_iniWindowSet.ReadInt ( "Setup" , "Height");
	m_colourBits = m_iniWindowSet.ReadInt ( "Setup","ColourDepth");
	m_fullScreen = m_iniWindowSet.ReadInt ( "Setup", "GameMode");
	m_hWnd = NULL;
	m_hDC = NULL;
	m_hRC = NULL;

}

OpenGLWindow::~OpenGLWindow(void)
{

}

// 创建游戏窗口,并对窗口进行初始化
HWND OpenGLWindow::IniWindow(HINSTANCE hInstance, int nCmdShow , WNDPROC MsgProc)
{


// 注册窗口类

	DWORD	dwExStyle;		// Window 扩展风格
	DWORD	dwStyle;		   // Window 窗口风格
	RECT	windowRect;			// 窗口尺寸
	int		nX=0,nY=0;

	if (m_fullScreen == FULLSCREEN)		// 选择全屏模式
	{	
		DEVMODE dmScr;							// 设备模式
		memset(&dmScr,0,sizeof(dmScr));	// 确保内存分配
		dmScr.dmSize=sizeof(dmScr);		// Devmode 结构的大小
		dmScr.dmPelsWidth = m_Width;		// 屏幕宽
		dmScr.dmPelsHeight= m_Height;		// 屏幕高
		dmScr.dmBitsPerPel= m_colourBits;// 色彩深度
		dmScr.dmDisplayFrequency=75;		// 刷屏速度
		dmScr.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;
		if (ChangeDisplaySettings(&dmScr, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				m_fullScreen=FALSE;
			}
		dwExStyle=WS_EX_APPWINDOW;		// Window 扩展风格
		dwStyle=WS_POPUP;				// Window 窗口风格
//		ShowCursor(FALSE);				// 隐藏鼠标
	}
	else
	{	
		dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;	// 使窗口具有3D外观
		dwStyle=WS_OVERLAPPEDWINDOW;				// 使用标准窗口
		//  WS_OVERLAPPEDWINDOW是有标题栏,窗口菜单,最大、小化按钮和可调整尺寸的窗口
		int wid=GetSystemMetrics(SM_CXSCREEN);		// 获取当前屏幕宽
		int hei=GetSystemMetrics(SM_CYSCREEN);		// 获取当前屏幕高
		nX=(wid-m_Width)/2;nY=(hei-m_Height)/2;		// 计算窗口居中用
	}
//-------------------------------------------------------------------
	AdjustWindowRectEx(&windowRect,dwStyle,FALSE,dwExStyle); // 根据窗口风格来调整窗口尺寸达到要求的大小
									
	// 填充窗口类结构。
	WNDCLASSEX wc;
	/*WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      GAMETITLE, NULL };*/
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = MsgProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GAMETITLE;
	wc.hIconSm = NULL;

	wc.hCursor = LoadCursor( NULL , IDC_ARROW);	// 窗口平时的鼠标样式.
	RegisterClassEx( &wc );

	m_hWnd = CreateWindowEx(NULL,GAMETITLE,GAMETITLE,
						  dwStyle|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
						  nX, nY,m_Width, m_Height,
						  NULL,NULL,hInstance,NULL);			// 创建窗口
	
	////////////////////////////////////////

	ShowWindow( m_hWnd, SW_SHOWDEFAULT );	// 显示窗口
	UpdateWindow( m_hWnd );									// 刷新窗口
	
	return m_hWnd;
}
