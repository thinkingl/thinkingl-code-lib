// IncredibuildCrack.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "IncredibuildCrack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GetIncrediBuildLicKey(LPTSTR lpszKey, int nBufLen);
// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣

			std::wcout<<_T("IncrediBuild Trail Reset Tool")<<std::endl;
			std::wcout<<std::endl;
			std::wcout<<_T("For Version 4.0 !")<<std::endl;
			std::wcout<<std::endl;
			std::wcout<<_T("Please run this program as administrator")<<std::endl;

			std::wcout << _T("Any problem, please contact thinkingzx@gmail.com !") << std::endl;
			std::cout << "���κ�����, ����ϵ lizhixing@kedacom.com " << std::endl;

			std::wcout<<std::endl;
			std::wcout<<std::endl;    

			TCHAR szOut[MAX_PATH] = { 0 };
			GetIncrediBuildLicKey(szOut, MAX_PATH);

			HKEY hKey;
			bool bSucc = true;
			// HKCR\Interface\{23DE9F4B-25F9-4163-BF69-01639BB2B8BA}
			if (ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, _T("Interface\\{23DE9F4B-25F9-4163-BF69-01639BB2B8BA}\\ProxyStubClsid32"), &hKey))
			{
				std::wcout<<_T("Registry Open Failed!")<<std::endl;
				system("pause");
				return 0;
			}
			if (ERROR_SUCCESS != RegSetValue(hKey, NULL, REG_SZ, szOut, wcslen(szOut)*sizeof(TCHAR)))
			{
				RegCloseKey(hKey);
				std::wcout<<_T("Registry SetValue Failed!")<<std::endl;
				system("pause");
				return 0;
			}

			RegCloseKey(hKey);

			system("net stop IncrediBuild_Agent");
			system("net start IncrediBuild_Agent");

		}
	}
	else
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: GetModuleHandle ʧ��\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

// #include <iostream>
// #include <tchar.h>
// #include <Windows.h>
// #include <atlcomtime.h>


void GetIncrediBuildLicKey( LPTSTR lpszKey, int nBufLen )
{
	COleDateTime destTime = COleDateTime::GetCurrentTime();

	int year = destTime.GetYear();
	int month = destTime.GetMonth();

	COleDateTimeSpan oneMonth;
	oneMonth.SetDateTimeSpan( 25, 2, 3, 4 );

	COleDateTime dateTime = destTime + oneMonth;

	int d = dateTime.GetDay();
	int y = dateTime.GetYear();
	int m = dateTime.GetMonth();

	DATE date = (DATE)dateTime;

	BYTE * pByte = (BYTE *)&date;

	BYTE * T1 = (BYTE *) (pByte);
	BYTE * T2 = (BYTE *) (pByte + 2);

	DWORD dwM1 = (DWORD) *pByte;
	dwM1 *= (DWORD) *(pByte + 1);
	dwM1 *= (DWORD) *(pByte + 2);
	dwM1 *= (DWORD) *(pByte + 3);

	WORD wM2 = (WORD) *(pByte + 4);
	wM2 *= (WORD) *(pByte + 5);

	WORD wM3 = (WORD) *(pByte + 6);
	wM3 *= (WORD) *(pByte + 7);

	TCHAR szOut[MAX_PATH] = { 0 };

	wsprintf(szOut, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		dwM1,
		wM2,
		wM3,
		*T1, *(T1+1),
		*T2, *(T2+1), *(T2+2), *(T2+3), *(T2+4), *(T2+5)
		);
	lstrcpyn(lpszKey, szOut, nBufLen);
}