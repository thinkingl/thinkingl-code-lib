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
	char *ReadText(int hotkeyIndex, char * );	// ����hotkeyIndex����ݼ�����.
	DWORD ReadDWORD(int hotkeyIndex, char * );  // ����hotkeyIndex����ݼ��ļ�ֵ.

};
