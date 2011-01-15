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

private:
	/** ini�����ļ�����. */
	CIniParse m_iniFileParser;
};

