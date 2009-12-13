#pragma once

#include "portabledefine.h"

class IDatabase
{
public:
	IDatabase(void);
	virtual ~IDatabase(void);

	enum EWebpageState
	{
		PageStateSaved,		// 已经保存。
		PageStateCached,	// 已经被缓存。
		PageStateFeching,	// 正在获取。
	};

//	virtual BOOL OpenDatabase( LPCTSTR strPath ) = 0;
	// 打开。
	virtual BOOL Open() = 0;

	// 关闭。 
	virtual BOOL Close() = 0;

	// 获取一条缓存记录。

	// 添加一条缓存记录。


};
