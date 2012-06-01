
//  Control.cpp
//  控制类.
//  李之兴
//  负责控制信息的分析和发送.
//  想了很久,决定把他定位在底层.为其他模块提供处理过的操作指令.
//  在这里可以实现用户的控制定制.


#include ".\control.h"
//#using <mscorlib.dll>

#define CUSTOM_KEY_FILE "ini\\CustomKey.ini"	// 快捷键配置文件路径和文件名
#define GAME_SETUP_FILE "ini\\Setup.ini"

Control::Control(void)
{
}

Control::~Control(void)
{
}

// 检测命令是否被发出。
int * Control::GetOrderList( )
{
	nOrderList[0] = 0;		// 命令的数目初始化为0;
	for( int i = 0 ; i < CMD_NUM ; i++)
	{		
		//  由命令号,查询命令翻译数组,获得命令快捷键,然后分析快捷键是否被按下.
		//  其中命令快捷键的存储方式采用 CHotKeyCtl::GetHotKey()const 的返回值的格式: 在低16位中,低8位是虚拟键值,高8位是功能键标志位.
		if( m_inputReader.IsKeyDown( LOBYTE(LOWORD( m_nCmdDictionary[i] )) , HIBYTE( LOWORD( m_nCmdDictionary[i]))))
		{
			nOrderList[0] ++;
			nOrderList[nOrderList[0]] = i;
		}
	}
	return nOrderList;
}

// 初始化.
bool Control::Initialize( HWND hWnd)
{
	//  初始化命令翻译字典.从配置文件中读取数据,初始化翻译数组.
	CustomKeyIniSet CustomHotkeyFile;
	CustomHotkeyFile.Open( CUSTOM_KEY_FILE );
	for ( int i = 0 ; i < CMD_NUM ; i++ )
	{
		if( CustomHotkeyFile.ReadText( i , "Effect"))
			m_nCmdDictionary[i] = CustomHotkeyFile.ReadDWORD( i , "HotkeyCode" );
		else
			m_nCmdDictionary[i] = 0;
	}

	// 读取输入的配置.
	CustomKeyIniSet InputSetupFile;
	InputSetupFile.Open(GAME_SETUP_FILE);
	m_bInputMode = static_cast<bool>(InputSetupFile.ReadInt( "Input" , "InputMode" ));	// 读取输入模式
	m_bUseCursor = static_cast<bool>(InputSetupFile.ReadInt( "Input" , "UseCursor" ));	// 读取使用光标的设置.
	// 输入的初始化.
	m_inputReader.Initialize( hWnd,m_bInputMode ,m_bUseCursor);
	return true;
}