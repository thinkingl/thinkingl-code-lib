#ifndef _VC_SSH_H
#define _VC_SSH_H

#include "Common.h"
#include "cammanager.h"
#include "G200Data.h"
#include "xmlmsg.h"
class CVcSchedAppData
{
public:
	CVcSchedAppData();

public:
	u32 GetRegFailNum();
	void AddRegFailNum();
	void ClearRegFailNum();

	void SetHasFinishGetVideoInRt(BOOL32 bFinish);
	void SetHasFinishGetVideoOutRt(BOOL32 bFinish);
	BOOL32 HasFinishGetVideoInRt();
	BOOL32 HasFinishGetVideoOutRt();

protected:
	u32 m_dwRegFailNum;

	BOOL32 m_bHasGetVideoInRt;
	BOOL32 m_bHasGetVideoOutRt;
};


class CVcSchedIns : public CInstance
{
public:
	CVcSchedIns();

public:
	BOOL32 SendMsgToSdkSsn(const CMonMsg &cMonMsg, u16 wEvent);
	BOOL32 SendMsgToPuiSsn(const u16 wInstId, u16 wEvent, const u8 * const pbyMsg, u16 wLen);
	BOOL32 SendMsgToCfgMgr(const CMonMsg &cMonMsg, u16 wEvent);
    BOOL32 SendMsgToG200MS(CXmlMsg &cXmlMsg, const CMessage *const pcMsg);
    void DeleteEncoder(TKDMNO &tKdmNo,s32 nCurMatrixId,TVideoOutRelation* ptVideoOutRt);

	void ProcMsgFromSdk(const CMessage * pcMsg);
	void ProcMsgFromPui(const CMessage * pcMsg);
	void ProcMsgFromCfgMgr(const CMessage * pcMsg);
    void ProcMsgFromG200MS(const CMessage* pcMsg);
    void OnDealData(const CMessage* pcMsg);

	void ProcPuiReg(const CMessage * pcMsg);	
    void ProcPuReg(const CMessage * pcMsg);
	void ProcPuiDisconn(const CMessage * pcMsg);
	void OnPuDisconn(const TKDMNO& tKdmNo);

	void ProcSendStream(const CMessage * pcMsg);    //处理上级平台请求码流消息
	void ProcStopStream(const CMessage * pcMsg);    //处理上级平台停止码流消息
	void NotifyDstStopRcvStream(const TKDMNO& tKdmNo);   //通知上级平台停止接收经过指定编码器发送的码流

	void NotifyCamStatus(const u32 dwMatrixID, BOOL32 bOnline = TRUE);     //向上级平台上报某个矩阵上的所有摄像头设备信息
	void NotifyOneCamStatus(const TKDMNO& tKdmNo, BOOL32 bOnline = TRUE);  //向上级设备上报某一个摄像头设备状态信息

	BOOL32 MatrxiCtrl(const TChannel& tDevChn, TMatrixInfo& tMatrixInfo);   //矩阵控制

	void ProcDisconnWithMplat(void);   //处理与上级平台断链
    void ProcConnWithMplat(void);      //上级平台建链时,上报当前设备状态

	void ProcGetMatrixInfoNty(const CMessage * pcMsg);   //获取矩阵信息处理函数
	void ProcGetBindInRtNty(const CMessage * pcMsg);     //获取矩阵输入绑定关系处理函数
	void ProcGetBindOutRtNty(const CMessage * pcMsg);    //获取矩阵输出绑定关系处理函数
	void ProcAddBindInRtNty(const CMessage * pcMsg);     //添加矩阵绑定关系处理函数

private:
	enum{STATE_IDLE = 0, STATE_SERVICE};
	
    void InstanceEntry(CMessage *const pcMsg){};
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
	
    void OnIdle(const CMessage *const pcMsg);	//Idle状态下实例处理函数
    void OnService(const CMessage *const pcMsg);	//Service状态下实例处理函数

	BOOL32 OnCommonMsg(const CMessage *const pcMsg);
	
    LPCSTR GetStrState();

	void OnPrintEvent(CMessage *const pcMsg);   //打印函数

};

typedef zTemplate<CVcSchedIns, 1, CVcSchedAppData, MAX_ALIAS_LEN> CVcSchedApp;

extern CVcSchedApp g_cVcSchedApp;

#endif