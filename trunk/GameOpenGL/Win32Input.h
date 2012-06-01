
#pragma once

// Win32Input.h
// Win32Input类的头文件.
// 实现WindowsAPI实现的输入.
// 2006.3.29

#include "Windows.h"


class Win32Input
{
public:
	Win32Input(void);
public:
	~Win32Input(void);

public:
	// 根据给的虚拟键值判断对应的按键有没有按下.
	bool IsKeyDown( int nVirtualKeyCode ){ return static_cast<bool>( HIBYTE( GetAsyncKeyState( nVirtualKeyCode ))); }	

	// 获得鼠标在X,Y轴的相对移动
	int  GetMouseRelativeMoveX(){ return m_CursorRelMove.x; }			
	int  GetMouseRelativeMoveY(){ return m_CursorRelMove.y; }

	bool Update();								// 刷新缓冲区.
	bool Initialize( bool isUseCursor );							// 初始化

private:
	POINT m_CursorPos;		// 鼠标光标的位置.
	POINT m_CursorRelMove;	// 鼠标相对移动的位移.
	bool  m_bUseCursor;		// 是否使用鼠标光标.
};
