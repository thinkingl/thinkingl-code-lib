#pragma once

#include "KedaGiftDefine.h"

class CEmployerInput
{
public:
	CEmployerInput(void);
	~CEmployerInput(void);

	/** ��ȡ����û���н�����. */
	TEmployerList GetAllNoGiftEmployer() const;

private:
	/** �ļ���ʽ����. */
	bool FileInput( LPCTSTR strFilePath );

	/** �ַ�����ʽ����.*/


};

