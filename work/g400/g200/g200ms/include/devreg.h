#ifndef _VC_REG_H
#define _VC_REG_H

#include "g200comm.h"
#include "osp.h"
#include "kdmlist.h"
#include "monmsg.h"
#include "xmlmsg.h"
#include "cnetmanager.h"

#ifdef WIN32
#include "io.h"
//#include "iphlpapi.h"
#include "winbase.h"
#endif

typedef CKdmListEx<TVideoInItem, 1> TNeedRegDevList;

typedef CKdmListEx<TBasePu, 1> THasRegDevList;

#define  CHECK_WRITE_DB_RET(data,errstring)\
if(!data) {G200MSLog(LOG_DEVMGR_MOD,LOGLVL_EXCEPTION,errstring); g_cDevMgrApp.SetLastError(ERROR_WRITEFILE_FAILED); return FALSE;}

#define G200MS_CLIENTCONN_INTV  10*1000
#define XML_CMDSTR_LEN           2*1024
#define GETMAXCAMERA_PERTIME     140
#define GETMAXENCODER_PERTIME    6

class CDevMgrData
{
public:
	CDevMgrData();

public:
	BOOL32 LoadVideoInRtCfg();

	TNeedRegDevList *GetNeedRegDevListPtr();
	void AddNeedRegDev(const TVideoInItem& tItem);

	THasRegDevList* GetHasRegDevListPtr();
	void AddHasRegDev(const TBasePu& tVideoInItem);

	BOOL32 bInit3ASdk();
	void SetInit3ASdk(BOOL32 bInit);

	BOOL32 bG200OnLine();
	void SetG200OnLine(BOOL32 bOnline);

    BOOL32 GetLoginStatus();
    void SetLoginStatus(BOOL32 bStatus);

    BOOL32 IsHasInit3ALib();
    void SetInit3ALib(BOOL32 bStatus);


    BOOL32 IsFirstConn3A();
    void SetFirstConn3AFlag(BOOL32 bFlag);

    NetConfigure GetNicInfo(const s8* pszIp);
    BOOL32 SetNicInfo(const s8* pszOldIp, const s8 *achIp, const s8 *achMask, const s8 *achGateWay);

	s32 GetLastError();
	void SetLastError(s32 dwError);

	TLicenseInfo GetLicenseInfo() const;
	void SetLicenseInfo(TLicenseInfo& tLicenseInfo);
protected:
	TNeedRegDevList m_tNeedRegDevList;  //需要注册的设备信息
	THasRegDevList  m_tHasRegDevList;   //已经注册成功的设备信息
	BOOL32 m_bInit3ASdk;    //是否成功初始化3ASSdk
	BOOL32 m_bG200OnLine;   //G200是否在线
    BOOL32 m_bLogin;//是否有用户已经登录

    BOOL32 m_bHasInit3ALib;//断链重连时，用于判断3asdk库是否已经退出

    BOOL32 m_bFirst;//第一次连上3a时，读取配置文件中未注册的设备进行注册

    CNetManager m_cNetManager;
	s32 m_dwError;
	TLicenseInfo m_tLincenseInfo;
};

class CDevMgrIns : public CInstance
{
public:
	CDevMgrIns();

public:
	void SendMsgToG200(CMonMsg& cMonMsg, u16 wEvent);
    void SendMsgToG200(CMessage *const pcMsg,u16 wEvent);
	void StartDevReg();

	void ProcG200Reg(const CMessage * pcMsg);
	void UpdateVideoInCfg(TVideoInItem& tItem);

	void NotifyHasRegDevInfo();
private:
	enum{STATE_IDLE = 0, STATE_SERVICE};
	
    void InstanceEntry(CMessage *const pcMsg){};
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
    BOOL32 RegisteCameraTo3A(const s8* pszKdmNo,const s8* pszCameraName);
    BOOL32 ModifyCameraTo3A(const s8* pszKdmNo,const s8* pszCameraName);
    BOOL32 UnRegisteCameraFrom3A(s8* pszKdmNo);
	
    void OnIdle(CMessage *const pcMsg,CApp* pcApp);	//Idle状态下实例处理函数
    void OnService(CMessage *const pcMsg,CApp* pcApp);	//Service状态下实例处理函数
	BOOL32 OnCommonMsg(CMessage *const pcMsg);
	void SendMsgToClientConn(CXmlMsg &cXmlMsg,CMessage *const pcMsg);
    void SendMsgToClientConn(CMessage *const pcMsg);
    LPCSTR GetStrState();
    void OnDealData(CMessage *const pcMsg, CApp* pcApp);
    void ProcessLogin(CXmlMsg &cXmlMsg,CMessage *const pcMsg,CApp* pcApp);

    void ProcessAddMatrix(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp); 
    void ProcessDeleteMatrix(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    void ProcessUpdateMatrix(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);

    void ProcessAddEncoder(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    void ProcessDeleteEncoder(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    void ProcessUpdateEncoder(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);

    void ProcessAddCamera(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    void ProcessDeleteCamera(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    void ProcessUpdateCamera(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);

    void ProcessAddLink(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    BOOL32 InsertLinkToFile(CXmlMsg &cXmlMsg);
    void ProcessDelLink(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    BOOL32 DeleteLinkFromFile(CXmlMsg &cXmlMsg);

    void ProcessGetMatrixList(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    BOOL32 SelectMatrixListFromFile(CXmlMsg& cXmlMsg);

    void ProcessGetDeviceList(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    void ProcessUpdateUser(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);


    BOOL32 InsertMatrixToFile(CXmlMsg &cXmlMsg,s32& nMatrixId,TMatrixItem &tMatrixItem);
    BOOL32 UpdateMatrixToFile(CXmlMsg &cXmlMsg);
    BOOL32 DeleteMatrixFromFile(CXmlMsg &cXmlMsg);

    BOOL32 InsertEncoderToFile(CXmlMsg &cXmlMsg,s8* pszKdmNo,TVideoOutItem& tOutItem);
    BOOL32 UpdateEncoderToFile(CXmlMsg &cXmlMsg);
    BOOL32 DeleteEncoderFromFile(CXmlMsg &cXmlMsg);

    BOOL32 InsertCameraToFile(CXmlMsg &cXmlMsg,s8* pszKdmNo,TVideoInItem& tInItem);
    BOOL32 UpdateCameraToFile(CXmlMsg &cXmlMsg);
    BOOL32 DeleteCameraFromFile(CXmlMsg &cXmlMsg);

    BOOL32 UpdateUserFile(CXmlMsg &cXmlMsg);

    void ProcessGetSysConf(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    void ProcessSetSysPara(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    BOOL32 UpdateSysParaToFile(CXmlMsg& cXmlMsg);

    void ProcessSetSysIp(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    BOOL32 UpdateSysIpToFile(CXmlMsg& cXmlMsg);

    void ProcessSystemReboot(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    
    void ProcessHeartBeatFromClient(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
    void ProcessClientConnTimeOut(CMessage *const pcMsg,CApp* pcApp);

    void ProcessLogOut(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);

	void ProcessG200ConfigTar(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);

	void ProcessGet3AStatus(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);

	void ProcessGetCameraList(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
	void ProcessGetEncoderList(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp);
private:
    u32 m_dwDstIId;
    u32 m_dwNodeId;
    s8 m_achXmlCmd[XML_CMDSTR_LEN];
};

typedef zTemplate<CDevMgrIns, 1, CDevMgrData, MAX_ALIAS_LEN> CDevMgrApp;

extern CDevMgrApp g_cDevMgrApp;

#endif