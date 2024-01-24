#include<iostream>
#include<string>
#include <windows.h>

using namespace std;


bool isWin11()
{
	NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osInfo;
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");    
	if (NULL != RtlGetVersion) {
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		RtlGetVersion(&osInfo);

		if( osInfo.dwMajorVersion == 10 && osInfo.dwBuildNumber >= 22000 )
		{
			return true;
		}
	}

	return false;
}

void main() {
	int osver= 0.0;
	NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osInfo;
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");    
	if (NULL != RtlGetVersion) {
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		RtlGetVersion(&osInfo);
		osver = osInfo.dwMajorVersion;

		/*
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
		WORD   wServicePackMajor;
		WORD   wServicePackMinor;
		*/
		cout << "osver:[" << osver << "]" << endl;
		cout << "minorVersion:[" << osInfo.dwMinorVersion << "]" << endl;
		cout << "dwBuildNumber:[" << osInfo.dwBuildNumber << "]" << endl;
		cout << "dwPlatformId:[" << osInfo.dwPlatformId << "]" << endl;
		cout << "szCSDVersion:[" << osInfo.szCSDVersion << "]" << endl;
		cout << "wServicePackMajor:[" << osInfo.wServicePackMajor << "]" << endl;
		cout << "wServicePackMinor:[" << osInfo.wServicePackMinor << "]" << endl;

	}

	bool is11 = isWin11();
	cout << "is win11:[" << is11 << "]" << endl;

}