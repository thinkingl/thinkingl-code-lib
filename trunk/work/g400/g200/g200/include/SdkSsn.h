#ifndef _SDK_SSN_H
#define _SDK_SSN_H

#include "Common.h"
#include "G200Data.h"

class CSdkSsnIns : public CInstance
{
public:
	CSdkSsnIns();

private:
	enum{IDLE, SERVICE};
	
    LPCSTR GetStrState();	//获取当前Instance的状态的字符串
	
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
    void InstanceEntry(CMessage *const pcMsg){};
	
    void OnIdle(const CMessage *const pcMsg);	    //Idle状态下实例处理函数
    void OnService(const CMessage *const pcMsg);    //Service状态下实例处理函数
    BOOL m_bInitSdk;

};

typedef zTemplate<CSdkSsnIns, 1, CAppNoData, MAX_ALIAS_LEN> CSdkSsnApp;
#endif