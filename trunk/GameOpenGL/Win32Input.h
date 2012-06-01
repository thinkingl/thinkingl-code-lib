
#pragma once

// Win32Input.h
// Win32Input���ͷ�ļ�.
// ʵ��WindowsAPIʵ�ֵ�����.
// 2006.3.29

#include "Windows.h"


class Win32Input
{
public:
	Win32Input(void);
public:
	~Win32Input(void);

public:
	// ���ݸ��������ֵ�ж϶�Ӧ�İ�����û�а���.
	bool IsKeyDown( int nVirtualKeyCode ){ return static_cast<bool>( HIBYTE( GetAsyncKeyState( nVirtualKeyCode ))); }	

	// ��������X,Y�������ƶ�
	int  GetMouseRelativeMoveX(){ return m_CursorRelMove.x; }			
	int  GetMouseRelativeMoveY(){ return m_CursorRelMove.y; }

	bool Update();								// ˢ�»�����.
	bool Initialize( bool isUseCursor );							// ��ʼ��

private:
	POINT m_CursorPos;		// ������λ��.
	POINT m_CursorRelMove;	// �������ƶ���λ��.
	bool  m_bUseCursor;		// �Ƿ�ʹ�������.
};
