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
		m_luckyRaund = 0;
	}

	/** ����*/
	CString m_strKedaNo;

	/** ���� */
	CString m_strName;

	// ����
	CString m_department;

	/** �н����. */
	CTime m_timeLuck;

	/** �콱���. */
	CTime m_timeGetGift;

	/** �Ƿ�ȱϯ. */
	BOOL m_bAbsent;

	/** �н��ִ�. */
	int m_luckyRaund;
};


typedef std::vector< CEmployer > TEmployerList;

typedef std::vector< CString > TKedaNoList;