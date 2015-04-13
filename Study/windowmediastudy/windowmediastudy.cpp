// windowmediastudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <Vfw.h>
#include <iostream>
#include <map>
#include <string>

using namespace std;

#pragma comment( lib, "vfw32.lib")
#pragma comment( lib, "Winmm.lib")

#define WAVE_FORMAT_G723       (unsigned short)0x0042
#define WAVE_FORMAT_G728       (unsigned short)0x0041//
#define WAVE_FORMAT_G729       (unsigned short)0x0044//
#define WAVE_FORMAT_G722       (unsigned short)0x0065//
#define WAVE_FORMAT_G7221C     (unsigned short)0x0070//
#define WAVE_FORMAT_AACLC	   (unsigned short)0xA106//
#define WAVE_FORMAT_AMR        (unsigned short)0x7A22
#define WAVE_FORMAT_AAC_MS	   	(unsigned short)255


struct MP3_WAVEFORMATEX
{
	WAVEFORMATEX wfx;
	unsigned char byRawData[12];
};

//0-4 mp3, 5-6 G.711. 7-8 G.723 9 G.728 10 G.722 11 G.729 12 g.7221c 13 adpcm
const MP3_WAVEFORMATEX g_sAudioFormat[44] =
{
	//Worst Pcm格式 
	{ { WAVE_FORMAT_PCM, 1, 11025L, 22050L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//Worst Mp3格式
	{ { WAVE_FORMAT_MPEGLAYER3, 1, 11025L, 2500L, 1, 0, 12 },
	{ 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x01, 0x02, 0x00, 0x71, 0x05 } },

	//Bad Pcm格式
	{ { WAVE_FORMAT_PCM, 2, 11025L, 44100L, 4, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//Bad Mp3格式
	{ { WAVE_FORMAT_MPEGLAYER3, 2, 11025L, 4000L, 1, 0, 12 },
	{ 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x01, 0x00, 0x71, 0x05 } },

	//Normal Pcm格式
	{ { WAVE_FORMAT_PCM, 2, 22050L, 88200L, 4, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//Normal Mp3格式
	{ { WAVE_FORMAT_MPEGLAYER3, 2, 22050L, 5000L, 1, 0, 12 },
	{ 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x82, 0x00, 0x01, 0x00, 0x71, 0x05 } },

	//Fine Pcm格式
	{ { WAVE_FORMAT_PCM, 1, 32000L, 64000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//Fine Mp3格式
	{ { WAVE_FORMAT_MPEGLAYER3, 1, 32000L, 8000L, 1, 0, 12 },
	{ 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x01, 0x01, 0x00, 0x71, 0x05 } },

	//Best Pcm格式
	{ { WAVE_FORMAT_PCM, 2, 32000L, 128000L, 4, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//Best Mp3格式
	{ { WAVE_FORMAT_MPEGLAYER3, 2, 32000L, 12000L, 1, 0, 12 },
	{ 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0xb0, 0x01, 0x01, 0x00, 0x71, 0x05 } },

	//G.711 PCM 
	{ { WAVE_FORMAT_PCM, 1, 8000L, 16000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//PCM a law格式
	{ { WAVE_FORMAT_ALAW, 1, 8000L, 8000L, 1, 8, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//G.711 PCM 
	{ { WAVE_FORMAT_PCM, 1, 8000L, 16000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//PCM u law格式
	{ { WAVE_FORMAT_MULAW, 1, 8000L, 8000L, 1, 8, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	////G.723 PCM 6000bps
	{ { WAVE_FORMAT_PCM, 1, 8000L, 16000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//G.7231格式
	{ { WAVE_FORMAT_G723, 1, 8000L, 800L, 24, 0, 10 },
	{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	////G.723 PCM 5333bps
	{ { WAVE_FORMAT_PCM, 1, 8000L, 16000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//G.7231格式
	{ { WAVE_FORMAT_G723, 1, 8000L, 666L, 20, 0, 10 },
	{ 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	////G.728 PCM
	{ { WAVE_FORMAT_PCM, 1, 8000L, 16000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//G.728格式
	{ { WAVE_FORMAT_G728, 1, 8000L, 2000L, 5, 0, 0 },
	{ 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	////G.722 PCM
	{ { WAVE_FORMAT_PCM, 1, 16000L, 32000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//G.722格式
	{ { WAVE_FORMAT_G722, 1, 16000L, 8000L, 2, 16, 0 },
	{ 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	////G.729 PCM
	{ { WAVE_FORMAT_PCM, 1, 8000L, 16000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//G.729格式
	{ { WAVE_FORMAT_G729, 1, 8000L, 1000L, 10, 0, 10 },
	{ 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },


	////G.7221 PCM
	{ { WAVE_FORMAT_PCM, 1, 32000L, 64000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//G.7221格式 24K
	{ { WAVE_FORMAT_G7221C, 1, 32000L, 3000L, 2, 16, 0 },
	{ 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	////adPCM 13
	{ { WAVE_FORMAT_PCM, 1, 8000L, 16000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	//adpcm
	{ { WAVE_FORMAT_IMA_ADPCM, 1, 8000L, 1000L, 2, 16, 0 },
	{ 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//AACLC 16k 14
	{ { WAVE_FORMAT_PCM, 1, 16000L, 32000, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ { WAVE_FORMAT_AACLC, 1, 16000L, 48000, 2, 16, 2 },
	{ 0x14, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//AACLC 32k 15
	{ { WAVE_FORMAT_PCM, 1, 32000L, 64000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ { WAVE_FORMAT_AACLC, 1, 32000L, 72000, 2, 16, 2 },
	{ 0x12, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//AACLC 48k 16
	{ { WAVE_FORMAT_PCM, 1, 48000L, 96000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ { WAVE_FORMAT_AACLC, 1, 48000L, 128000, 2, 16, 2 },
	{ 0x11, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//AACLC 44k 17
	{ { WAVE_FORMAT_PCM, 2, 44100L, 176400L, 4, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ { WAVE_FORMAT_AAC_MS, 2, 44100L, 128000, 2, 16, 2 },
	{ 0x11, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//AACLC PCM 16k_2 18
	{ { WAVE_FORMAT_PCM, 2, 16000L, 64000L, 4, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ { WAVE_FORMAT_AACLC, 2, 16000L, 48000, 2, 16, 2 },
	{ 0x14, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//AACLC 32k_2 19
	{ { WAVE_FORMAT_PCM, 2, 32000L, 128000L, 4, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ { WAVE_FORMAT_AACLC, 2, 32000L, 72000, 2, 16, 2 },
	{ 0x12, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//AACLC 48k_2 20
	{ { WAVE_FORMAT_PCM, 2, 48000L, 192000L, 4, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ { WAVE_FORMAT_AACLC, 2, 48000L, 128000, 2, 16, 2 },
	{ 0x11, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	//ARM 21
	{ { WAVE_FORMAT_PCM, 1, 8000L, 16000L, 2, 16, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ { WAVE_FORMAT_AMR, 1, 8000L, 16000L, 1, 8, 0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
};

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

void PrintFormats(DWORD dwFormats)
{
	static map<DWORD, string> formatNameTable;
	if ( formatNameTable.empty())
	{
		formatNameTable[WAVE_FORMAT_1M08] = "11.025 kHz, Mono,   8-bit";
		formatNameTable[WAVE_FORMAT_1S08] = "11.025 kHz, Stereo, 8-bit";
		formatNameTable[WAVE_FORMAT_1M16] = "11.025 kHz, Mono,   16-bit";
		formatNameTable[WAVE_FORMAT_1S16] = "11.025 kHz, Stereo, 16-bit";
		formatNameTable[WAVE_FORMAT_2M08] = "22.05  kHz, Mono,   8-bit";
		formatNameTable[WAVE_FORMAT_2S08] = "22.05  kHz, Stereo, 8-bit";
		formatNameTable[WAVE_FORMAT_2M16] = " 22.05  kHz, Mono,   16-bit";
		formatNameTable[WAVE_FORMAT_2S16] = " 22.05  kHz, Stereo, 16-bit";
		formatNameTable[WAVE_FORMAT_4M08] = " 44.1   kHz, Mono,   8-bit";
		formatNameTable[WAVE_FORMAT_4S08] = " 44.1   kHz, Stereo, 8-bit";
		formatNameTable[WAVE_FORMAT_4M16] = " 44.1   kHz, Mono,   16-bit";
		formatNameTable[WAVE_FORMAT_4S16] = " 44.1   kHz, Stereo, 16-bit";

		formatNameTable[WAVE_FORMAT_44M08] = " 44.1   kHz, Mono,   8-bit";
		formatNameTable[WAVE_FORMAT_44S08] = " 44.1   kHz, Stereo, 8-bit";
		formatNameTable[WAVE_FORMAT_44M16] = " 44.1   kHz, Mono,   16-bit";
		formatNameTable[WAVE_FORMAT_44S16] = " 44.1   kHz, Stereo, 16-bit";
		formatNameTable[WAVE_FORMAT_48M08] = " 48     kHz, Mono,   8-bit";
		formatNameTable[WAVE_FORMAT_48S08] = " 48     kHz, Stereo, 8-bit";
		formatNameTable[WAVE_FORMAT_48M16] = " 48     kHz, Mono,   16-bit";
		formatNameTable[WAVE_FORMAT_48S16] = " 48     kHz, Stereo, 16-bit";
		formatNameTable[WAVE_FORMAT_96M08] = " 96     kHz, Mono,   8-bit";
		formatNameTable[WAVE_FORMAT_96S08] = " 96     kHz, Stereo, 8-bit";
		formatNameTable[WAVE_FORMAT_96M16] = " 96     kHz, Mono,   16-bit";
		formatNameTable[WAVE_FORMAT_96S16] = " 96     kHz, Stereo, 16-bit";
	}

	for (int i = 0; i < 32;++i)
	{
		DWORD curFormat = (1 << i);
		if ((dwFormats&curFormat) != 0)
		{
			if (!formatNameTable[curFormat].empty())
			{
				cout << i + 1 << "\t" << formatNameTable[curFormat] << endl;
			}
			else
			{
				cout << "Index " << i << " is not support by the API!" << endl;
			}
		}
	}
	
}


// 音频输出的参数.
void ReportAudioEnv()
{
	UINT aoDevNum = waveOutGetNumDevs();
	cout << "Audio output dev number: " << aoDevNum << endl;

	for (UINT i = 0; i < aoDevNum; ++i)
	{
		WAVEOUTCAPS woCaps;
		MMRESULT ret = waveOutGetDevCaps(i, &woCaps, sizeof(woCaps));
		if ( MMSYSERR_NOERROR != ret )
		{
			cout << "Get device caps fail! ret: " << ret << endl;
			continue;
		}

		cout << "Audio output dev " << i << " :" << endl;
		cout << "Manufacturer ID: " << woCaps.wMid << endl;
		cout << "Product ID: " << woCaps.wPid << endl;
		cout << "Version of the driver: " << woCaps.vDriverVersion << endl;
		wcout << L"Product name: " << woCaps.szPname << endl;
		//printf
		cout << "formats supported: " << woCaps.dwFormats << endl;

		PrintFormats(woCaps.dwFormats);
		cout << endl;
		
		cout << "number of sources supported: " << woCaps.wChannels << endl;
		cout << "packing: " << woCaps.wReserved1 << endl;
		cout << "fanctionality supported by driver: " << woCaps.dwSupport << endl;
		cout << "----------------------------------------" << endl << endl;
	}

	cout << endl;
	UINT aiDevNum = waveInGetNumDevs();
	cout << "Audio input dev number: " << aiDevNum << endl;

	for (UINT i = 0; i < aiDevNum; ++i)
	{
		WAVEINCAPS wiCaps;
		MMRESULT ret = waveInGetDevCaps(i, &wiCaps, sizeof(wiCaps));
		if (MMSYSERR_NOERROR != ret)
		{
			cout << "Get audio input device caps fail! ret: " << ret << endl;
			continue;
		}
		cout << "manufacturer ID: " << wiCaps.wMid << endl;
		cout << "product ID: " << wiCaps.wPid << endl;
		cout << "version of the driver: " << wiCaps.vDriverVersion << endl;
		wcout << L"product name: " << wiCaps.szPname << endl;
		cout << "formats supported: " << wiCaps.dwFormats << endl;

		PrintFormats(wiCaps.dwFormats);
		cout << endl;

		cout << "number of channels supported: " << wiCaps.wChannels << endl;
		cout << "structure packing: " << wiCaps.wReserved1 << endl;
		
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	// 设置字符集,输出中文.
	setlocale(LC_CTYPE, "");

	// 是否支持图像采集.
	bool isSupportVideoInput = HasVideoInput();
	cout << "Is support Video Input : " << ( isSupportVideoInput ? "True" : "False") << endl;

	// 枚举安装的摄像头驱动.
	EnumInstalledCaptureDrivers();

	// 音频
	ReportAudioEnv();

	system("pause");
	return 0;
}

