#ifndef _CFG_MGR_H
#define _CFG_MGR_H

#include "Common.h"
#include "kdmlist.h"

typedef CKdmListEx<TVideoOutItem,1> TItemList;

typedef CKdmListEx<TVideoInItem, 1> TWaitForRegDevList;

class CCfgMgrAppData
{
public:
	CCfgMgrAppData();

public:
	void SetCfgPath(LPCSTR lpszPath);
	LPCSTR GetCfgPath();
	
	//读取绑定关系配置文件
	BOOL32 ReadBindRtCfg(LPCSTR lpszCfgFilePath);
	BOOL32 ReadMatrixCfg();
	BOOL32 ReadVideoInCfg();
	BOOL32 ReadVideoOutCfg();

	TItemList* GetItemListPtr();
	void AddItem(TVideoOutItem& tItem);
	BOOL32 IsHasItem(const TKDMNO& tKdmNo);
	TVideoOutItem *GetItem(const TKDMNO& tKdmNo);
	void ClearItemList();
	
	TWaitForRegDevList* GetRegDevListPtr();
	void AddDev(const TVideoInItem& tVideoInItem);

	BOOL32 bRegSrvOnSvr();
	void SetRegSrvOnSvr(BOOL32 bOnSvr);
private:
	u8 GetMatrixType(s32 nMatrixId);
private:
	TMatrixItem m_tMatrixItem[G200_MAX_MATRIX_NUM];
	TItemList   m_tItemList;
	TWaitForRegDevList m_tWaitForRegDevList;
	u32 m_dwRealUsedMatrixNum;
	s8 m_abyCfgPath[MAXLEN_FILEPATH+2];

	BOOL32 m_bRegSrvOnSvr;   //注册服务器处理服务状态
};

class CCfgMgrIns : public CInstance
{
private:
	u32 m_dwNodeId;
	u32 m_dwDstId;

public:
	CCfgMgrIns();

private:
	enum{IDLE, SERVICE};
	
    LPCSTR GetStrState();	//获取当前Instance的状态的字符串
	
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
    void InstanceEntry(CMessage *const pcMsg){};
	
    void OnIdle(const CMessage *const pcMsg);	    //Idle状态下实例处理函数
    void OnService(const CMessage *const pcMsg);    //Service状态下实例处理函数
	BOOL32 OnCommonMsg(const CMessage *const pcMsg);

	void ConnToRegSrv();
	void RegisterToRegSrv();

	void SendMsgToVcSched(const CMonMsg &cMonMsg, u16 wEvent);

	void StartDevReg();
};

typedef zTemplate<CCfgMgrIns, 1, CCfgMgrAppData, MAX_ALIAS_LEN> CCfgMgrApp;

extern CCfgMgrApp  g_cCfgMgrApp;

#endif