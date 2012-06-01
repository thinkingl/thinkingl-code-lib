
// ����:	COpenGLWindow
// OpenGLWindow.cpp
// ��Ϸ�������ࡣ
// �����͹�����Ϸ������.
// ����Ϸ�����ļ�������Ϸ�Ĵ���������Ϣ,���ݴ˴�����Ϸ����.
// �ṩȫ���봰�ڵ�ģʽת��.
// ������ 2006.3.2��
// By Sinbad
// QQ: 155124089


// ��Ϸ����
#define GAMETITLE "My Work Demo 0.1"

// �����б��е�����λ�ú궨��

// ��Ϸģʽ
#define WINDOWMODE	0
#define FULLSCREEN	1

// �����ļ���
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

// ������Ϸ����,���Դ��ڽ��г�ʼ��
HWND OpenGLWindow::IniWindow(HINSTANCE hInstance, int nCmdShow , WNDPROC MsgProc)
{


// ע�ᴰ����

	DWORD	dwExStyle;		// Window ��չ���
	DWORD	dwStyle;		   // Window ���ڷ��
	RECT	windowRect;			// ���ڳߴ�
	int		nX=0,nY=0;

	if (m_fullScreen == FULLSCREEN)		// ѡ��ȫ��ģʽ
	{	
		DEVMODE dmScr;							// �豸ģʽ
		memset(&dmScr,0,sizeof(dmScr));	// ȷ���ڴ����
		dmScr.dmSize=sizeof(dmScr);		// Devmode �ṹ�Ĵ�С
		dmScr.dmPelsWidth = m_Width;		// ��Ļ��
		dmScr.dmPelsHeight= m_Height;		// ��Ļ��
		dmScr.dmBitsPerPel= m_colourBits;// ɫ�����
		dmScr.dmDisplayFrequency=75;		// ˢ���ٶ�
		dmScr.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;
		if (ChangeDisplaySettings(&dmScr, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				m_fullScreen=FALSE;
			}
		dwExStyle=WS_EX_APPWINDOW;		// Window ��չ���
		dwStyle=WS_POPUP;				// Window ���ڷ��
//		ShowCursor(FALSE);				// �������
	}
	else
	{	
		dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;	// ʹ���ھ���3D���
		dwStyle=WS_OVERLAPPEDWINDOW;				// ʹ�ñ�׼����
		//  WS_OVERLAPPEDWINDOW���б�����,���ڲ˵�,���С����ť�Ϳɵ����ߴ�Ĵ���
		int wid=GetSystemMetrics(SM_CXSCREEN);		// ��ȡ��ǰ��Ļ��
		int hei=GetSystemMetrics(SM_CYSCREEN);		// ��ȡ��ǰ��Ļ��
		nX=(wid-m_Width)/2;nY=(hei-m_Height)/2;		// ���㴰�ھ�����
	}
//-------------------------------------------------------------------
	AdjustWindowRectEx(&windowRect,dwStyle,FALSE,dwExStyle); // ���ݴ��ڷ�����������ڳߴ�ﵽҪ��Ĵ�С
									
	// ��䴰����ṹ��
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

	wc.hCursor = LoadCursor( NULL , IDC_ARROW);	// ����ƽʱ�������ʽ.
	RegisterClassEx( &wc );

	m_hWnd = CreateWindowEx(NULL,GAMETITLE,GAMETITLE,
						  dwStyle|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
						  nX, nY,m_Width, m_Height,
						  NULL,NULL,hInstance,NULL);			// ��������
	
	////////////////////////////////////////

	ShowWindow( m_hWnd, SW_SHOWDEFAULT );	// ��ʾ����
	UpdateWindow( m_hWnd );									// ˢ�´���
	
	return m_hWnd;
}
