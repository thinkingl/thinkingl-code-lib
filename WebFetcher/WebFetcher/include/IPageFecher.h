#pragma once

#include "portabledefine.h"

class IPageFecher
{
public:
	virtual ~IPageFecher()
	{

	}

	/** 抓取一个网页。
	*/
	virtual BOOL FetchOnePage() = 0;

	/** 是否还有工作。 */
	virtual BOOL HasPageWaiting() = 0;
	
};
