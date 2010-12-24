// Parent.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Parent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

void CreateProcessTest()
{

	SECURITY_ATTRIBUTES saAttr; 
	BOOL fSuccess; 
	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	HANDLE hChildStdInRead, hChildStdInWrite, hChildStdOutRead, hChildStdOutWrite, hChildStdErrRead, hChildStdErrWrite;
	// create pipes for child process.
	BOOL bRet = CreatePipe( &hChildStdInRead, &hChildStdInWrite, &saAttr, 0 );
	ASSERT( bRet );
	// 父进程向子进程标准输入管道里写东西，子进程只读，不继承写的管道。
	SetHandleInformation( hChildStdInWrite, HANDLE_FLAG_INHERIT, 0 );

	bRet = CreatePipe( &hChildStdOutRead, &hChildStdOutWrite, &saAttr, 0 );
	ASSERT( bRet );
	// 和上边同理，相反。
	SetHandleInformation( hChildStdOutWrite, HANDLE_FLAG_INHERIT, 0 );

	bRet = CreatePipe( &hChildStdErrRead, &hChildStdErrWrite, &saAttr, 0 );
	ASSERT( bRet );
	SetHandleInformation( hChildStdErrRead, HANDLE_FLAG_INHERIT, 0 );


	STARTUPINFO siStartInfo;
	PROCESS_INFORMATION pi;

	ZeroMemory( &siStartInfo, sizeof(siStartInfo) );
	siStartInfo.cb = sizeof(siStartInfo);
 	siStartInfo.hStdInput = hChildStdInRead;
 	siStartInfo.hStdOutput = hChildStdOutWrite;
 	siStartInfo.hStdError = hChildStdErrWrite;
 	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;


	ZeroMemory( &pi, sizeof(pi) );
	
	// 	if( argc != 2 )
	// 	{
	// 		printf("Usage: %s [cmdline]\n", argv[0]);
	// 		return;
	// 	}

	LPCTSTR SON_PROCESS = _T( "IPCStudy_Son.exe" );
	TCHAR arCmdLine[MAX_PATH] = { 0 };
	_tcscpy( arCmdLine, SON_PROCESS );




	// Start the child process. 
	if( !CreateProcess( NULL,   // No module name (use command line)
		arCmdLine,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&siStartInfo,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		) 
	{
		printf( "CreateProcess failed (%d)\n", GetLastError() );
		return;
	}

// 	CFile childStdOut( hChildStdOutRead );
// 	CArchive archiveChildStdOut( &childStdOut, CArchive::load );
// 
// 	// write 
// 	CFile childStdIn( hChildStdInWrite );
// 	CArchive archiveChildStdIn( &childStdIn, CArchive::store );
// 	archiveChildStdIn << _T( "To my child, Hello Dear!" );
// 
// 	archiveChildStdIn.Flush();
// 
// 
// 	CString strOut;
// 	archiveChildStdOut >> strOut;
// 	cout << "Receive child's message: " << (LPCTSTR)strOut;

//	CloseHandle( hChildStdInRead );
	CloseHandle( hChildStdOutWrite );
	CloseHandle( hChildStdErrWrite );

	DWORD dwRead, dwWritten;
	char message[255] = {0};
	strcpy( message, "Hello child!" );
	bRet = WriteFile( hChildStdInWrite, message, sizeof( message ), &dwWritten, NULL );
	ASSERT( bRet );
	CloseHandle( hChildStdInRead );
	CloseHandle( hChildStdInWrite );

	Sleep( 100 );
//	CloseHandle( hChildStdOutWrite );
	memset( message, 0, sizeof( message ) );
	bRet = ::ReadFile( hChildStdOutRead, message, sizeof( message ) - 1, &dwRead, NULL );
//	ASSERT( bRet );

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );

	Sleep( 100000 );

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

}

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
		CreateProcessTest();
	}

	return nRetCode;
}
