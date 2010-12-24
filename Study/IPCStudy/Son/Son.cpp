// Son.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Son.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;






int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
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
