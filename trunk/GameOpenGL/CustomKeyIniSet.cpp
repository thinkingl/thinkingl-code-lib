#include "StdAfx.h"
#include "CustomKeyIniSet.h"

CustomKeyIniSet::CustomKeyIniSet(void)
{
}

CustomKeyIniSet::~CustomKeyIniSet(void)
{
}

//  ��ȡ��x����ݼ��Ĺ���.
char * CustomKeyIniSet::ReadText( int Index , char *title)
{
	char strHotkeyIndex[20] ;
	sprintf_s( strHotkeyIndex,20 , "Hotkey%d" , Index);
	return CIniSet::ReadText( strHotkeyIndex , title );
}

//  ��ȡ��hotkeyIndex����ݼ��ļ�ֵ.
DWORD CustomKeyIniSet::ReadDWORD( int Index , char *title)
{
	char strHotkeyIndex[20];
	sprintf_s( strHotkeyIndex,20, "Hotkey%d" , Index );
	return static_cast<DWORD>( CIniSet::ReadInt( strHotkeyIndex , title ));
}


