#pragma once
#include "iapplication.h"
#include "ClassFactory.h"

#include <afxwin.h>

class CWin32Application :
	public IApplication
{
public:
	CWin32Application(void);
	virtual ~CWin32Application(void);

	virtual int RunWebFetch();

private:
	
};
