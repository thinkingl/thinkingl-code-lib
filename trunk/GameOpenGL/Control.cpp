
//  Control.cpp
//  ������.
//  ��֮��
//  ���������Ϣ�ķ����ͷ���.
//  ���˺ܾ�,����������λ�ڵײ�.Ϊ����ģ���ṩ������Ĳ���ָ��.
//  ���������ʵ���û��Ŀ��ƶ���.


#include ".\control.h"
//#using <mscorlib.dll>

#define CUSTOM_KEY_FILE "ini\\CustomKey.ini"	// ��ݼ������ļ�·�����ļ���
#define GAME_SETUP_FILE "ini\\Setup.ini"

Control::Control(void)
{
}

Control::~Control(void)
{
}

// ��������Ƿ񱻷�����
int * Control::GetOrderList( )
{
	nOrderList[0] = 0;		// �������Ŀ��ʼ��Ϊ0;
	for( int i = 0 ; i < CMD_NUM ; i++)
	{		
		//  �������,��ѯ���������,��������ݼ�,Ȼ�������ݼ��Ƿ񱻰���.
		//  ���������ݼ��Ĵ洢��ʽ���� CHotKeyCtl::GetHotKey()const �ķ���ֵ�ĸ�ʽ: �ڵ�16λ��,��8λ�������ֵ,��8λ�ǹ��ܼ���־λ.
		if( m_inputReader.IsKeyDown( LOBYTE(LOWORD( m_nCmdDictionary[i] )) , HIBYTE( LOWORD( m_nCmdDictionary[i]))))
		{
			nOrderList[0] ++;
			nOrderList[nOrderList[0]] = i;
		}
	}
	return nOrderList;
}

// ��ʼ��.
bool Control::Initialize( HWND hWnd)
{
	//  ��ʼ��������ֵ�.�������ļ��ж�ȡ����,��ʼ����������.
	CustomKeyIniSet CustomHotkeyFile;
	CustomHotkeyFile.Open( CUSTOM_KEY_FILE );
	for ( int i = 0 ; i < CMD_NUM ; i++ )
	{
		if( CustomHotkeyFile.ReadText( i , "Effect"))
			m_nCmdDictionary[i] = CustomHotkeyFile.ReadDWORD( i , "HotkeyCode" );
		else
			m_nCmdDictionary[i] = 0;
	}

	// ��ȡ���������.
	CustomKeyIniSet InputSetupFile;
	InputSetupFile.Open(GAME_SETUP_FILE);
	m_bInputMode = static_cast<bool>(InputSetupFile.ReadInt( "Input" , "InputMode" ));	// ��ȡ����ģʽ
	m_bUseCursor = static_cast<bool>(InputSetupFile.ReadInt( "Input" , "UseCursor" ));	// ��ȡʹ�ù�������.
	// ����ĳ�ʼ��.
	m_inputReader.Initialize( hWnd,m_bInputMode ,m_bUseCursor);
	return true;
}