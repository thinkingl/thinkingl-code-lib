// Son.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Son.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;






int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		const int BUFSIZE = 4000;
		CHAR chBuf[BUFSIZE]; 
		DWORD dwRead, dwWritten; 
		HANDLE hStdin, hStdout; 
		BOOL fSuccess; 

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		hStdin = GetStdHandle(STD_INPUT_HANDLE); 
		if ((hStdout == INVALID_HANDLE_VALUE) || 
			(hStdin == INVALID_HANDLE_VALUE)) 
			ExitProcess(1); 

		for (;;) 
		{ 
			// Read from standard input. 
			fSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL); 
			if (! fSuccess || dwRead == 0) 
				break; 

			// Write to standard output. 
			fSuccess = WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL); 
			if (! fSuccess) 
				break; 
		} 

	}

	return nRetCode;
}
