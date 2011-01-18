#pragma once

#include <vector>


enum EMessage
{
	WM_REFRESH_EMPLOY = WM_USER + 1000,	// 刷新界面上显示的人.
};

class CEmployer
{
public:
	CEmployer()
	{
		m_bAbsent = FALSE;
	}

	/** 工号*/
	CString m_strKedaNo;

	/** 姓名 */
	CString m_strName;
	/** 中奖情况. */
	CTime m_timeLuck;

	/** 领奖情况. */
	CTime m_timeGetGift;

	/** 是否缺席. */
	BOOL m_bAbsent;
};


typedef std::vector< CEmployer > TEmployerList;

typedef std::vector< CString > TKedaNoList;