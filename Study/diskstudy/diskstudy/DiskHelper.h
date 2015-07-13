#pragma once

#include <vector>
#include <string>

typedef _int64 i64;

class CDiskHelper
{
public:
	CDiskHelper();
	~CDiskHelper();



};

// 获取系统中的磁盘列表.
typedef std::wstring CLogicalVol;
typedef std::vector< CLogicalVol > CLogicalVolList;
bool GetLogicalVolList(CLogicalVolList& diskList);

typedef std::wstring CPhysicalDrive;
typedef std::vector< CPhysicalDrive > CPhysicalDriveList;
bool GetPhysicalDriveList(CPhysicalDriveList& physicalDriveList);

// 通过逻辑磁盘名获取物理磁盘号.
// 成功返回物理驱动器号, 失败返回-1.
typedef std::wstring CLogicalVolName;
int GetPhysicalDriveNumberByLogicalVolName(const CLogicalVolName& name);

CPhysicalDrive GetPhysicalDriveName(int physicalDriveNumber);

enum EDiskMediaType
{
	DiskMediaTypeInvalid,
	DiskMediaTypeFixHardDisk,
	DiskMediaTypeRemovableMedia,
	DiskMediaTypeUnknown,
};
// 获取磁盘信息.
struct TDiskInfo
{
	// 磁盘总大小(字节)
	i64 m_diskSize;

	// Cylinders数目
	i64 m_cylinders;

	// Media type. FixedMedia : 固定硬盘. RemovableMedia:可移除的媒体
	EDiskMediaType m_mediaType;
	int m_mediaTypeRaw;

	// 
	unsigned int m_sectorsPerTrack;

	//
	unsigned int m_bytesPerSector;

	// SMART版本信息. https://msdn.microsoft.com/en-us/library/windows/hardware/ff554977(v=vs.85).aspx
	//BYTE     bVersion;               // Binary driver version.
	//BYTE     bRevision;              // Binary driver revision.
	//BYTE     bReserved;              // Not used.
	//BYTE     bIDEDeviceMap;          // Bit map of IDE devices.
	//DWORD   fCapabilities;          // Bit mask of driver capabilities.
	unsigned char bIDEDeviceMap;	// IDE信息.
	unsigned int fCapabilities;		// 驱动能力集合Bit mask.

	// 温度.
	float m_diskTemperature;
};

bool GetDiskInfo(const CPhysicalDrive& drive, TDiskInfo& diskInfo);