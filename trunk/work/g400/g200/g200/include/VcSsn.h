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

	void ProcSendStream(const CMessage * pcMsg);    //�����ϼ�ƽ̨����������Ϣ
	void ProcStopStream(const CMessage * pcMsg);    //�����ϼ�ƽֹ̨ͣ������Ϣ
	void NotifyDstStopRcvStream(const TKDMNO& tKdmNo);   //֪ͨ�ϼ�ƽֹ̨ͣ���վ���ָ�����������͵�����

	void NotifyCamStatus(const u32 dwMatrixID, BOOL32 bOnline = TRUE);     //���ϼ�ƽ̨�ϱ�ĳ�������ϵ���������ͷ�豸��Ϣ
	void NotifyOneCamStatus(const TKDMNO& tKdmNo, BOOL32 bOnline = TRUE);  //���ϼ��豸�ϱ�ĳһ������ͷ�豸״̬��Ϣ

	BOOL32 MatrxiCtrl(const TChannel& tDevChn, TMatrixInfo& tMatrixInfo);   //�������

	void ProcDisconnWithMplat(void);   //�������ϼ�ƽ̨����
    void ProcConnWithMplat(void);      //�ϼ�ƽ̨����ʱ,�ϱ���ǰ�豸״̬

	void ProcGetMatrixInfoNty(const CMessage * pcMsg);   //��ȡ������Ϣ������
	void ProcGetBindInRtNty(const CMessage * pcMsg);     //��ȡ��������󶨹�ϵ������
	void ProcGetBindOutRtNty(const CMessage * pcMsg);    //��ȡ��������󶨹�ϵ������
	void ProcAddBindInRtNty(const CMessage * pcMsg);     //��Ӿ���󶨹�ϵ������

private:
	enum{STATE_IDLE = 0, STATE_SERVICE};
	
    void InstanceEntry(CMessage *const pcMsg){};
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
	
    void OnIdle(const CMessage *const pcMsg);	//Idle״̬��ʵ��������
    void OnService(const CMessage *const pcMsg);	//Service״̬��ʵ��������

	BOOL32 OnCommonMsg(const CMessage *const pcMsg);
	
    LPCSTR GetStrState();

	void OnPrintEvent(CMessage *const pcMsg);   //��ӡ����

};

typedef zTemplate<CVcSchedIns, 1, CVcSchedAppData, MAX_ALIAS_LEN> CVcSchedApp;

extern CVcSchedApp g_cVcSchedApp;

#endif