#pragma once
#include "itask.h"

#include "portabledefine.h"

/** ������ҳ������. 
*	����ҳ�н�������ͼͼƬURL��ģʽ.
*/

class CParseMainPageTask :
	public ITask
{
public:
	CParseMainPageTask( ctstring strMainPageUrl );
	virtual ~CParseMainPageTask(void);

private:

};

