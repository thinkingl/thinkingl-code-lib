#pragma once

/** 随机选号. */

#include "KedaGiftDefine.h"
#include "EmployerGiftConfig.h"

class CRandomPick
{
public:
	CRandomPick(void);
	~CRandomPick(void);
		

	/** 设置列表. */
	void SetEmployerList( const TEmployerList& employerList );

	/** 获取随机人员中奖.. 
	*	每个人只有一次中奖机会,不重复.
	*/
	CEmployer GetLukyOne( int raund );

	/** 随机选取一个还没有中奖人,用于显示. */
	CEmployer RandomPickOneNoGiftToShow() const;

	/** 重置中奖情况.. */
	void ResetGift();

private:
	/** 随机选择一个序号. */
	int GetRadomNum( int nBegin, int nEnd ) const;

	/** 初始化已经中奖的名单. */
	void InitAllReadyGifted();

	/** 处理已经中奖的同志. */
	void HandleGiftedLucyMan( int nIndex );

	/** 写日志. */
	CArchive& Log();
private:
	/** 所有员工. 
	*	从 0 - m_nLuckyEnd 是已经中奖被排除的人员序号.
	*/
	TEmployerList m_tAllEmployer;

	/** 已经中奖的人被移到前面. 这个是第一个没有中奖的人. */
	int m_nFirstNoGift;

	/** 配置文件. */
	CEmployerGiftConfig m_giftConfig;

	/** 日志文件. */
	CFile m_fileLog;
	CArchive *m_pArchiveLog;
};

