
// Socks5Demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSocks5DemoApp:
// �йش����ʵ�֣������ Socks5Demo.cpp
//

class CSocks5DemoApp : public CWinAppEx
{
public:
	CSocks5DemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSocks5DemoApp theApp;