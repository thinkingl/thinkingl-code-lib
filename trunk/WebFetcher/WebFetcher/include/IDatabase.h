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
	//	PageStateFeching,	// 正在获取。
	};

	/** 数据库中的记录项。 */
	struct TUrlRecordItem
	{
		tstring m_strUrl;		// url，完整url。
		tstring m_strCachePath;	// 缓存路径，相对路径。
		tstring m_strSavePath;	// 保存路径，相对路径。
		EWebpageState m_eState;	// 状态。
	};
	typedef std::vector< TUrlRecordItem > TUrlRecordItemList;

//	virtual BOOL OpenDatabase( LPCTSTR strPath ) = 0;
	// 打开。
	virtual BOOL Open() = 0;

	// 关闭。 
	virtual BOOL Close() = 0;

	/** 根据指定状态获取指定条数的 url记录。*/
	virtual BOOL GetUrlByStateByNum( EWebpageState eState, 
		TUrlRecordItemList& tRecordItemList, int nCount ) = 0;

	/** 查找一条url记录。 */
	virtual BOOL SearchUrl( LPCTSTR strUrl, TUrlRecordItem& item ) = 0;

	// 添加一条缓存记录。


};
