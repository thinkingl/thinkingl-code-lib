#pragma once

#include <vector>


enum EMessage
{
	WM_REFRESH_EMPLOY = WM_USER + 1000,	// ˢ�½�������ʾ����.
};

class CEmployer
{
public:
	CEmployer()
	{
		m_bAbsent = FALSE;
	}

	/** ����*/
	CString m_strKedaNo;

	/** ���� */
	CString m_strName;
	/** �н����. */
	CTime m_timeLuck;

	/** �콱���. */
	CTime m_timeGetGift;

	/** �Ƿ�ȱϯ. */
	BOOL m_bAbsent;
};


typedef std::vector< CEmployer > TEmployerList;

typedef std::vector< CString > TKedaNoList;