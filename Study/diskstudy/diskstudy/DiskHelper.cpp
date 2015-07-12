#include "stdafx.h"
#include "DiskHelper.h"

#include <sstream>
#include <winioctl.h>

CDiskHelper::CDiskHelper()
{
}


CDiskHelper::~CDiskHelper()
{
}

bool GetLogicalVolList(CLogicalVolList& volList)
{
	DWORD logicalDrivesMask = ::GetLogicalDrives();

	for (int i = 0; i < 32; ++i)
	{
		if ((logicalDrivesMask >> i) & 1)
		{
			char drive = 'A'+i;
			std::wstring vol;
			vol.push_back(drive);
			vol += L":";
			volList.push_back(vol);
		}
	}
	return true;
}

bool GetPhysicalDriveList(CPhysicalDriveList& driveList)
{
	CLogicalVolList volList;
	bool bOk = GetLogicalVolList(volList);
	if (!bOk)
	{
		return bOk;
	}

	for (size_t i = 0; i < volList.size(); ++i)
	{
		std::wstring volName = volList[i];
		
		int physicalDriveNum = GetPhysicalDriveNumberByLogicalVolName(volName);
		if (physicalDriveNum > 0)
		{
			CPhysicalDrive driveName = GetPhysicalDriveName(physicalDriveNum);
			driveList.push_back(driveName);
		}
	}

	return true;
}

int GetPhysicalDriveNumberByLogicalVolName(const CLogicalVolName& volName)
{
	if (volName.empty())
	{
		assert(false && "Disk volume name is empty!");
		return -1;
	}

	// 完整逻辑分区名.
	wchar_t volLetter = volName[0];
	std::wstringstream ssFullVolName;
	ssFullVolName << L"\\\\.\\" << volLetter << ':';

	// 打开逻辑分区.
	HANDLE hVolDevice = CreateFile(ssFullVolName.str().c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
		);
	if (hVolDevice == INVALID_HANDLE_VALUE)
	{
		int er = ::GetLastError();
		assert(false && "Create file fail!");
		return -1;
	}

	// 获取它对应的物理分区.
	STORAGE_DEVICE_NUMBER devNumber;
	DWORD dataLen = 0;
	BOOL bOk = ::DeviceIoControl(hVolDevice,
		IOCTL_STORAGE_GET_DEVICE_NUMBER,
		NULL,
		0,
		&devNumber,
		sizeof(devNumber),
		&dataLen,
		NULL
		);
	
	CloseHandle(hVolDevice);

	if (!bOk)
	{
		int er = GetLastError();
		assert(false && "Device IO Control fail!");
	
		return -1;
	}

	// 暂时只处理硬盘, 将其它设备类型过滤.
	if (FILE_DEVICE_DISK != devNumber.DeviceType)
	{
		return -1;
	}

	int physicalNumber = devNumber.DeviceNumber;
	
	return physicalNumber;
}

CPhysicalDrive GetPhysicalDriveName(int physicalDriveNum)
{
	std::wstringstream ss;
	ss << L"\\\\.\\PhysicalDrive" << physicalDriveNum;
	return ss.str();
}

bool GetDiskInfo(const CPhysicalDrive& drive, TDiskInfo& diskInfo)
{
	// 打开设备.
	HANDLE hDevice = CreateFile(drive.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		0
		);
	if (INVALID_HANDLE_VALUE == hDevice)
	{
		int er = ::GetLastError();
		assert(false && "Create file fail!");
		return false;
	}

	bool ret = true;

	// 读取磁盘基本信息.
	DISK_GEOMETRY_EX geometry;
	DWORD outLen = 0;
	BOOL bOk = DeviceIoControl(hDevice,
		IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
		NULL,
		0,
		&geometry,
		sizeof(geometry),
		&outLen,
		0
		);
	if (bOk)
	{
		diskInfo.m_diskSize = geometry.DiskSize.HighPart;
		diskInfo.m_diskSize <<= 32;
		diskInfo.m_diskSize += geometry.DiskSize.LowPart;

		diskInfo.m_cylinders = geometry.Geometry.Cylinders.HighPart;
		diskInfo.m_cylinders <<= 32;
		diskInfo.m_cylinders += geometry.Geometry.Cylinders.LowPart;

		diskInfo.m_sectorsPerTrack = geometry.Geometry.SectorsPerTrack;
		diskInfo.m_bytesPerSector = geometry.Geometry.BytesPerSector;
		
		switch (geometry.Geometry.MediaType)
		{
		case FixedMedia:
			diskInfo.m_mediaType = DiskMediaTypeFixHardDisk;
			break;
		case RemovableMedia:
			diskInfo.m_mediaType = DiskMediaTypeRemovableMedia;
			break;
		default:
			diskInfo.m_mediaType = DiskMediaTypeUnknown;
			assert(false && "未知的媒体类型!");
			break;
		}
		diskInfo.m_mediaTypeRaw = geometry.Geometry.MediaType;
	}
	else
	{
		int er = ::GetLastError();
		ret = false;
		assert(false && "Get device te");
	}

	// 读取SMART版本.
	GETVERSIONINPARAMS smartVersion;
	bOk = DeviceIoControl(hDevice,
		SMART_GET_VERSION,
		NULL,
		0,
		&smartVersion,
		sizeof(smartVersion),
		&outLen,
		NULL
		);
	if (bOk)
	{
		smartVersion;
	}
	else
	{
		int er = GetLastError();
		ret = false;
		assert(false && "Get SMART version fail!");
	}

	// 关闭句柄.
	CloseHandle(hDevice);
	hDevice = INVALID_HANDLE_VALUE;

	return ret;
}