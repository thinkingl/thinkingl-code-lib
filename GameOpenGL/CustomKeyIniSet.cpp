#include "StdAfx.h"
#include "CustomKeyIniSet.h"

CustomKeyIniSet::CustomKeyIniSet(void)
{
}

CustomKeyIniSet::~CustomKeyIniSet(void)
{
}

//  读取第x个快捷键的功能.
char * CustomKeyIniSet::ReadText( int Index , char *title)
{
	char strHotkeyIndex[20] ;
	sprintf_s( strHotkeyIndex,20 , "Hotkey%d" , Index);
	return CIniSet::ReadText( strHotkeyIndex , title );
}

//  读取第hotkeyIndex个快捷键的键值.
DWORD CustomKeyIniSet::ReadDWORD( int Index , char *title)
{
	char strHotkeyIndex[20];
	sprintf_s( strHotkeyIndex,20, "Hotkey%d" , Index );
	return static_cast<DWORD>( CIniSet::ReadInt( strHotkeyIndex , title ));
}


