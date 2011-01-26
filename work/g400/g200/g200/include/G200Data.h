#ifndef _G200_DATA_H
#define _G200_DATA_H

#include "Common.h"
#include "kdmlist.h"
#include "readlicense.h"
#include "usbkeyconst.h"
#include "cnetmanager.h"
struct TPuiData
{
protected:
    TBasePui  m_tBasePui;                       //设备
    u16       m_wInstId;                        //实例号
	u8		  m_byIpNum;						//IP数量
	TIpAddr   m_atDevIp[IP_NUM];				//IP列表
    u8        m_byDevStat;                      //设备状态
	                                            //  0:连接1:使用2:退网3:同步
	
    s32		  m_nRegTime;                       //注册时间
	u32       m_dwMaxPuNum;					    //前端数限制
	u32       m_dwCurPuNum;					    //在线前端数
	u32		  m_dwRegionID;                     //区域号
public:
    TPuiData(void);
	
    void PrintData(u8 byOffsetSize = 0) const;
	
    void SetConnected(BOOL32 bConnected);//设置Pui连接状态
    void SetInstId(u16 wInstId);         //设置Pui实例号
    void SetBasePui(const TBasePui& tBasePui);//设置BasePui
    const TBasePui& GetBasePui(void) const;   //获取BasePui
    TBasePui& GetBasePui(void);               //获取BasePui
    BOOL32 IsConnected(void)const;            //获取Pui连接状态
    u16 GetInstId(void) const;                //获取Pui实例号
    void SetUsed(BOOL32 bUsed);               //设置是否启用
    BOOL32 IsUsed(void)const;                 //获取是否启用
    void SetDel(BOOL32 bDel);                 //设置是否退网
    BOOL32 IsDel(void)const;                  //获取是否退网
	void SetSure(BOOL32 bSure);               //设置是否同步
	BOOL32 IsSure(void)const;                 //获取是否同步
	
    void SetRegTime();
    s32 GetRegTime() const;
	
	void SetCurPuNum(u32 dwNum);    //设置当前在线PU数目
	u32  GetCurPuNum() const;       //获取当前在线PU数目
	void AddCurPuNum();             //增加一个当前在线PU数目
	void DelCurPuNum();             //减少一个当前在线PU数目
	
	void SetMaxPuNum(u32 dwNum);    //设置最大在线PU数目
	u32  GetMaxPuNum() const;       //获取最大在线PU数目

	void SetIpNum(u8 byNum);
	u8 GetIpNum() const;
	void SetIpAddr(u8 byIndex, const TIpAddr& tIpAddr);//按索引设置ip地址
	const TIpAddr& GetIpAddr(u8 byIndex) const;
};
typedef CKdmListEx<TPuiData,1> TPuiDataList;

struct TVideoSrcInfo
{
protected:
	u8 m_byCamType;         //摄像头类型;
    u16 m_wCamId;           //摄像头地址码;
    u8 m_byTransType;       //码流传输方式;
    u8 m_byCamCtrlWay;      //视频输入源摄象头控制方式（0:正向，1:反向）
    u8 m_byCamParseWay;     //视频输入源摄象头协议解析方式;
    u8 m_byCamAutoRun;      //摄象头自动巡航功能;
	s8 szVideoSrcAlias[MAXLEN_ALIAS+1];
public:
	TVideoSrcInfo();

	u8 GetCamType();
	void SetCamType(const u8 byCamType);
	
	u16 GetCamId();
	void SetCamId(const u16 wCamId);
	
	u8 GetTransType();
	void SetTransType(const u8 byTransType);
	
	u8 GetCamCtrlWay();
	void SetCamCtrlWay(const u8 byCamCtrlWay);
	
	u8 GetCamParseWay();
	void SetCamParseWay(const u8 byCamParseWay);
	
	u8 GetCamAutoRun();
	void SetCamAutoRun(const u8 byCamAutoRun);

	LPCSTR GetVideoSrcAlias() const;
    void SetVideoSrcAlias(LPCSTR lpszSrcVideoChnAlias);
};

struct TMatrixData
{
public:
	TMatrixData();
	
public:
	u32 GetMatrixID();
	void SetMatrixID(const u32 dwMatrixID);
	
	u8 GetMatrixType();
	void SetMatrixType(const u8 byMatrixType);
	
    void SetMatrixName(const s8* pszMatrixName);
    const s8* GetMatrixName();

    void SetMatrixInportNum(s32 nPortNum);
    s32 GetMatrixInportNum();

    void SetMatrixOutportNum(s32 nPortNum);
    s32 GetMatrixOutportNum();

	BOOL32 IsNeedNtyCamStatus();
	void SetNeedNtyCamStatus(BOOL32 bNeed);
	
	u16 GetOnlineEncNum();
	void SetOnlineEncNum(const u16 wNum);
	void AddOnlineEncNum();
	void SubOnlineEncNum();
	
protected:
    s8 m_achMatrixName[MAXLEN_NAME + 1];
    s32 m_nMatrixInportNum;
    s32 m_nMatrixOutPortNum;
	u32 m_dwMatrixID;
	u8 m_byMatrixType;
	u16 m_wOnlineEncNum;
	BOOL32 m_bNeedNtyCamStatus;
};

typedef CKdmListEx<TMatrixData, 1> TMatrixDataList;

struct TCameraData
{
protected:
    TPuStatus  m_tPuStatus;                 //PU状态
    TVideoSrcInfo m_tVideoSrcInfo;          //视频源信息
    u8  m_byDevStat;                        //设备状态
                					        //1:退网
                                            //2:同步
    s8 m_szDevName[MAXLEN_NAME+1];         //设备名称;
public:
    TCameraData(void);
    void PrintData(u8 byOffsetSize = 0);

    void SetPuStatus(const TPuStatus& tPuStatus); //设置PU状态
    void SetPuOnline(BOOL32 bIsOnline);

    TPuStatus* GetPuStatusPtr();
    TPuStatus& GetPuStatus(void); //获取PU状态
    
	TVideoSrcInfo& GetVideoSrcInfo();
	void SetVideoSrcInfo(const TVideoSrcInfo& tVideoSrcInfo);

    BOOL32 IsUsed() const;          //是否启用
    void SetUsed(BOOL32 bIsUsed);   //设置是否启用

    void SetDel(BOOL32 bDel);       //设置是否退网
    BOOL32 IsDel(void)const;        //获取是否退网

    BOOL32 IsStop(s32 dwTime) const;  //是否停用

    BOOL32 IsOnLine(void) const;      //是否在线

	void SetSure(BOOL32 bSure); //设置是否同步
	BOOL32 IsSure(void) const;   //获取是否同步
    LPCSTR GetDevName();
    void SetDevName(LPCSTR lpszName);
};

struct TEncData
{
protected:
    TPuStatus       m_tPuStatus;                        //PU状态
    u16             m_wCamNum;                          //监控点数
    TBasePuVideoChnInfo m_tBasePuVideoChnInfo;          //视频源参数
    u32             m_dwIpAddr;                         //IP地址(网路序)
    u16             m_wStartPort;                       //源码流起始端口
    u16             m_wInstId;                          //实例号
    u8              m_byDevStat;                        //设备状态
                                                            //1:取完Cap
                                                            //2:取完状态
                					                        //3:退网
                                                            //4:同步

    u8              m_bySwitchType;                     //转发类型
    TTransportAddr  m_tTransportAddr;                   //组播地址
	s8              m_szManuFac[MAXLEN_DEVICE_MANUFACTURER+1]; //设备制造商

    u32             m_dwPuiAddr;                        //对应的平台地址(Pui的)(网络序)
    s8 m_szDevName[MAXLEN_NAME+1];         //设备名称;
public:
    TEncData(void);
    void PrintData(u8 byOffsetSize = 0);
    void SetPuStatus(const TPuStatus& tPuStatus); //设置PU状态
    void SetPuOnline(const BOOL32 bIsOnline);

    TPuStatus* GetPuStatusPtr();
    TPuStatus& GetPuStatus(void); //获取PU状态
    void SetBasePuVideoChnInfo(const TBasePuVideoChnInfo& tBasePuVideoChnInfo);//设置视频源参数
    const TBasePuVideoChnInfo& GetBasePuVideoChnInfo(void) const; //获取视频源参数
	void ParsePuVideoChnInfo();//刷新PuStatus各编码通道的视频源参数
    void SetBasePu(const TBasePu& tBasePu);
    BOOL32 IsUsed() const;          //是否启用
    void SetUsed(const BOOL32 bIsUsed);   //设置是否启用

    u32 GetIpAddr() const;          //获取IP地址，网络序
    void SetIpAddr(const u32 dwIpAddr);   //设置IP地址，网络序

    u16 GetInstId(void) const;          //获取Pui实例号
    void SetInstId(const u16 wInstId);  //设置Pui实例号

    void SetDel(const BOOL32 bDel);       //设置是否退网
    BOOL32 IsDel(void)const;        //获取是否退网
    void    SetSwitchType(u8 bySwitchType); //设置转发类型
    u8      GetSwitchType(void)const;       //获取转发类型
    void    SetTransportAddr(const TTransportAddr& tTransportAddr);//设置组播地址
    const TTransportAddr&  GetTransportAddr(void)const;//获取组播地址

    void SetStopTime(const TKDMTime& tKdmTime); //设置停用时间
    TKDMTime GetStopTime() const;               //获取停用时间
    BOOL32 IsStop(const s32 dwTime) const;            //是否停用
    u16 GetStartPort()const;//获取源码流起始端口
    void SetStartPort(u16 wStartPort);//设置源码流起始端口
    u8 GetDirectTransNum()const;//获取直传路数
    void SetDirectTransNum(const u8 byDirectTransNum);//设置直传路数

    u8 GetNowDirectTransCnt(const u8 byEncChnId)const;//获取当前直传路数
    void SetNowDirectTransCnt(const u8 byEncChnId, const u8 byNowDirectTransCnt);//设置当前直传路数
    BOOL32 IsNeedAutoLook(void)const;//是否需要自动选看
    void SetNeedAutoLook(const BOOL32 bNeedAutoLook);//设置自动选看
    BOOL32 IsOnLine(void) const;              //是否在线

    BOOL32 IsHasGetCap() const;
    void HasGetCap(const BOOL32 bIsHasGetCap);

    BOOL32 IsHasGetStatus() const;
    void HasGetStatus(const BOOL32 bIsHasGetStatus);

	void SetSure(const BOOL32 bSure); //设置是否同步
	BOOL32 IsSure(void)const; //获取是否同步

    BOOL32 IsCanUse() const;              //平台是否获取所有信息

    void SetCamNum(const u16 wCamNum);    //设置监控点数
    u16 GetCamNum() const;          //获取监控点数

 	void SetManuFac(LPCSTR szManuFac);  //设置厂商名称
	LPCSTR GetManuFac() const;          //获取厂商名称

    u32 GetPuiIpAddr() const;          //获取Pu登陆的Pui的IP地址，网络序
    void SetPuiIpAddr(const u32 dwIpAddr);   //设置Pu登陆的Pui的IP地址，网络序

    const s8* GetDevName();
    void SetDevName(const s8* lpszName);
};


//矩阵视频输入端口与摄像头的绑定关系
struct TVideoInRelation
{
public:
	TVideoInRelation();
	
	u32 GetMatrixID();
	void SetMatrixID(const u32 dwMatrixID);

	u8 GetMatrixType();
	void SetMatrixType(const u8 byMatrixType);
	
	u16 GetVideoInPort();
	void SetVideoInPort(const u16 wVideoInPort);
	
	TCameraData& GetCameraData();
	void SetCameraData(const TCameraData& tCamData);

    void SetRegisterStatus(BOOL32 bRegiste);
	BOOL32 GetRegisterStatus();

    void SetCamAddr(s32 nCamAddr);
    s32 GetCamAddr();

    void SetCamType(s32 nType);
    s32 GetCamType();

protected:
	u32 m_dwMatrixID;
	u8  m_byMatrixType;      //矩阵类型
	u16 m_wVideoInPort;      //矩阵输入端口号

    BOOL32 m_bRegiste;
    s32 m_nCamAddr;
    s32 m_nCamType;
	TCameraData m_tCamData;       //与矩阵输入端口连接的摄像头信息
};
typedef CKdmListEx<TVideoInRelation, 1> TVideoInRTList;

struct TEncVideoInputPortInfo
{
public:
	TEncVideoInputPortInfo();
	
	BOOL32 IsConnVideoOutPort();
	void SetConnVideoOutPort(BOOL32 bConn);

	BOOL32 IsUsed();
	void SetUsed(BOOL32 bUsed);

	u16 GetCurMatrixInputPort();
	void SetCurMatrixInputPort(const u16 wCurInputPort);

	u16 GetVideoOutPort();
	void SetVideoOutPort(const u16 wPort);

protected:
	BOOL32 m_bIsConnVideoOutPort;  //该编码器的视频源输入端口是否连接了矩阵的视频输出端口
	BOOL32 m_bIsUsed;              //该编码器的视频源输入端口是否正在使用
	u16 m_wCurMatrixInPutPort;     //当前正在服务的矩阵视频输入源端口
	u16 m_wVideoOutPort;           //该编码器的视频源输入端口连接的矩阵视频输出端口号
};

//矩阵视频输出端口与编码器的对应关系
struct TVideoOutRelation
{
public:
	TVideoOutRelation();

	u32 GetMatrixID();
	void SetMatrixID(const u32 dwMatrixID);

	BOOL32 IsConnMatrix();
	void SetConnMatrix(BOOL32 bConnMatrix);
	
	TEncVideoInputPortInfo& GetEncVideoInputPortInfo(const u8 byVidChnId);
	void SetEncVideoInputPortInfo(const u8 byVidChnId, const TEncVideoInputPortInfo& tInfo);
	
	TEncData& GetEncData();
	void SetEncData(const TEncData& tEncData);

	//获取编码器可用的编码通道、视频输入通道及对应的矩阵输出端口号
	BOOL32 IsCanUse(OUT u8 *pbyEncChnId, OUT u8 *pbyVidChnId, OUT u16 *pwValidOutPort);

	//恢复编码器的编码通道及视频输入通道的使用状态
	void ResetEncStatus(const u8 byEncChnId, const u8 byVidChnId);
	
    void SetVideoSrcPortNum(s32 nPortNum);
    s32 GetVideoSrcPortNum();

protected:
	u32    m_dwMatrixID;
    s32 m_nVideoSrcPortNum;
	BOOL32 m_bIsConnMatrix;   //是否连接了矩阵
	TEncData m_tEncDevData;   //编码器信息
	TEncVideoInputPortInfo m_atEncVideoInputPortInfo[MAX_PU_CHNNUM];      //编码器各视频源输入端口信息
};

typedef CKdmListEx<TVideoOutRelation, 1> TVideoOutRTList;

struct TSwitchData
{
public:
	TSwitchData();

public:
	void SetMatrixID(const u32 dwMatrixID);
	u32 GetMatrixID();

	void SetEncNO(const TKDMNO& tKdmNo);
	TKDMNO& GetEncNO();

	void SetCamNO(const TKDMNO& tKdmNo);
	TKDMNO& GetCamNO();

	void SetEncChnId(const u8& byEncChnId);
	u8 GetEncChnId();

	void SetVidChnId(const u8 byVidChnId);
	u8 GetVidChnId();

	void SetVideoOutPort(const u16 wOutPort);
	u16 GetVideoOutPort();

	void SetVideoInPort(const u16 wInPort);
	u16 GetVideoInPort();

	void SetVideoIp(u32 ip);
	u32  GetVideoIp();

	void SetVideoIpPort(u16 port);
	u16  GetVideoIpPort();
	
	void SetAudioIp(u32 dwIp);
	u32  GetAudioIp();

	void SetAudioIpPort(u16 wIpPort);
	u16  GetAudioIpPort();
protected:
	u32 m_dwMatrixID;    //矩阵编号
	TKDMNO m_tEncNO;     //编码器编号
	TKDMNO m_tCamNO;     //摄像头编号
	u8     m_byEncChnId; //编码通道号
	u8     m_byVidChnId; //编码器视频输入通道号（从0开始）
	u16    m_wVideoOutPort; //矩阵输出端口号（从1开始）
	u16    m_wVideoInPort;  //矩阵输入端口号（从1开始）

	u32    m_dwIp;          //发送码流的ip地址
	u16    m_wPort;        //发送码流的端口

	u32    m_dwAudioIp;     //发送音频的ip地址
	u16    m_wAudioPort;    //发送音频的端口
};
typedef CKdmListEx<TSwitchData, 1> TSwitchDataList;

class CLicenseHandler
{
public:
	enum{TYPE_NONE,TYPE_FILE,TYPE_USB};
	CLicenseHandler(const s8* pszDevice = "eth0"):m_pszDeviceName(pszDevice)
	{
		m_bValid = FALSE;
		m_nMaxChannel = 0;
		m_nMaxEncoder = 0;
		m_abyMacAddr[0] = 0;
		m_achApproval[0] = 0;
		m_achMaxDate[0] = 0;
		m_byType = TYPE_NONE;
	}
	void InitLicense(const s8* pszFilePath)
	{	
#ifdef _LINUX_
		s8 achHostFile[MAXLEN_FILENAME + 1];
		s8 achLicenseKeyFile[MAXLEN_FILENAME + 1];
		sprintf(achHostFile,"%s%s",pszFilePath,"host.txt");
		sprintf(achLicenseKeyFile,"%s%s",pszFilePath,"kedalicense.key");
		s32 nLicenceLen = 64;
		s8 achTmp[64] = {0};
		
		do 
		{
			if (!m_cNetManager.GetMacAddr(m_pszDeviceName,m_abyMacAddr,sizeof(m_abyMacAddr)))
			{
				m_bValid = FALSE;
				break;
			}
			
			FILE* pFile = fopen(achHostFile,"w");
			fwrite(m_abyMacAddr,sizeof(u8),strlen(m_abyMacAddr),pFile);
			fclose(pFile);
			
			u16 wRet = 0;		
			wRet = ReadLicenseFile(achLicenseKeyFile);
			if (0 != wRet)
			{
				OspPrintf(TRUE , FALSE , "Fail ReadLicenseFile\n");
				break;
			}
			
			//获取文件中的机器码
			wRet = GetValue("KEY_LICENSE_MACHINENO", achTmp, nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , "Fail get KEY_LICENSE_MACHINENO\n");
				break;
			}
			printf("machineno:%s\n", achTmp);
			if (memcmp(achTmp, m_abyMacAddr, 64) != 0)
			{
				OspPrintf(TRUE, FALSE , "Fail cmp MACHINENO\n");
				break;
			}
			//license有效
			m_bValid = TRUE;
			m_byType = TYPE_FILE;

			nLicenceLen = 64;
			wRet = GetValue("KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM", achTmp, nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from file:KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM not found!\n");
				break;
			}
			m_nMaxChannel = atoi(achTmp);
			
			nLicenceLen = 64;
			wRet = GetValue("KEY_LICENSE_MAX_OTHER_ENCODER_NUM",achTmp,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from file:KEY_LICENSE_MAX_OTHER_ENCODER_NUM not found!\n");
				break;
			}
			m_nMaxEncoder = atoi(achTmp);
			
			nLicenceLen = sizeof(m_achMaxDate);
			wRet = GetValue("KEY_LICENSE_MAX_DATE",m_achMaxDate,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from file:KEY_LICENSE_MAX_DATE not found!\n");
				break;
			}

			nLicenceLen = sizeof(m_achApproval);
			wRet = GetValue("KEY_LICENSE_KEY",m_achApproval,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from file:KEY_LICENSE_KEY not found!\n");
				break;
			}
			
		} while (TRUE);
		
		//读取文件不成功,则读取usb中的license
		if (!m_bValid)
		{
			u16 wRet = ReadLicenseFile("USBKEY");//返回0为成功
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey license\n");
				return;		
			}
			m_bValid = TRUE;
			m_byType = TYPE_USB;

			nLicenceLen = 64;
			wRet = GetValue("KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM", achTmp, nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from usbkey:KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM not found!\n");
				return;
			}
			nLicenceLen = 64;
			wRet = GetValue("KEY_LICENSE_MAX_OTHER_ENCODER_NUM",achTmp,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from usbkey:KEY_LICENSE_MAX_OTHER_ENCODER_NUM not found!\n");
				return;
			}
			m_nMaxEncoder = atoi(achTmp);
			
			nLicenceLen = sizeof(m_achMaxDate);
			wRet = GetValue("KEY_LICENSE_MAX_DATE",m_achMaxDate,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from usbkey:KEY_LICENSE_MAX_DATE not found!\n");
				return;
			}

			nLicenceLen = sizeof(m_achApproval);
			wRet = GetValue("KEY_LICENSE_KEY",m_achApproval,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from usbkey:KEY_LICENSE_KEY not found!\n");
				return;
			}
		}
#else
		m_bValid = TRUE;
		m_nMaxChannel = 32;
		m_nMaxEncoder = 32;
#endif
	}	

	s32 GetMaxChannel() const
	{
		return m_nMaxChannel;
	}

	const s8* GetMacInfo() const
	{
		return m_abyMacAddr;
	}

	const s8* GetApprovalNo() const
	{
		return m_achApproval;
	}

	const s8* GetMaxDate() const
	{
		return m_achMaxDate;
	}

	BOOL32 IsLicenseValid()
	{
		return m_bValid;
	}

	u8 GetLicenseType()
	{
		return m_byType;
	}

	s32 GetMaxEncoderNum()
	{
		return m_nMaxEncoder;
	}
private:
	CNetManager m_cNetManager;//读取mac地址
	BOOL32 m_bValid;//license是否有效
	s32 m_nMaxChannel;//license支持的最大通道数
	s32 m_nMaxEncoder;//允许注册的最大编码器数
	s8 m_abyMacAddr[128 + 1];//对应网卡的MAC,加密之后
	s8 m_achApproval[64 + 1];//许可证编号
	s8 m_achMaxDate[128 + 1];//许可证有效期
	u8 m_byType;//license类型
	const s8* m_pszDeviceName;
};

class CVcData
{
public:
	CVcData();
	~CVcData();

	TMatrixDataList* GetMatrixDataListPtr();
	void AddMatrixData(const TMatrixData& tMatrixData);
	void DelMatrixData(const u32 dwMatrixID);
	TMatrixData* GetMatrixData(const u32 dwMatrixID);

	TVideoInRTList *GetVideoInRTListPtr();
	void AddVideoInRT(const TVideoInRelation& tVideoInRt);
	void DelVideoInRT(const TVideoInRelation& tVideoInRt);
	TVideoInRelation* GetVideoInRT(const u32 dwMatrixID);
	TVideoInRelation* GetVideoInRT(const TKDMNO& tVidiconID);

	TVideoOutRTList *GetVideoOutRTListPtr(const u32 dwMatrixID);
	void AddVideoOutRT(const u32 dwMatrixID, const TVideoOutRelation& tVideoOutRT);
	void DelVideoOutRT(const u32 dwMatrixID, const TVideoOutRelation& tVideoOutRT);
	TVideoOutRelation* GetVideoOutRt(const TKDMNO& tKdmNo);
	TVideoOutRelation* GetVideoOutRt(s32 nMatrix,s32 nIndex);
	TPuiDataList *GetPuiDataListPtr();
	TPuiData* AddPuiDev(const TBasePui& tBasePui);
	void DelPuiDev(const TPuiID& tPuiID);
	TPuiData* GetPuiData(const TPuiID& tPuiId);
	TPuiData* GetPuiData(const u16 wInstId);

	TSwitchDataList* GetSwitchDataListPtr();
	void AddSwitchData(const TSwitchData& tSwitchData);
	void DelSwitchDataByCamNo(const TKDMNO& tSrcNo,u32 dwIp = 0,u32 dwPort = 0);
	TSwitchData* GetSwitchData(const TKDMNO& tSrcNo,u32 dwIp = 0,u32 dwPort = 0);

	void AddOnlineEncNum();
	void SetOnlineEncNum(u32 dwOnlineEncNum);
	void DelOnlineEncNum();
	u32 GetOnlineEncNum();

	BOOL32 ChooseEnableEnc(const u32 dwMatrixID, const u16 wInPort, OUT TVideoOutRelation& tMatrixCtrlEnc, OUT TVideoOutRelation& tRcvStreamEnc, OUT u8& byEncChnId, OUT u8& byVidChnId, OUT u16& wOutPort);
	
	void SetConnMplat(BOOL32 status);
	BOOL32 GetConnMplat();
private:
	u32 GetMatrixID(const TKDMNO& tKdmNo);
    s32 GetEncdrListIndex(u32 dwMatrixIndex);
    s32 GetEncdrContainerindex(u32 dwMatrixIndex);
	
public:
	TG200CfgBase m_tG200Cfg;
	CLicenseHandler m_cLicenseHandler;
public:
	SEMHANDLE       m_hMainSem;
protected:
	TMatrixDataList m_tMatrixDataList;  //矩阵信息列表
	TVideoInRTList  m_tVideoInRTList;   //视频输入绑定关系列表
	TVideoOutRTList m_atVideoOutRTList[G200_MAX_MATRIX_NUM];   //每个矩阵编号对应一个视频输出队列
	TPuiDataList    m_tPuiDataList;     //pui设备列表
	TSwitchDataList m_tSwitchDataList;  //交换数据列表

	u32 m_dwOnlineEncNum;  //在线编码器数目
	BOOL32 m_bConnMPlat;   //与上级平台的连接状态
};

extern CVcData g_cVcData;

#endif