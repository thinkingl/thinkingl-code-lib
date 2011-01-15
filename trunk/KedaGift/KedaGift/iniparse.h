/*==========================================================
类名    ： CIniParse
功能    ： 封装了对ini配置文件的读写.

    
主要接口：
	
备注    ：
	  -------------------------------------------------------------------------------------------------------
	  修改记录：
	  日  期		版本		修改人		走读人        修改记录
	  2007/07/27	V0.0		李之兴						创建(基类形式使用.)
	  2008/05/05	v0.9		李之兴						改造为独立易用的类.
===========================================================*/
#pragma once


class CIniParse   
{
public:

	enum 
	{
		Max_Buffer_Len = 4 * 1024,	// 最大的接收数据缓冲区大小.
	};

	CIniParse();
	
	// 带参数构造,
	// 参数:
	// LPCTSTR	strIniFileName 配置文件名.
	// LPCTSTR ModuleName 模块名,如果为NULL,则默认使用当前进程模块.
	CIniParse( LPCTSTR strIniFileName, LPCTSTR ModuleName );
	
	CIniParse( LPCTSTR strIniFileName, HMODULE hModuleHandle );
	// 参数:
	// LPCTSTR strIniFilePath : 完整的配置文件路径.
	CIniParse( LPCTSTR strIniFilePath );
	virtual ~CIniParse();

	// 读取INI中的整数.
	int ReadInt( LPCTSTR strApp, LPCTSTR strKey, int nDefaultValue );

	// 写入整数.
	BOOL WriteInt( LPCTSTR strApp, LPCTSTR strKey, int nValue );

	// 读取String
	CString ReadString( LPCTSTR strApp, LPCTSTR strKey, LPCTSTR strDefaultValue = NULL );
	// 写入字符串.
	BOOL WriteString( LPCTSTR strApp, LPCTSTR strKey, LPCTSTR strValue );

	// 读取APP列表.
	BOOL ReadAppList( CStringArray& caAppList );

	// 读取指定APP下的Key列表.
	BOOL ReadKeyList( LPCTSTR strApp, CStringArray& caKeyList );

	// 删除某个APP.
	BOOL DelApp( LPCTSTR strApp );

	// 删除某个Key.
	BOOL DelKey( LPCTSTR strApp, LPCTSTR strKey );

	// 设置配置文件的文件名或相对路径.
	// 路径的其余部分将自动补全为配置文件所属模块所在路径.
	// 如果所属模块为NULL,则使用调用者进程作为默认模块.
	// 如果本模块路径无法获取,就是用系统路径.(需要验证,windows受限用户可能会无法访问系统路径??? by lzx)
	void SetIniFileName( LPCTSTR strIniFileName );
	// 设置绝对路径.
	void SetIniFilePath( LPCTSTR strIniFilePath );
	
	// 设置配置文件所属的模块.
	// 设置后会刷新配置文件路径.
	void SetOwnerModule( LPCTSTR strModuleName );
	void SetOwnerModule( HMODULE hModuleHandle );
	
	// 获取配置文件的路径.
	CString GetIniFilePath();

protected:
	

	// 解析缓冲区..
	// 获取到的所有app或者key列表在缓冲区中是一个个字符串.最终由 "\0\0" 结尾.
	BOOL ParseBuffer( TCHAR aBuffer[], int nBuffLen, CStringArray& stringArray );



protected:

	// 配置文件完整路径.
	CString m_strIniFilePath;

	// 從屬者模块句柄.
	HMODULE m_hOwnerHandle;

	// 配置文件名.
	CString m_strIniFileName;

	// 根据相对路径获得配置文件的绝对路径..
	CString GetIniFilePath( LPCTSTR sFileName);
	
	// 根据模块句柄获取模块路径.
	CString GetModuleDirectory(HMODULE hModuleHandle );

};
