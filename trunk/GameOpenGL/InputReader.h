#pragma once

// InputReader.h
// ��InputReader��ͷ�ļ�.
// ������������Ϸ�װ.
// ��֮��     2006.3.29

#include "Win32Input.h"
#include "./include/DirectInput.h"		//�����̺���Ϸ����DirectInput�Ľӿ�

// hotkey���ܼ���־����,ȡ����MFCͷ�ļ�.
#define HOTKEYF_SHIFT           0x01
#define HOTKEYF_CONTROL         0x02
#define HOTKEYF_ALT             0x04

// ����ģʽ.
#define IM_WIN  false
#define IM_DI	true
//***************************************


class InputReader
{
public:
	InputReader(void);
public:
	~InputReader(void);

private:
	Win32Input  *m_win32Input;	// ����WindowsAPI�������Ķ���.
	DirectInput *m_directInput;	// ������DI�����������Ķ���.
	bool m_bInputMode;
	POINT m_mousePos;
public:
	bool IsKeyDown(  int wVirtualKeyCode,  int wModifiers  );			// ����ݼ��Ƿ񱻰���,���������͹��ܼ�.
	bool Initialize( HWND hWnd ,bool inputMode, bool isExclusive);		// ��ʼ��.
	bool Update();				//  ˢ������.

	//  ��������X���ϵ�����˶�.
	int  GetMouseRelativeMoveX()
	{
		if( m_bInputMode == IM_DI)						   // DIģʽ��.
			return m_directInput->GetMouseRelativeMoveX();
		else
			return m_win32Input->GetMouseRelativeMoveX();  // Win32APIģʽ��.
	}
	//  ��������Y���ϵ�����˶�.
	int  GetMouseRelativeMoveY()
	{
		if( m_bInputMode == IM_DI)						   // DIģʽ��.
			return m_directInput->GetMouseRelativeMoveY();
		else
			return m_win32Input->GetMouseRelativeMoveY();  // Win32APIģʽ��
	}
	//  ��������Z���ϵ�����˶�,(������)
	int  GetMouseRelativeMoveZ()
	{
		if( m_bInputMode == IM_DI)						  // DIģʽ��.
			return m_directInput->GetMouseRelativeMoveZ();
		else
			return 0;									  // Win32APIģʽ����ʱ�޷�֧������м����.
	}

	//  ������������Ļ�ϵľ������ꡣ������û�о���λ�á�
	int GetMousePosX( ) { return m_mousePos.x; }
	int GetMousePosY( ) { return m_mousePos.y; }
	

};
