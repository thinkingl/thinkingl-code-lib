enum EMessage
{
	WM_REFRESH_EMPLOY = WM_USER + 1000,	// ˢ�½�������ʾ����.
};

class CEmployer
{
public:
	CEmployer();

	/** ����*/
	CString m_strKedaNo;

	/** ���� */
	CString m_strName;
	/** �н����. */
	CTime m_time;
};