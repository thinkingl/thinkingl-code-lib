/*==========================================================
����    �� CIniParse
����    �� ��װ�˶�ini�����ļ��Ķ�д.

    
��Ҫ�ӿڣ�
	
��ע    ��
	  -------------------------------------------------------------------------------------------------------
	  �޸ļ�¼��
	  ��  ��		�汾		�޸���		�߶���        �޸ļ�¼
	  2007/07/27	V0.0		��֮��						����(������ʽʹ��.)
	  2008/05/05	v0.9		��֮��						����Ϊ�������õ���.
===========================================================*/
#pragma once


class CIniParse   
{
public:

	enum 
	{
		Max_Buffer_Len = 4 * 1024,	// ���Ľ������ݻ�������С.
	};

	CIniParse();
	
	// ����������,
	// ����:
	// LPCTSTR	strIniFileName �����ļ���.
	// LPCTSTR ModuleName ģ����,���ΪNULL,��Ĭ��ʹ�õ�ǰ����ģ��.
	CIniParse( LPCTSTR strIniFileName, LPCTSTR ModuleName );
	
	CIniParse( LPCTSTR strIniFileName, HMODULE hModuleHandle );
	// ����:
	// LPCTSTR strIniFilePath : �����������ļ�·��.
	CIniParse( LPCTSTR strIniFilePath );
	virtual ~CIniParse();

	// ��ȡINI�е�����.
	int ReadInt( LPCTSTR strApp, LPCTSTR strKey, int nDefaultValue );

	// д������.
	BOOL WriteInt( LPCTSTR strApp, LPCTSTR strKey, int nValue );

	// ��ȡString
	CString ReadString( LPCTSTR strApp, LPCTSTR strKey, LPCTSTR strDefaultValue = NULL );
	// д���ַ���.
	BOOL WriteString( LPCTSTR strApp, LPCTSTR strKey, LPCTSTR strValue );

	// ��ȡAPP�б�.
	BOOL ReadAppList( CStringArray& caAppList );

	// ��ȡָ��APP�µ�Key�б�.
	BOOL ReadKeyList( LPCTSTR strApp, CStringArray& caKeyList );

	// ɾ��ĳ��APP.
	BOOL DelApp( LPCTSTR strApp );

	// ɾ��ĳ��Key.
	BOOL DelKey( LPCTSTR strApp, LPCTSTR strKey );

	// ���������ļ����ļ��������·��.
	// ·�������ಿ�ֽ��Զ���ȫΪ�����ļ�����ģ������·��.
	// �������ģ��ΪNULL,��ʹ�õ����߽�����ΪĬ��ģ��.
	// �����ģ��·���޷���ȡ,������ϵͳ·��.(��Ҫ��֤,windows�����û����ܻ��޷�����ϵͳ·��??? by lzx)
	void SetIniFileName( LPCTSTR strIniFileName );
	// ���þ���·��.
	void SetIniFilePath( LPCTSTR strIniFilePath );
	
	// ���������ļ�������ģ��.
	// ���ú��ˢ�������ļ�·��.
	void SetOwnerModule( LPCTSTR strModuleName );
	void SetOwnerModule( HMODULE hModuleHandle );
	
	// ��ȡ�����ļ���·��.
	CString GetIniFilePath();

protected:
	

	// ����������..
	// ��ȡ��������app����key�б��ڻ���������һ�����ַ���.������ "\0\0" ��β.
	BOOL ParseBuffer( TCHAR aBuffer[], int nBuffLen, CStringArray& stringArray );



protected:

	// �����ļ�����·��.
	CString m_strIniFilePath;

	// �Č���ģ����.
	HMODULE m_hOwnerHandle;

	// �����ļ���.
	CString m_strIniFileName;

	// �������·����������ļ��ľ���·��..
	CString GetIniFilePath( LPCTSTR sFileName);
	
	// ����ģ������ȡģ��·��.
	CString GetModuleDirectory(HMODULE hModuleHandle );

};
