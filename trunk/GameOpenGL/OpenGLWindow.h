
// ����:	COpenGLWindow

// ��Ϸ�������ࡣ
// �����͹�����Ϸ������.
// ����Ϸ�����ļ�������Ϸ�Ĵ���������Ϣ,���ݴ˴�����Ϸ����.
// �ṩȫ���봰�ڵ�ģʽת��.
// ������ 2006.3.2��
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
	CIniSet m_iniWindowSet;	// ���������ļ�����.

	int		m_Width;				// ���ڿ�
	int		m_Height;				// ���ڸ�
	int		m_colourBits  ;		// ��ɫ���
	bool    m_fullScreen;		// �Ƿ�ȫ��
	HWND	m_hWnd;	// ���� Windows ���������Ĵ��ھ��
	HDC m_hDC;					// ���洰�ڵ��豸�����
	HGLRC m_hRC;
public:
	// ������Ϸ����,���Դ��ڽ��г�ʼ��
	HWND IniWindow(HINSTANCE hInstance, int nCmdShow ,WNDPROC  MsgProc);
};
