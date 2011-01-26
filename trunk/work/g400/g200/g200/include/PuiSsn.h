#ifndef _PUI_SSN_H
#define _PUI_SSN_H

#include "Common.h"

class CPuiSsnIns : public CInstance
{
private:
    u32 m_dwDstIId;	//���ӵ�Osp Dst id
    u32 m_dwNodeId;	//���ӵ�Osp node id
	
private:
    enum{IDLE, WAITING, SERVICE};
	
    LPCSTR GetStrState();	//��ȡ��ǰInstance��״̬���ַ���
	
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
    void InstanceEntry(CMessage *const pcMsg);
	
    void OnIdle(const CMessage *const pcMsg);	    //Idle״̬��ʵ��������
    void OnWaiting(const CMessage *const pcMsg);	//Waiting״̬��ʵ��������
    void OnService(const CMessage *const pcMsg);    //Service״̬��ʵ��������
public:
    CPuiSsnIns();
};
typedef zTemplate<CPuiSsnIns, 1, CAppNoData, MAX_ALIAS_LEN> CPuiSsnApp;


#endif