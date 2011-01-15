#pragma once

#include "KedaGiftDefine.h"

class CEmployerInput
{
public:
	CEmployerInput(void);
	~CEmployerInput(void);

	/** 获取所有没有中奖的人. */
	TEmployerList GetAllNoGiftEmployer() const;

private:
	/** 文件方式输入. */
	bool FileInput( LPCTSTR strFilePath );

	/** 字符串方式输入.*/


};

