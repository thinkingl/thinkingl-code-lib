#include "PuiSsn.h"
#include "monmsg.h"
#include "cmsevent.h"
#include "cmstypes.h"


CPuiSsnIns::CPuiSsnIns()
{
    m_dwNodeId = INVALID_NODE;
    m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);
}

/*====================================================================
������      : GetStrState
����        : ��ȡ״̬�ַ���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
06/03/16    3.9         ½ѩ��        ����
====================================================================*/
LPCSTR CPuiSsnIns::GetStrState()
{
    switch(CurState())
    {
    case IDLE:
        return "Idle";
    case WAITING:
        return "Waiting";
    case SERVICE:
        return "Service";
    default:
        return "δ֪״̬";
    }
}

/*====================================================================
������      : DaemonInstanceEntry
����        ��Daemonʵ����Ϣ������ں���
�㷨ʵ��    ��
����ȫ�ֱ�����
�������˵����CMessage *const pcMsg,  �������Ϣ
CApp* pcApp,            ����Ӧ��ָ��
����ֵ˵��  ����
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
06/03/20    3.9         ½ѩ��        ����
====================================================================*/
void CPuiSsnIns::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
    if(pcMsg->event == EV_G200_SET_INST_ALIAS)
    {
        //����ʵ����
        s32 nInsCount = pcApp->GetInstanceNumber();

        char szAlias[MAX_ALIAS_LEN]={0};
        strcpy(szAlias, "[G200-PuiSsn-Daemon]");
        pcApp->SetInstAlias(CInstance::DAEMON, szAlias, (u8)strlen(szAlias));

        for(u16 wIndex = 1; wIndex <= nInsCount; wIndex++)
        {
            sprintf(szAlias, "[G200/PuiSsn-%d]", wIndex);
            pcApp->SetInstAlias(wIndex, szAlias, (u8)strlen(szAlias));
        }
    }
}

/*====================================================================
������      : InstanceEntry
����        : ��ͨʵ�� osp ��Ϣ��ں���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: CMessage *const pcMsg--osp��Ϣ
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
06/03/16    3.9         ½ѩ��        ����
====================================================================*/
void CPuiSsnIns::InstanceEntry(CMessage *const pcMsg)
{
    G200Log(LOG_PUI_MOD,LOGLVL_DEBUG2,"%s[%s]:Receive Message[%s]\n",m_alias,GetStrState(),OspEventDesc(pcMsg->event));
    switch(CurState())
    {
    case IDLE:
        {
            OnIdle(pcMsg);
            break;
        }
    case WAITING:
        {
            OnWaiting(pcMsg);
            break;
        }
    case SERVICE:
        {
            OnService(pcMsg);
            break;
        }
    default:
        {
            G200Log(LOG_PUI_MOD, LOGLVL_EXCEPTION, "%s[%s-%d]:δ����״̬\n", m_alias, GetStrState(), CurState());
            break;
        }
    }
}

/*====================================================================
������      : OnIdle
����        : ��ͨʵ��Idle״̬��Ϣ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: CMessage *const pcMsg--osp��Ϣ
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
06/03/16    3.9         ½ѩ��        ����
====================================================================*/
void CPuiSsnIns::OnIdle(const CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case PUI_CMU_REG_REQ:
        {
            //Puiע������
            m_dwNodeId = pcMsg->srcnode;
            m_dwDstIId = pcMsg->srcid;

            OspNodeDiscCBRegQ(m_dwNodeId, m_appId, m_instId);

            post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), pcMsg->event, pcMsg->content, pcMsg->length);

            NextState(WAITING);

            SetTimer(EV_G200_PUI_REG_TIMEOUT, G200_TIMESPACE_PUI_REG_TIMER);

            break;
        }
    default:
        {
            //��ǰ״̬δ���崦����Ϣ
            G200Log(LOG_PUI_MOD, LOGLVL_IMPORTANT, "%s[%s]:��ǰû�н������״̬\n", m_alias, GetStrState());
            break;
        }
    }
}

/*====================================================================
������      : OnWaiting
����        : ��ͨʵ��Waiting״̬��Ϣ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: CMessage *const pcMsg--osp��Ϣ
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
06/03/16    3.9         ½ѩ��        ����
====================================================================*/
void CPuiSsnIns::OnWaiting(const CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case OSP_DISCONNECT:
        {
            //Osp������Ϣ
            KillTimer(EV_G200_PUI_REG_TIMEOUT);

            m_dwNodeId = INVALID_NODE;
            m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);

            post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PUI_DISCONNECT_NTY);

            NextState(IDLE);

            break;
        }
    case EV_G200_FORCE_PUI_DISCONNECT_CMD:
        {
            //�豸ǿ������
            post(m_dwDstIId, CMU_PUI_LOGOUT_CMD, NULL, 0, m_dwNodeId);
            if(m_dwNodeId != INVALID_NODE)
            {
                OspDisconnectTcpNode(m_dwNodeId);
            }
            else
            {
                post(MAKEIID(m_appId, m_instId), OSP_DISCONNECT);
            }
            break;
        }
    case CMU_PUI_REG_ACK:
        {
            KillTimer(EV_G200_PUI_REG_TIMEOUT);

            post(m_dwDstIId, pcMsg->event, pcMsg->content, pcMsg->length, m_dwNodeId);

            NextState(SERVICE);

            break;
        }
    case CMU_PUI_REG_NACK:
        {
            KillTimer(EV_G200_PUI_REG_TIMEOUT);

            post(m_dwDstIId, pcMsg->event, pcMsg->content, pcMsg->length, m_dwNodeId);

            if(m_dwNodeId != INVALID_NODE)
            {
                OspDisconnectTcpNode(m_dwNodeId);
            }
            else
            {
                m_dwNodeId = INVALID_NODE;
                m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);

                NextState(IDLE);
            }
            break;
        }
    case EV_G200_PUI_REG_TIMEOUT:
        {
            //�豸ע�ᳬʱ
            if(m_dwNodeId != INVALID_NODE)
            {
                OspDisconnectTcpNode(m_dwNodeId);
            }
            else
            {
                m_dwNodeId = INVALID_NODE;
                m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);

                post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PUI_DISCONNECT_NTY);

                NextState(IDLE);
            }
            break;
        }
    default:
        {
            //��ǰ״̬δ���崦����Ϣ
            G200Log(LOG_PUI_MOD, LOGLVL_IMPORTANT, "%s[%s]:��ǰû�н������״̬\n", m_alias, GetStrState());
            break;
        }
    }
}

/*====================================================================
������      : OnService
����        : ��ͨʵ��Service״̬��Ϣ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: CMessage *const pcMsg--osp��Ϣ
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
06/03/16    3.9         ½ѩ��        ����
====================================================================*/
void CPuiSsnIns::OnService(const CMessage *const pcMsg)
{
    CMonMsg cMonMsg( pcMsg->content, pcMsg->length );
    switch(pcMsg->event)
    {
    case OSP_DISCONNECT:
        {
            //Osp������Ϣ
            m_dwNodeId = INVALID_NODE;
            m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);

            post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PUI_DISCONNECT_NTY);

            NextState(IDLE);

            break;
        }
    case EV_G200_FORCE_PUI_DISCONNECT_CMD:
        {
            //�豸ǿ������
            post(m_dwDstIId, CMU_PUI_LOGOUT_CMD, NULL, 0, m_dwNodeId);
            if(m_dwNodeId != INVALID_NODE)
            {
                OspDisconnectTcpNode(m_dwNodeId);
            }
            else
            {
                post(MAKEIID(m_appId, m_instId), OSP_DISCONNECT);
            }
        }
        break;
    case PUI_CMU_PU_REG_REQ:
        {
            //Puע��
            //Pu��½����
            cMonMsg.SetSessID(cMonMsg.GetXuIns(), m_instId);

            post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), pcMsg->event, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());

            break;
        }
    default:
        {
            //������Ϣ
            if(GETAPP(pcMsg->srcid) == AID_G200_VCSCHED)
            {
                post(m_dwDstIId, pcMsg->event, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen(), m_dwNodeId);
                G200Log(LOG_PUI_MOD, LOGLVL_DEBUG1, "%s[%s]:����[%s]��Pui\n", m_alias, GetStrState(), OspEventDesc(pcMsg->event));
            }
            else
            {
                post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), pcMsg->event, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
                G200Log(LOG_PUI_MOD, LOGLVL_DEBUG1, "%s[%s]:�յ�Pui��Ϣ[%s]\n", m_alias, GetStrState(), OspEventDesc(pcMsg->event));
            }
            break;
        }
    }
}
