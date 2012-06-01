#pragma once

// InputReader.h
// 类InputReader的头文件.
// 对于输入的整合封装.
// 李之兴     2006.3.29

#include "Win32Input.h"
#include "./include/DirectInput.h"		//鼠标键盘和游戏柄等DirectInput的接口

// hotkey功能键标志定义,取自于MFC头文件.
#define HOTKEYF_SHIFT           0x01
#define HOTKEYF_CONTROL         0x02
#define HOTKEYF_ALT             0x04

// 输入模式.
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
	Win32Input  *m_win32Input;	// 定义WindowsAPI获得输入的对象.
	DirectInput *m_directInput;	// 定义用DI方法获得输入的对象.
	bool m_bInputMode;
	POINT m_mousePos;
public:
	bool IsKeyDown(  int wVirtualKeyCode,  int wModifiers  );			// 检测快捷键是否被按下,包括主键和功能键.
	bool Initialize( HWND hWnd ,bool inputMode, bool isExclusive);		// 初始化.
	bool Update();				//  刷新输入.

	//  获得鼠标在X轴上的相对运动.
	int  GetMouseRelativeMoveX()
	{
		if( m_bInputMode == IM_DI)						   // DI模式下.
			return m_directInput->GetMouseRelativeMoveX();
		else
			return m_win32Input->GetMouseRelativeMoveX();  // Win32API模式下.
	}
	//  获得鼠标在Y轴上的相对运动.
	int  GetMouseRelativeMoveY()
	{
		if( m_bInputMode == IM_DI)						   // DI模式下.
			return m_directInput->GetMouseRelativeMoveY();
		else
			return m_win32Input->GetMouseRelativeMoveY();  // Win32API模式下
	}
	//  获得鼠标在Z轴上的相对运动,(鼠标滚轮)
	int  GetMouseRelativeMoveZ()
	{
		if( m_bInputMode == IM_DI)						  // DI模式下.
			return m_directInput->GetMouseRelativeMoveZ();
		else
			return 0;									  // Win32API模式下暂时无法支持鼠标中间滚轮.
	}

	//  获得鼠标光标在屏幕上的绝对坐标。鼠标滚轮没有绝对位置。
	int GetMousePosX( ) { return m_mousePos.x; }
	int GetMousePosY( ) { return m_mousePos.y; }
	

};
