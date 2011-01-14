enum EMessage
{
	WM_REFRESH_EMPLOY = WM_USER + 1000,	// 刷新界面上显示的人.
};

class CEmployer
{
public:
	CEmployer();

	/** 工号*/
	CString m_strKedaNo;

	/** 姓名 */
	CString m_strName;
	/** 中奖情况. */
	CTime m_time;
};