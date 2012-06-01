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


// 检测快捷键是否被按下,包括主键和功能键.
//
bool InputReader::IsKeyDown( int wVirtualKeyCode , int wModifiers )
{
	if( m_bInputMode == IM_WIN )			//	Win API 的输入模式下.
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
	else								//  Direct Input 模式下.
	{
		/*static*/ int  nScanKeyCode ;		//  取得虚拟键值对应的扫描码.
		nScanKeyCode = MapVirtualKey( wVirtualKeyCode , 0 );		// DI模式下用的是键盘扫描码.

		if ( nScanKeyCode == 0 )
		{
			// 可能是鼠标.
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

// 初始化函数
bool InputReader::Initialize( HWND hWnd ,bool inputMode, bool isUseCursor )
{
	m_bInputMode = inputMode;					// 输入模式.
	if(m_bInputMode == IM_DI)					// DI模式下
	{
		m_directInput = new DirectInput();
		m_directInput->Initialize(hWnd,			// 对directInput进行初始化.
			(HINSTANCE)GetModuleHandle(NULL),
			(!isUseCursor),						// 如果使用鼠标光标,则不使用独占方式的鼠标DI
			IS_USEKEYBOARD|IS_USEMOUSE);	
	}
	else
	{											// Win32 API 模式下.
		m_win32Input = new Win32Input ();
		m_win32Input->Initialize( isUseCursor );// 对Win32 API模式的输入对象进行初始化.
	}

	ShowCursor( isUseCursor );				    // 显示鼠标光标.
	return true;
}

// 刷新输入设备缓冲区.
bool InputReader::Update()
{
	if( m_bInputMode == IM_DI)
		return m_directInput->Update();
	else
		return m_win32Input->Update();

	// 更新鼠标位置.
	GetCursorPos( &m_mousePos );
}

