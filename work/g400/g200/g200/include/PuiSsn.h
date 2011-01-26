#ifndef _PUI_SSN_H
#define _PUI_SSN_H

#include "Common.h"

class CPuiSsnIns : public CInstance
{
private:
    u32 m_dwDstIId;	//连接的Osp Dst id
    u32 m_dwNodeId;	//连接的Osp node id
	
private:
    enum{IDLE, WAITING, SERVICE};
	
    LPCSTR GetStrState();	//获取当前Instance的状态的字符串
	
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
    void InstanceEntry(CMessage *const pcMsg);
	
    void OnIdle(const CMessage *const pcMsg);	    //Idle状态下实例处理函数
    void OnWaiting(const CMessage *const pcMsg);	//Waiting状态下实例处理函数
    void OnService(const CMessage *const pcMsg);    //Service状态下实例处理函数
public:
    CPuiSsnIns();
};
typedef zTemplate<CPuiSsnIns, 1, CAppNoData, MAX_ALIAS_LEN> CPuiSsnApp;


#endif