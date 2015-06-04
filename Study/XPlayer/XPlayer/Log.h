#pragma once

/** 记录日志. 
*	+by lizhixing@2015-06-04
*/

#include <vector>
#include <string>
#include <sstream>

class CLog
{
public:
	CLog();
	~CLog();

	// 换行.
	CLog& newline();

// 	CLog& operator <<(const char* pAnsiStr);
// 	CLog& operator <<( int val );
// 	CLog& operator <<(const wchar_t* pWStr);
// 	CLog& operator <<(void*);	// 16进制输出.

	/**	一般的日志输出, 字符串的话只支持UTF-8的. */
	template< typename _T >
	CLog& operator << (_T logItem)
	{
		m_curLine << logItem;
		return *this;
	}

	// 所有日志输出到一个完整的字符串.
	void toString( std::wstring& str );

private:
	// 将日志全缓存下来.
	typedef std::vector<std::wstring> CLogList;
	CLogList m_logList;

	// 本行日志.
	std::wstringstream  m_curLine;
};

static CLog s_log;

// 写日志就用这个函数.
inline CLog& LOG()
{
	return s_log.newline();
}
