
// VirtualTreeControl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVirtualTreeControlApp:
// �йش����ʵ�֣������ VirtualTreeControl.cpp
//

class CVirtualTreeControlApp : public CWinAppEx
{
public:
	CVirtualTreeControlApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVirtualTreeControlApp theApp;