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
	
    LPCSTR GetStrState();	//��ȡ��ǰInstance��״̬���ַ���
	
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
    void InstanceEntry(CMessage *const pcMsg){};
	
    void OnIdle(const CMessage *const pcMsg);	    //Idle״̬��ʵ��������
    void OnService(const CMessage *const pcMsg);    //Service״̬��ʵ��������
    BOOL m_bInitSdk;

};

typedef zTemplate<CSdkSsnIns, 1, CAppNoData, MAX_ALIAS_LEN> CSdkSsnApp;
#endif