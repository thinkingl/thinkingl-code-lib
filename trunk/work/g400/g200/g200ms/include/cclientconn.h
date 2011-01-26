#ifndef _CCLIENT_CONN_H
#define _CCLIENT_CONN_H

#include "osp.h"
#include "httpcommon.h"
#include "xmlmsg.h"
#include "monmsg.h"
class CClientIns:public CInstance
{
private:
    enum{STATE_IDLE,STATE_SERVER};
    void InstanceEntry(CMessage *const pcMsg){};
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
    void SendXml2Client(CMessage *const pcMsg);
    LPCSTR GetStrState();
    void OnServer(CMessage *const pcMsg, CApp* pcApp);
    void SendMsgToDevMgr(CMonMsg &cMonMsg, u16 wEvent);
    void SendMsgToDevMgr(CMessage *const pcMsg);
private:
};

typedef zTemplate<CClientIns,1> CClientApp;

#endif /*_CCLIENT_CONN_H*/