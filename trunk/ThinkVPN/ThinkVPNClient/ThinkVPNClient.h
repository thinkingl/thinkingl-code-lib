// ThinkVPNClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CThinkVPNClientApp:
// �йش����ʵ�֣������ ThinkVPNClient.cpp
//

class CThinkVPNClientApp : public CWinApp
{
public:
	CThinkVPNClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CThinkVPNClientApp theApp;