#pragma once

#include "portabledefine.h"

class CCommon
{
public:
	CCommon(void);
	virtual ~CCommon(void);

	static void Utf8toUtf16( LPCSTR strUtf8, wstring& strUtf16 );

	static void Utf16toUtf8( LPCWSTR strUtf16, string& strUtf8 );

	static int StrNCmpNocase( LPCTSTR str1, LPCTSTR str2, int nLen );

};
