#include "stdafx.h"
#include "Log.h"


CLog::CLog()
{
}


CLog::~CLog()
{
}

CLog& CLog::newline()
{
	if ( !m_curLine.str().empty() )
	{
		m_logList.push_back(m_curLine.str());
		m_curLine = std::wstringstream();
	}
	return *this;
}
// 
// CLog& CLog::operator<<(int val)
// {
// 	m_curLine << val;
// 	return *this;
// }
// 
// CLog& CLog::operator<<(const char* pAnsiLog)
// {
// 	m_curLine << pAnsiLog;
// 	return *this;
// }

void CLog::toString(std::wstring& str)
{
	str.clear();
	for (size_t i = 0; i < m_logList.size(); ++i )
	{
		str += m_logList[i];
		str += L"\r\n";
	}
	str += m_curLine.str();
}