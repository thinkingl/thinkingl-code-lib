
// KedaGift.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKedaGiftApp:
// �йش����ʵ�֣������ KedaGift.cpp
//

class CKedaGiftApp : public CWinApp
{
public:
	CKedaGiftApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKedaGiftApp theApp;