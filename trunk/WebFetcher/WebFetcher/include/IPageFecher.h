#pragma once

#include "portabledefine.h"

class IPageFecher
{
public:

	/** ץȡһ����ҳ��
	*/
	virtual BOOL FetchOnePage() = 0;

	/** �Ƿ��й����� */
	virtual BOOL HasPageWaiting() = 0;
	
};
