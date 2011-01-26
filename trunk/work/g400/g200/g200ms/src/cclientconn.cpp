#include "cclientconn.h"
#include "httpcommon.h"
#include "srvmain.h"


LPCSTR CClientIns::GetStrState()
{
	switch(CurState())
    {
    case STATE_IDLE:
        return "Idle";
    case STATE_SERVER:
        return "Service";
    default:
        return "δ֪״̬";
    }
}

void CClientIns::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{  
   
    G200MSLog(LOG_CLIENTCONN_MOD,LOGLVL_DEBUG2,"[G200ms/Clientconn-Daemon]:Receive Message[%s-%d]\n",
                    OspEventDesc(pcMsg->event), pcMsg->event);

    switch(pcMsg->event)
	{
	case EV_HS_SERVER_REG:
		{
			OspNodeDiscCBReg(pcMsg->srcnode, GetAppID(), GetInsID());
			BOOL32 connected = FALSE;
			post(pcMsg->srcid, EV_SERVER_HS_REG_ACK, &connected,sizeof(connected),  pcMsg->srcnode); //��httpServerһ��1
            NextState(STATE_SERVER);
			break;
		}
	case EV_HS_SERVER_UNREG:
		{
			NextState( STATE_IDLE);
			BOOL32 connected = FALSE;
			post(pcMsg->srcid, EV_SERVER_HS_REG_ACK,  &connected,sizeof(connected),  pcMsg->srcnode); 
			break;
		}
	//���տͻ��˷��͹�������
	case EV_HS_SERVER_KEDACOMXMLDATA:
		{
			if (GETAPP(pcMsg->srcid) == AID_G200MS_DEVMGR && pcMsg->srcnode == 0)
			{
                SendXml2Client(pcMsg);
			}
            else
            {
                CMonMsg cMonMsg;
                cMonMsg.SetSessID(pcMsg->srcid);
                cMonMsg.SetReserved1(pcMsg->srcnode);
                cMonMsg.SetMsgBody(pcMsg->content,pcMsg->length);

                SendMsgToDevMgr(cMonMsg,EV_HS_SERVER_KEDACOMXMLDATA);
            }
            
			break;
		}
	case OSP_DISCONNECT:
		{
			NextState(STATE_IDLE);
			break;
		}
    default:
        {
            //�豸����ģ�鷢�͵�Ӧ��
            G200MSLog(LOG_CLIENTCONN_MOD,LOGLVL_DEBUG2,"[G200ms/ClientConn-Daemon]:�յ��޷��������Ϣ[%s-%d]\n",
                OspEventDesc(pcMsg->event), pcMsg->event);
        }
        break;
	}
	return;
}



void CClientIns::SendXml2Client(CMessage *const pcMsg)
{
    CMonMsg* pcMonMsg = (CMonMsg*)pcMsg->content;
    G200MSLog(LOG_CLIENTCONN_MOD,LOGLVL_DEBUG2,"[G200ms/ClientConn-Daemon]:Send Message[%s-%d] to httpServer\n",
        OspEventDesc(pcMsg->event),pcMsg->event);
    u32 dwSrcId = pcMonMsg->GetSessID();
    u32 dwNode = pcMonMsg->GetReserved1();
    post(dwSrcId,EV_HS_SERVER_KEDACOMXMLDATA,pcMonMsg->GetMsgBody(),
        pcMonMsg->GetMsgBodyLen(),dwNode);
}

void CClientIns::SendMsgToDevMgr(CMonMsg &cMonMsg, u16 wEvent)
{
    //����Ϣת�����豸����ģ�鴦��
    post(MAKEIID(AID_G200MS_DEVMGR,CInstance::DAEMON),wEvent,cMonMsg.GetMonMsg(),
        cMonMsg.GetMonMsgLen());
}
void CClientIns::SendMsgToDevMgr(CMessage *const pcMsg)
{
    post(MAKEIID(AID_G200MS_DEVMGR,CInstance::DAEMON),pcMsg->event,pcMsg->content,pcMsg->length);
}