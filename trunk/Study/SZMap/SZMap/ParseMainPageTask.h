#pragma once
#include "itask.h"

#include "portabledefine.h"

/** 解析主页的任务. 
*	从主页中解析出地图图片URL的模式.
*/

class CParseMainPageTask :
	public ITask
{
public:
	CParseMainPageTask( ctstring strMainPageUrl );
	virtual ~CParseMainPageTask(void);

private:

};

