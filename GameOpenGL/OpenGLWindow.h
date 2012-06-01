
// 类名:	COpenGLWindow

// 游戏主窗口类。
// 创建和管理游戏主窗口.
// 由游戏配置文件读入游戏的窗口配置信息,并据此创建游戏窗口.
// 提供全屏与窗口的模式转换.
// 创建于 2006.3.2日
// By Sinbad
// QQ: 155124089

#pragma once

#include "stdafx.h"
#include ".\include\iniset.h"

class OpenGLWindow
{
public:
	OpenGLWindow(void);
	~OpenGLWindow(void);
	
private:
	CIniSet m_iniWindowSet;	// 窗口配置文件对象.

	int		m_Width;				// 窗口宽
	int		m_Height;				// 窗口高
	int		m_colourBits  ;		// 颜色深度
	bool    m_fullScreen;		// 是否全屏
	HWND	m_hWnd;	// 保存 Windows 分配给程序的窗口句柄
	HDC m_hDC;					// 保存窗口的设备句柄。
	HGLRC m_hRC;
public:
	// 创建游戏窗口,并对窗口进行初始化
	HWND IniWindow(HINSTANCE hInstance, int nCmdShow ,WNDPROC  MsgProc);
};
