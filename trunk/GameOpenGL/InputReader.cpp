#include "InputReader.h"
#include "stdafx.h"



InputReader::InputReader(void)
{
	////////////////////////////////////////////
	m_win32Input = new Win32Input();
	//////////////////////////////////
}

InputReader::~InputReader(void)
{
}


// ����ݼ��Ƿ񱻰���,���������͹��ܼ�.
//
bool InputReader::IsKeyDown( int wVirtualKeyCode , int wModifiers )
{
	if( m_bInputMode == IM_WIN )			//	Win API ������ģʽ��.
	{
		if ( ! m_win32Input->IsKeyDown( wVirtualKeyCode ))
			return false;
		if ( ! wModifiers )
			return true;
		if ( wModifiers & HOTKEYF_ALT )
			{
				if ( ! m_win32Input->IsKeyDown( VK_MENU ))
					return false; 
			}
		if ( wModifiers & HOTKEYF_CONTROL )
		{
			if( ! m_win32Input->IsKeyDown( VK_CONTROL ))
				return false;
		}
		if ( wModifiers & HOTKEYF_SHIFT)
		{
			if(  ! m_win32Input->IsKeyDown( VK_SHIFT ))
				return false;
		}
		return true;
	}
	else								//  Direct Input ģʽ��.
	{
		/*static*/ int  nScanKeyCode ;		//  ȡ�������ֵ��Ӧ��ɨ����.
		nScanKeyCode = MapVirtualKey( wVirtualKeyCode , 0 );		// DIģʽ���õ��Ǽ���ɨ����.

		if ( nScanKeyCode == 0 )
		{
			// ���������.
			switch( wVirtualKeyCode )
			{
			case VK_LBUTTON:
				if ( !m_directInput->IsButtonDown( 0 ) )
				{
					return false;
				}
				break;

			case VK_RBUTTON:
				if ( !m_directInput->IsButtonDown( 1 ) )
				{
					return false;
				}
				break;

			default:
				break;
			}
		}
		else
		{

			if ( ! m_directInput->IsKeyDown( nScanKeyCode ))
				return false;
		}

		if ( ! wModifiers )
			return true;
		if ( wModifiers & HOTKEYF_ALT )
			{
				if ( !( m_directInput->IsKeyDown(DIK_LALT) || m_directInput->IsKeyDown( DIK_RALT)))
					return false; 
			}
		if ( wModifiers & HOTKEYF_CONTROL )
		{
			if( !( m_directInput->IsKeyDown(DIK_LCONTROL) || m_directInput->IsKeyDown( DIK_RCONTROL)))
				return false;
		}
		if ( wModifiers & HOTKEYF_SHIFT)
		{
			if(!( m_directInput->IsKeyDown(DIK_LSHIFT) || m_directInput->IsKeyDown( DIK_RSHIFT)))
				return false;
		}
		return true;
	}
}

// ��ʼ������
bool InputReader::Initialize( HWND hWnd ,bool inputMode, bool isUseCursor )
{
	m_bInputMode = inputMode;					// ����ģʽ.
	if(m_bInputMode == IM_DI)					// DIģʽ��
	{
		m_directInput = new DirectInput();
		m_directInput->Initialize(hWnd,			// ��directInput���г�ʼ��.
			(HINSTANCE)GetModuleHandle(NULL),
			(!isUseCursor),						// ���ʹ�������,��ʹ�ö�ռ��ʽ�����DI
			IS_USEKEYBOARD|IS_USEMOUSE);	
	}
	else
	{											// Win32 API ģʽ��.
		m_win32Input = new Win32Input ();
		m_win32Input->Initialize( isUseCursor );// ��Win32 APIģʽ�����������г�ʼ��.
	}

	ShowCursor( isUseCursor );				    // ��ʾ�����.
	return true;
}

// ˢ�������豸������.
bool InputReader::Update()
{
	if( m_bInputMode == IM_DI)
		return m_directInput->Update();
	else
		return m_win32Input->Update();

	// �������λ��.
	GetCursorPos( &m_mousePos );
}

