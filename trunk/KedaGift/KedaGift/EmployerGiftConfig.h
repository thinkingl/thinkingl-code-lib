#pragma once

#include "KedaGiftDefine.h"
#include "iniparse.h"


class CEmployerGiftConfig
{
public:
	CEmployerGiftConfig(void);
	~CEmployerGiftConfig(void);

	/** ��ȡ�Ѿ��н�����. */
	TEmployerList GetLuckies();

	/** ���һ���н�����. */
	void AddLuckMan( const CEmployer& lucyMan );

	/** ����. */
	void Reset();

	/** ��ȡһ���Ѿ��н�����. */
	BOOL GetEmployerLucky( LPCTSTR strKedaNo, CEmployer& employer );

private:
	/** ini�����ļ�����. */
	CIniParse m_iniFileParser;
};

