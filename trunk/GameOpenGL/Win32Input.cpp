#include "Win32Input.h"

const int DEF_CURSOR_POS_X = 400;
const int DEF_CURSOR_POS_Y = 300;

Win32Input::Win32Input(void)
{
}

Win32Input::~Win32Input(void)
{
}


// ˢ�»�����
bool Win32Input::Update()
{
	if (!m_bUseCursor)		// ���û�Ҫʹ�������ʱ,Ҫ��ֹ���ù��λ��. 
	{
		GetCursorPos( &m_CursorPos );								// ��õ�ǰ���λ��.
		m_CursorRelMove.x = DEF_CURSOR_POS_X - m_CursorPos.x;		// ���X,Y���ϵ����λ��.
		m_CursorRelMove.y = DEF_CURSOR_POS_Y - m_CursorPos.y;
		SetCursorPos( DEF_CURSOR_POS_X , DEF_CURSOR_POS_Y );	    // ������ƻ�Ĭ��λ��.
	}
	return true;
}

// ��ʼ��
bool Win32Input::Initialize( bool isUseCursor )
{
	SetCursorPos( DEF_CURSOR_POS_X , DEF_CURSOR_POS_Y );	// ������ƶ�����Ļ���ĸ���.
	m_CursorRelMove.x = m_CursorRelMove.y = 0;				// ��������λ�ƻ���������.
	m_bUseCursor = isUseCursor;
	return true;
}