#include "Win32Input.h"

const int DEF_CURSOR_POS_X = 400;
const int DEF_CURSOR_POS_Y = 300;

Win32Input::Win32Input(void)
{
}

Win32Input::~Win32Input(void)
{
}


// 刷新缓冲区
bool Win32Input::Update()
{
	if (!m_bUseCursor)		// 当用户要使用鼠标光标时,要禁止重置光标位置. 
	{
		GetCursorPos( &m_CursorPos );								// 获得当前光标位置.
		m_CursorRelMove.x = DEF_CURSOR_POS_X - m_CursorPos.x;		// 获得X,Y轴上的鼠标位移.
		m_CursorRelMove.y = DEF_CURSOR_POS_Y - m_CursorPos.y;
		SetCursorPos( DEF_CURSOR_POS_X , DEF_CURSOR_POS_Y );	    // 将光标移回默认位置.
	}
	return true;
}

// 初始化
bool Win32Input::Initialize( bool isUseCursor )
{
	SetCursorPos( DEF_CURSOR_POS_X , DEF_CURSOR_POS_Y );	// 将光标移动到屏幕中心附近.
	m_CursorRelMove.x = m_CursorRelMove.y = 0;				// 将鼠标相对位移缓冲区清零.
	m_bUseCursor = isUseCursor;
	return true;
}