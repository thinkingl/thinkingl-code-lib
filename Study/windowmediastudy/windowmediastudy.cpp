// windowmediastudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <Vfw.h>
#include <iostream>

using namespace std;

#pragma comment( lib, "vfw32.lib")

bool HasVideoInput()
{
	// 创建捕获窗口.
	HWND hCapWnd = capCreateCaptureWindow(_T("Cap Wnd"), WS_POPUP, 0, 0, 256, 256, NULL, 0);
	if (NULL == hCapWnd)
	{
		cout << "Create Capture Window fail! er: "<< GetLastError() << endl;
		return false;
	}

	// 尝试连接驱动.
	bool hasVideoInput = false;
	for (int i = 0; i < 10; ++i)
	{
		if (capDriverConnect(hCapWnd, i))
		{
			hasVideoInput = true;
			break;
		}
	}

	// 释放.
	capDriverDisconnect(hCapWnd);
	DestroyWindow(hCapWnd);

	return hasVideoInput;
}

void EnumInstalledCaptureDrivers()
{
	for (int i = 0; i < 10; ++i)
	{
		wchar_t szDeviceName[MAX_PATH] = { 0 };
		wchar_t szDeviceVersion[MAX_PATH] = { 0 };
		if (capGetDriverDescription(i, szDeviceName, sizeof(szDeviceName), szDeviceVersion, sizeof(szDeviceVersion)))
		{
			cout << "Capture driver " << i << " Device name: " << szDeviceName << " device version: " << szDeviceVersion << endl;
		}
		else
		{
			cout << "Get capture driver " << i << "'s description fail! er: " << GetLastError() << endl;
			break;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{

	// 是否支持图像采集.
	bool isSupportVideoInput = HasVideoInput();
	cout << "Is support Video Input : " << ( isSupportVideoInput ? "True" : "False") << endl;

	// 枚举安装的驱动.
	EnumInstalledCaptureDrivers();

	system("pause");
	return 0;
}

