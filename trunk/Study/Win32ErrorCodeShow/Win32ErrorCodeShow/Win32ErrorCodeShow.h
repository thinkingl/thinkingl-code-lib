
// Win32ErrorCodeShow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWin32ErrorCodeShowApp:
// �йش����ʵ�֣������ Win32ErrorCodeShow.cpp
//

class CWin32ErrorCodeShowApp : public CWinApp
{
public:
	CWin32ErrorCodeShowApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWin32ErrorCodeShowApp theApp;