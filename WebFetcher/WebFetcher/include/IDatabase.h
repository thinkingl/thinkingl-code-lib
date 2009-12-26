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
		PageStateWaiting,	// 正在下载，仍没有下载到本地。
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

	/** 添加一条记录。 */
	virtual BOOL AddRecord( const TUrlRecordItem& item ) = 0;

	/** 修改一条已经存在的记录。 */
	virtual BOOL ModifyRecord( const TUrlRecordItem& item ) = 0;

	/** 只修改一条记录中的状态。 */
	virtual BOOL ModifyWebpageState( LPCTSTR strUrl, EWebpageState eStat ) = 0;

	/** 添加一条失败url记录. */
	virtual BOOL AddFailUrl( LPCTSTR strFailUrl, LPCTSTR strPageOfUrl ) = 0;

};
