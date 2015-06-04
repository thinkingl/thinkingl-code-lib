#pragma once

/**´íÎóÂë.
*	+by lizhixing@2015-06-04
*/
#include <string>
#include <map>

class CError
{
public:
	CError();
	~CError();

	void Init();

	std::wstring GetErrorMsg( HRESULT hr );

	static std::wstring ErrorMsg(HRESULT hr);

private:
	typedef std::map<HRESULT, std::wstring> CHResultErrorMsgMap;
	CHResultErrorMsgMap m_hresultErrorMsgTable;
};

