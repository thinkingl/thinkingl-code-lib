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

	void InitVFWErrorMsg();
	void InitEVMsg();

	std::wstring GetErrorMsg( HRESULT hr );
	std::wstring GetEVMsg(long evCode);

	static std::wstring ErrorMsg(HRESULT hr);

	static std::wstring EVMsg( long evCode );
private:
	typedef std::map<HRESULT, std::wstring> CHResultErrorMsgMap;
	CHResultErrorMsgMap m_hresultErrorMsgTable;

	CHResultErrorMsgMap m_evMsgTable;
};

