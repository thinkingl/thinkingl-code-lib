#pragma once
#include ".\include\iniset.h"

class CustomKeyIniSet :
	public CIniSet
{
public:
	CustomKeyIniSet(void);

public:
	~CustomKeyIniSet(void);

public:
	char *ReadText(int hotkeyIndex, char * );	// 读第hotkeyIndex个快捷键功能.
	DWORD ReadDWORD(int hotkeyIndex, char * );  // 读第hotkeyIndex个快捷键的键值.

};
