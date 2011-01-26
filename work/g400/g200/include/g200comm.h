#ifndef _G200_COMM_H
#define _G200_COMM_H

#include "kdvtype.h"
#include "cmstypes.h"
#include "kdvsys.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib") 
#pragma pack(push)
#pragma pack(1)
#endif

#define CONFIG_DIRECTORY         "conf/"
#define CONFIG_FILE_NAME         "G200.ini"
#define CONFIG_FILE_DEVICES      "devices.ini"

#define MATRIX_CFG_FILE_NAME     "matrixcfg.ini"
#define IDLEDEVICE_FILE_NAME     "IdleDevices.ini"


#define SECTION_MPLAT           ( "MPLAT" )
#define KEY_MPLATIP             ( "MPLAT_IP" )               //上级CMU IP_NOT_SET
#define KEY_MPLATPORT           ( "MPLAT_PORT" )             //上级CMU侦听端口
#define KEY_3AIP                ( "3A_IP" )
#define KEY_3APORT              ( "3A_PORT" )
#define KEY_3AUSER              ( "3A_User" )
#define KEY_3APSW               ( "3A_Psw" )
#define KEY_MPLATTYPE           ( "MPLAT_TYPE" )

#define SECTION_SELFPLAT        ( "SPLAT" )
#define KEY_SELFPLAT_NO         ( "SPLAT_NO" )                //Cmu ID
#define KEY_SELFPLAT_IP         ( "SPLAT_IP" )                //Cmu IP
#define KEY_SELFPLAT_PORT       ( "SPLAT_PORT")               //Cmu 侦听端口

#define SECTION_REGSRV          ( "REGSRV" )
#define KEY_REGSRV_PORT         ( "REGSRV_PORT" )             //注册服务器监听端口

#define KEY_PUI_ENABLE          ( "IsRunPui" )              //是否运行Pui
#define KEY_MATRIXCFG_PATH      ( "MATRIXCFG_PATH" )

#define SECTION_DEVICENO        ( "DeviceNo" )
#define KEY_CURMAXMATRIXID      ( "CurMaxMatrixId" )
#define KEY_CURMATRIXNUM        ( "CurMatrixNum")

#define KEY_CURMAXCAMNO         ( "CurMaxCamNo" )
#define KEY_CURCAMNUM			( "CurCamNum")

#define KEY_CURMAXENCODERNO     ( "CurMaxEncoderNo" )
#define KEY_CURENCODERNUM       ( "CurEncoderNum" )

#define SECTION_MATRIXDEVICE    ( "MATRIXDEVICES" )
#define SECTION_CAMERADEVICE    ( "CAMERADEVICES" )
#define KEY_VIDEOINPORT         ( "VIDEOINPORT" )
#define KEY_TYPE                ( "TYPE")
#define KEY_CAMAddr             ( "CAMAddr")

#define SECTION_ENCODERDEVICE   ( "ENCODERDEVICES")
#define SECTION_PUI             ( "PUI Base" )
#define KEY_PUIID               ( "PUIID" )


#define SECTION_CLIENT          ("CLIENT")
#define KEY_USERNAME            ("UserName")
#define KEY_PSW                 ("Psw")

#define G200_MAX_MATRIX_NUM               64


//定义APP号
#define AID_G200MS_DEVMGR          300


//消息定义
//G200服务器与G200管理服务器之间的消息号
#define  EV_G200_G200MS_BGN       OSP_USEREVENT_BASE+1
#define  EV_G200_G200MS_END       EV_G200_G200MS_BGN+999
 
//G200管理服务器内部消息号
#define  EV_G200MS_MSG_BGN        EV_G200_G200MS_END+1
#define  EV_G200MS_MSG_END        EV_G200MS_MSG_BGN+999

//G200服务器内部消息号
#define  EV_G200_MSG_BGN        EV_G200MS_MSG_END+1
#define  EV_G200_MSG_END        EV_G200_MSG_BGN+999

#define  EV_G200_CLIENTCFG_BGN  EV_G200_MSG_END+1
#define  EV_G200_CLIENTCFG_END  EV_G200_CLIENTCFG_BGN+999

enum TCommMsg
{
	G200_G200MS_REG_REQ = EV_G200_G200MS_BGN,             //G200调度服务器向注册服务器注册请求，消息体TLicenseInfo
	G200MS_G200_REG_ACK,
	G200MS_G200_REG_NACK,

	G200MS_G200_ADD_BINDINRT_NTY,                         //设备注册成功，消息体TTotalNum + N * TBasePu

	EV_G200MS_CLIENTCONN_TIMER,                           //客户登录计时事件
};

enum TClientMsg
{
    BS_G200MS_USER_LOGIN_REQ = EV_G200_CLIENTCFG_BGN,//4025
    G200MS_BS_USER_LOGIN_ACK,
    G200MS_BS_USER_LOGIN_NACK,
    
    BS_G200MS_ADD_MATRIX_REQ,
    G200MS_BS_ADD_MATRIX_ACK,
    G200MS_BS_ADD_MATRIX_NACK,

    BS_G200MS_DELETE_MATRIX_REQ,
    G200MS_BS_DELETE_MATRIX_ACK,
    G200MS_BS_DELETE_MATRIX_NACK,

    BS_G200MS_UPDATE_MATRIX_REQ,
    G200MS_BS_UPDATE_MATRIX_ACK,
    G200MS_BS_UPDATE_MATRIX_NACK,

    BS_G200MS_ADD_ENCODER_REQ,
    G200_BS_ADD_ENCODER_ACK,
    G200_BS_ADD_ENCODER_NACK,

    BS_G200MS_DELETE_ENCODER_REQ,
    G200MS_BS_DELETE_ENCODER_ACK,
    G200MS_BS_DELETE_ENCODER_NACK,
    
    BS_G200MS_UPDATE_ENCODER_REQ,
    G200MS_BS_UPDATE_ENCODER_ACK,
    G200MS_BS_UPDATE_ENCODER_NACK,

    BS_G200MS_ADD_CAMERA_REQ,
    G200_BS_ADD_CAMERA_ACK,
    G200_BS_ADD_CAMERA_NACK,
    
    BS_G200MS_DELETE_CAMERA_REQ,
    G200MS_BS_DELETE_CAMERA_ACK,
    G200MS_BS_DELETE_CAMERA_NACK,

    BS_G200MS_UPDATE_CAMERA_REQ,
    G200MS_BS_UPDATE_CAMERA_ACK,
    G200MS_BS_UPDATE_CAMERA_NACK,

    BS_G200MS_ADDLINK_REQ,
    G200MS_BS_ADDLINK_ACK,
    G200MS_BS_ADDLINK_NACK,

    BS_G200MS_DELLINK_REQ,
    G200MS_BS_DELLINK_ACK,
    G200MS_BS_DELLINK_NACK,

    BS_G200MS_MATRIXSINFO_REQ,
    G200MS_BS_MATRIXSINFO_ACK,
    G200MS_BS_MATRIXSINFO_NACK,

    BS_G200MS_DEVICESLIST_REQ,
    G200MS_BS_DEVICESLIST_ACK,
    G200MS_BS_DEVICESLIST_NACK,

    BS_G200MS_UPDATE_USER_REQ,
    G200MS_BS_UPDATE_USER_ACK,
    G200MS_BS_UPDATE_USER_NACK,

    BS_G200MS_SYSCONF_REQ,
    G200MS_BS_SYSCONF_ACK,
    G200MS_BS_SYSCONF_NACK,

    BS_G200MS_SYSTEMREBOOT_REQ,
    G200MS_BS_SYSTEMREBOOT_ACK,
    G200MS_BS_SYSTEMREBOOT_NACK,

    BS_G200MS_HEARTBEAT_NTF,
    G200MS_BS_HEARTBEAT_NTF,

    BS_G200MS_SYSPARASET_REQ,
    G200MS_BS_SYSPARASET_ACK,
    G200MS_BS_SYSPARASET_NACK,

    BS_G200MS_SYSIPSET_REQ,
    G200MS_BS_SYSIPSET_ACK,
    G200MS_BS_SYSIPSET_NACK,

    BS_G200MS_G200_SYSMAIN_REQ, //系统维护,返回:版本信息,license信息
    G200_G200MS_BS_SYSMAIN_ACK,
    G200_G200MS_BS_SYSMAIN_NACK,

    BS_G200MS_SYSQUIT_REQ, //客户端用户退出请求
    G200MS_BS_SYSQUIT_ACK,
    G200MS_BS_SYSQUIT_NACK,

	BS_G200MS_G200_DEVICESTATUS_REQ,//请求编码器设备的在线状态
	G200_G200MS_BS_DEVICESTATUS_ACK,
	G200_G200MS_BS_DEVICESTATUS_NACK,

	BS_G200MS_EXPORTCONFIG_REQ,//将配置文件打包到指定的目录下
	G200MS_BS_EXPORTCONFIG_ACK,
	G200MS_BS_EXPORTCONFIG_NACK,
	
	BS_G200MS_G200_MPLATSTATUS_REQ,
	G200_G200MS_BS_MPLATSTATUS_ACK,
	G200_G200MS_BS_MPLATSTATUS_NACK,

	BS_G200MS_3ASTATUS_REQ,
	G200MS_BS_3ASTATUS_ACK,
	G200MS_BS_3ASTATUS_NACK,

    BS_G200MS_CAMERALIST_REQ,
	G200MS_BS_CAMERALIST_ACK,
	G200MS_BS_CAMERALIST_NACK,

	BS_G200MS_ENCODERLIST_REQ,
	G200MS_BS_ENCODERLIST_ACK,
	G200MS_BS_ENCODERLIST_NACK
};

//客户端配置时的错误返回码
enum ErrorCode
{
    ERROR_USERNAME_PSW_NOEXISTING = 1000,       //错误的用户名和密码
    ERROR_USERHASLOGINNED         = 1001,       //用户已经登录
	ERROR_EXCEEDLIMIT             = 1002,       //添加设备时超过软件支持的最大限度
	ERROR_LICENSE_INVALID         = 1003,       //license无效不允许添加设备
	ERROR_WRITEFILE_FAILED        = 1004        //写文件失败
};
struct TMatrixItem
{
public:
	TMatrixItem()
	{
		memset(this, 0, sizeof(TMatrixItem));
	}
	
public:
	u32 GetMatrixID()
	{
		return ntohl(m_dwMatrixID);
	}

	void SetMatrixID(const u32 dwMatrixID)
	{
		m_dwMatrixID = htonl(dwMatrixID);
	}
	
	u8 GetMatrixType()
	{
		return m_byMatrixType;
	}

	void SetMatrixType(const u8 byMatrixType)
	{
		m_byMatrixType = byMatrixType;
	}
	
    void SetMatrixName(const s8* pszMatrixName)
    {
        if (pszMatrixName == NULL)
        {
            return;
        }
        strncpy(m_achMatrixName,pszMatrixName,MAXLEN_NAME);
        m_achMatrixName[MAXLEN_NAME] = '\0';
    }

    const s8* GetMatrixName()
    {
        return m_achMatrixName;
    }

    void SetMatrixInportNum(s32 nPortNum)
    {
        m_nMatrixInportNum = nPortNum;
    }
    s32 GetMatrixInportNum()
    {
        return m_nMatrixInportNum;
    }

    void SetMatrixOutportNum(s32 nPortNum)
    {
        m_nMatrixOutPortNum = nPortNum;
    }
    s32 GetMatrixOutportNum()
    {
        return m_nMatrixOutPortNum;
    }
protected:
    s8 m_achMatrixName[MAXLEN_NAME + 1];
    s32 m_nMatrixInportNum;
    s32 m_nMatrixOutPortNum;
	u32 m_dwMatrixID;
	u8 m_byMatrixType;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TVideoOutItem
{
public:
    TVideoOutItem()
    {
	    memset(this, 0, sizeof(TVideoOutItem));
    }

    u32 GetMatrixID()
    {
	    return m_dwMatrixID;
    }

    void SetMatrixID(const u32 dwMatrixID)
    {
	    m_dwMatrixID = dwMatrixID;
    }

    BOOL32 IsConnMatrix()
    {
	    return m_bConnMatrix;
    }

    void SetConnMatrix(BOOL32 bConnMatrix)
    {
	    m_bConnMatrix = bConnMatrix;
    }

    void SetEncoder(TBasePu &tBasePu)
    {
        m_tEncoder = tBasePu;
    }

    TBasePu& GetEncoder()
    {
        return m_tEncoder;
    }

    u16 GetVideoOutPort(const u8 byVidChnId)
    {
	    if (byVidChnId > MAX_PU_CHNNUM)
	    {
		    return m_awVideoOutPort[MAX_PU_CHNNUM-1];
	    }

	    return m_awVideoOutPort[byVidChnId];
    }

    void SetVideoOutPort(const u8 byVidChnId, const u16 wPort)
    {
	    if (byVidChnId < MAX_PU_CHNNUM)
	    {
		    m_awVideoOutPort[byVidChnId] = wPort;
	    }
    }

    void SetVideoSrcPortNum(s32 nPortNum)
    {
        m_nVideoSrcPortNum = nPortNum;
    }

    s32 GetVideoSrcPortNum()
    {
        return m_nVideoSrcPortNum;
    }
protected:
	u32 m_dwMatrixID;
    s32 m_nVideoSrcPortNum;
	BOOL32 m_bConnMatrix;
    TBasePu m_tEncoder;
	u16 m_awVideoOutPort[MAX_PU_CHNNUM];   //每个视频源通道号对应的矩阵输出端口号
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TVideoInItem
{
public:
	TVideoInItem()
	{
		memset(this, 0, sizeof(TVideoInItem));
	}
	
public:
	u32 GetMatrixID()
	{
		return ntohl(m_dwMatrixID);
	}

	void SetMatrixID(const u32 dwMatrixID)
	{
		m_dwMatrixID = htonl(dwMatrixID);
	}
	
	u8 GetMatrixType()
	{
		return m_byMatrixType;
	}

	void SetMatrixType(const u8 byMatrixType)
	{
		m_byMatrixType = byMatrixType;
	}
	
	TBasePu& GetCam()
	{
		return m_tCam;
	}
    
	void SetCam(const TBasePu& tCam)
	{
		m_tCam = tCam;
	}
	
//去掉
// 	LPCSTR GetCamName()
// 	{
// 		return m_tCam.GetObjName();
// 	}
// 
// 	void SetCamName(LPCSTR lpszName)
// 	{
// 		if (NULL == lpszName || strlen(lpszName) == 0)
// 		{
// 			return;
// 		}
// 		
// 		m_tCam.SetObjName(lpszName);
// 	}
	
	u16 GetVideoInPort()
	{
		return ntohs(m_wVideoInPort);
	}

	void SetVideoInPort(const u16 wPort)
	{
		m_wVideoInPort = htons(wPort);
	}

    void SetRegisterStatus(BOOL32 bRegiste)
    {
        m_bRegiste = bRegiste;
    }
    BOOL32 GetRegisterStatus()
    {
        return m_bRegiste;
    }

    void SetCamAddr(s32 nCamAddr)
    {
        m_nCamAddr = nCamAddr;
    }
    s32 GetCamAddr()
    {
        return m_nCamAddr;
    }

    void SetCamType(s32 nType)
    {
        m_nCamType = nType;
    }
    s32 GetCamType()
    {
        return m_nCamType;
    }
protected:
	u32 m_dwMatrixID;
	u8 m_byMatrixType;
	TBasePu m_tCam;
	u16 m_wVideoInPort;

    BOOL32 m_bRegiste;
    s32 m_nCamAddr;
    s32 m_nCamType;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TG200CfgBase
{
public:
	TG200CfgBase()
	{
		memset(this, 0, sizeof(TG200CfgBase));
	}
	
	BOOL32 LoadCfg(LPCSTR lpszCfgFilePath)
	{
		char szCmuNo[LEN_KDM_NO + 1] = {0};
		s32 dwTmp;
		s8 szIP[MAX_IP_ADDR] = { 0 };
		char szSuperDomainName[MAXLEN_NAME+1] = {0};
		char szPath[MAXLEN_FILEPATH+1] = {0};
		
		GetRegKeyString(lpszCfgFilePath, SECTION_MPLAT, KEY_MPLATIP, "127.0.0.1", szIP, MAX_IP_ADDR);
		SetMPlatIP(szIP);
		
		GetRegKeyString(lpszCfgFilePath,SECTION_MPLAT,KEY_3AIP,"127.0.0.1",szIP,MAX_IP_ADDR);
		Set3AIP(szIP);

		GetRegKeyInt(lpszCfgFilePath, SECTION_MPLAT, KEY_MPLATPORT, 1722, &dwTmp);
		SetMPlatPort((u16)dwTmp);

		GetRegKeyInt(lpszCfgFilePath, SECTION_MPLAT,KEY_3APORT,1722,&dwTmp);
		Set3APort(dwTmp);

		GetRegKeyInt(lpszCfgFilePath,SECTION_MPLAT,KEY_MPLATTYPE,1,&dwTmp);
		SetMplatType(dwTmp);
		
		GetRegKeyString(lpszCfgFilePath, SECTION_SELFPLAT, KEY_SELFPLAT_NO, "55010000000000000050000000000000", szCmuNo, LEN_KDM_NO+1);
		SetG200Id(szCmuNo);
		
		GetRegKeyString(lpszCfgFilePath, SECTION_SELFPLAT, KEY_SELFPLAT_IP, "127.0.0.1", szIP, MAX_IP_ADDR);
		SetSelfPlatIP(szIP);
		
		GetRegKeyInt(lpszCfgFilePath, SECTION_SELFPLAT, KEY_SELFPLAT_PORT, 1722, &dwTmp);
		SetSelfPlatPort((u16)dwTmp);
		
		GetRegKeyInt(lpszCfgFilePath, SECTION_SELFPLAT, KEY_PUI_ENABLE, 1, &dwTmp);
		SetIsRunPui((BOOL32)dwTmp);

		GetRegKeyInt(lpszCfgFilePath, SECTION_REGSRV, KEY_REGSRV_PORT, 20000, &dwTmp);
		SetRegSrvListenPort((u16)dwTmp);
		
        GetRegKeyInt(lpszCfgFilePath,SECTION_DEVICENO,KEY_CURMATRIXNUM,0,&dwTmp);
		SetMatrixNum(dwTmp);

		GetRegKeyInt(lpszCfgFilePath,SECTION_DEVICENO,KEY_CURMAXMATRIXID,1,&dwTmp);
        SetMaxMatrixId(dwTmp);

		GetRegKeyInt(lpszCfgFilePath,SECTION_DEVICENO,KEY_CURCAMNUM,0,&dwTmp);
		SetCamNum(dwTmp);

        GetRegKeyInt(lpszCfgFilePath,SECTION_DEVICENO,KEY_CURMAXCAMNO,1,&dwTmp);
        SetMaxCamNO(dwTmp);

		GetRegKeyInt(lpszCfgFilePath,SECTION_DEVICENO,KEY_CURENCODERNUM,0,&dwTmp);
		SetEncoderNum(dwTmp);

        GetRegKeyInt(lpszCfgFilePath,SECTION_DEVICENO,KEY_CURMAXENCODERNO,1,&dwTmp);
        SetMaxEncoderNO(dwTmp);

		return TRUE;
	}
	
	void SetMPlatIP(LPCSTR lpszMPlatIP)
	{
		if (NULL == lpszMPlatIP)
		{
			return;
		}
		
		u32 dwIpAddr = inet_addr(lpszMPlatIP);
		if(dwIpAddr == INADDR_NONE)
		{
			OspLog(LOGLVL_EXCEPTION, "Failed to SetMPlatIP[%s]\n", lpszMPlatIP);
			return;
		}
		
		m_dwMPlatIP = dwIpAddr;
	}
	void Set3AIP(LPCSTR lpsz3AIP)
	{
		if (NULL == lpsz3AIP)
		{
			return;
		}
		
		u32 dwIpAddr = inet_addr(lpsz3AIP);
		if(dwIpAddr == INADDR_NONE)
		{
			OspLog(LOGLVL_EXCEPTION, "Failed to Set3AIP[%s]\n", lpsz3AIP);
			return;
		}
		
		m_dw3AIP = dwIpAddr;
	}

	u32 GetMPlatIP() const
	{
		return m_dwMPlatIP;
	}
	
	u32 Get3AIP() const
	{
		return m_dw3AIP;
	}

	void SetMPlatPort(u16 wPort)
	{
		m_wMPlatPort = htons(wPort);
	}

	void Set3APort(u16 wPort)
	{
		m_w3APort = htons(wPort);
	}

	u16 GetMPlatPort() const
	{
		return ntohs(m_wMPlatPort);
	}
	
	u16 Get3APort() const
	{
		return ntohs(m_w3APort);
	}

	u8 GetMplatType()
	{
		return m_byMplatType;
	}

	void SetMplatType(u8 byType)
	{
		m_byMplatType = byType;
	}

	void SetSelfPlatIP(LPCSTR lpszSelfPlatIP)
	{
		if (NULL == lpszSelfPlatIP)
		{
			return;
		}
		
		u32 dwIpAddr = inet_addr(lpszSelfPlatIP);
		if (INVALID_NODE == dwIpAddr)
		{
			OspLog(LOGLVL_EXCEPTION, "Failed to SetSelfPlatIP[%s]\n", lpszSelfPlatIP);
			return;
		}
		
		m_dwSelfPlatIP = dwIpAddr;
	}

	u32 GetSelfPlatIP() const
	{
		return m_dwSelfPlatIP;
	}
	
	void SetSelfPlatPort(u16 wPort)
	{
		m_wSelfPlatPort = htons(wPort);
	}

	u16 GetSelfPlatPort() const
	{
		return ntohs(m_wSelfPlatPort);
	}

	void SetRegSrvListenPort(u16 wPort)
	{
		m_wRegSrvListenPort = htons(wPort);
	}
	
	u16 GetRegSrvListenPort() const
	{
		return ntohs(m_wRegSrvListenPort);
	}
	
	
    void SetG200Id(const TKDMNO& tSelfPlatNo)
	{
		m_tBasePlat.SetKdmNO(tSelfPlatNo);
	}

    void SetG200Id(LPCSTR lpszSelfPlatNo)
	{
		if (NULL == lpszSelfPlatNo || strlen(lpszSelfPlatNo) != 32)
		{
			OspLog(LOGLVL_EXCEPTION, "Failed to SetG200Id[%s], because of null string or length of string less than 32!\n", lpszSelfPlatNo);
			return;
		}
		
		m_tBasePlat.SetNOString(lpszSelfPlatNo);
	}

    TKDMNO GetG200Id(void) const
	{
		return m_tBasePlat.GetKdmNo();
	}

    void GetG200Id(LPSTR lpszSelfPlatNo) const
	{
		strncpy(lpszSelfPlatNo, m_tBasePlat.GetNOString(), LEN_KDM_NO);
	}
	
    void SetDevName(LPCSTR lpszDevName)
	{
		m_tBasePlat.SetObjName(lpszDevName);
	}

    LPCSTR GetDevName(void) const
	{
		return m_tBasePlat.GetNOString();
	}
	
	void SetBindRtCfgPath(LPCSTR lpszCfgPath)
	{
		if (NULL == lpszCfgPath || strlen(lpszCfgPath) == 0)
		{
			return;
		}
		
		u16 wLen = strlen(lpszCfgPath) < MAXLEN_FILEPATH ? strlen(lpszCfgPath) : MAXLEN_FILEPATH;
		strncpy(m_abyBindRtCfgPath, lpszCfgPath, wLen);
        if (m_abyBindRtCfgPath[wLen-1] != '/')
        {
            m_abyBindRtCfgPath[wLen] = '/';
            m_abyBindRtCfgPath[wLen + 1] ='\0';
        }
	}

	LPCSTR GetBindRtCfgPath() const
	{
		return m_abyBindRtCfgPath;
	}
	
	TBaseCmu GetBaseCmu() const
	{
		return m_tBasePlat;
	}
	
    BOOL32 IsRunPui() const
	{
		return m_bIsRunPui;
	}

    void SetIsRunPui(BOOL32 bIsRun)
	{
		m_bIsRunPui = bIsRun;
	}
	
    void SetBootTime(s32 nBootTime)
	{
		m_nBootTime = nBootTime;
	}

    s32 GetBootTime() const
	{
		return m_nBootTime;
	}
	
    void SetMaxMatrixId(s32 nMatrixId)
    {
        m_nCurMaxMatrixId = nMatrixId;
    }
    s32 GetMaxMatrixId()
    {
        return m_nCurMaxMatrixId;
    }
	
	void SetMatrixNum(s32 nMatrixNum)
	{
		m_nCurMatrixNum = nMatrixNum;
	}
	s32 GetMatrixNum()
	{
		return m_nCurMatrixNum;
	}

    void SetMaxEncoderNO(s32 nEncoderNO)
    {
        m_nCurMaxEncoderNO = nEncoderNO;
    }
    s32 GetMaxEncoderNO()
    {
        return m_nCurMaxEncoderNO;
    }
	void SetEncoderNum(s32 nEncoderNum)
	{
		m_nCurEncoderNum = nEncoderNum;
	}
	s32 GetEncoderNum()
	{
		return m_nCurEncoderNum;
	}

    void SetMaxCamNO(s32 nCamNO)
    {
        m_nCurMaxCamNO = nCamNO;
    }
    s32 GetMaxCamNO()
    {
        return m_nCurMaxCamNO;
    }
    void SetCamNum(s32 nCamNum)
	{
		m_nCurCamNum = nCamNum;
	}
	s32 GetCamNum()
	{
		return m_nCurCamNum;
	}

    const s8* GetKdmNoHead()
    {
        strncpy(m_achKdmNoHead,m_tBasePlat.GetNOString(),sizeof(m_achKdmNoHead) -1);
        return m_achKdmNoHead;
    }
protected:
	TBaseCmu m_tBasePlat;
	u32 m_dwMPlatIP;
	u16 m_wMPlatPort;
	u32 m_dw3AIP;
	u16 m_w3APort;
	u8 m_byMplatType;
	u32 m_dwSelfPlatIP;
    u16 m_wSelfPlatPort;
	u16 m_wRegSrvListenPort;
	BOOL32 m_bIsRunPui;	
	s8 m_abyBindRtCfgPath[MAXLEN_FILEPATH+1];
	
    s32 m_nCurMaxMatrixId;
	s32 m_nCurMatrixNum;

    s32 m_nCurMaxEncoderNO;
	s32 m_nCurEncoderNum;

    s32 m_nCurMaxCamNO;
	s32 m_nCurCamNum;

	s32 m_nBootTime;

    s8 m_achKdmNoHead[18 + 1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TLicenseInfo 
{
	BOOL32 bValid;
	s32    nMaxVideoNum;
	s32    nMaxEncoderNum;
}
#ifndef WIN32
__attribute__((packed))
#endif
;

#ifdef WIN32
#pragma pack(pop)
#endif

#endif