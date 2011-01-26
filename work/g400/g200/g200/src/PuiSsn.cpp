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
函数名      : GetStrState
功能        : 获取状态字符串
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
06/03/16    3.9         陆雪忠        创建
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
        return "未知状态";
    }
}

/*====================================================================
函数名      : DaemonInstanceEntry
功能        ：Daemon实例消息处理入口函数
算法实现    ：
引用全局变量：
输入参数说明：CMessage *const pcMsg,  传入的消息
CApp* pcApp,            传入应用指针
返回值说明  ：无
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
06/03/20    3.9         陆雪忠        创建
====================================================================*/
void CPuiSsnIns::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
    if(pcMsg->event == EV_G200_SET_INST_ALIAS)
    {
        //设置实例名
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
函数名      : InstanceEntry
功能        : 普通实例 osp 消息入口函数
算法实现    :
引用全局变量:
输入参数说明: CMessage *const pcMsg--osp消息
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
06/03/16    3.9         陆雪忠        创建
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
            G200Log(LOG_PUI_MOD, LOGLVL_EXCEPTION, "%s[%s-%d]:未定义状态\n", m_alias, GetStrState(), CurState());
            break;
        }
    }
}

/*====================================================================
函数名      : OnIdle
功能        : 普通实例Idle状态消息处理函数
算法实现    :
引用全局变量:
输入参数说明: CMessage *const pcMsg--osp消息
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
06/03/16    3.9         陆雪忠        创建
====================================================================*/
void CPuiSsnIns::OnIdle(const CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case PUI_CMU_REG_REQ:
        {
            //Pui注册请求
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
            //当前状态未定义处理消息
            G200Log(LOG_PUI_MOD, LOGLVL_IMPORTANT, "%s[%s]:当前没有进入服务状态\n", m_alias, GetStrState());
            break;
        }
    }
}

/*====================================================================
函数名      : OnWaiting
功能        : 普通实例Waiting状态消息处理函数
算法实现    :
引用全局变量:
输入参数说明: CMessage *const pcMsg--osp消息
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
06/03/16    3.9         陆雪忠        创建
====================================================================*/
void CPuiSsnIns::OnWaiting(const CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case OSP_DISCONNECT:
        {
            //Osp断链消息
            KillTimer(EV_G200_PUI_REG_TIMEOUT);

            m_dwNodeId = INVALID_NODE;
            m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);

            post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PUI_DISCONNECT_NTY);

            NextState(IDLE);

            break;
        }
    case EV_G200_FORCE_PUI_DISCONNECT_CMD:
        {
            //设备强制退网
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
            //设备注册超时
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
            //当前状态未定义处理消息
            G200Log(LOG_PUI_MOD, LOGLVL_IMPORTANT, "%s[%s]:当前没有进入服务状态\n", m_alias, GetStrState());
            break;
        }
    }
}

/*====================================================================
函数名      : OnService
功能        : 普通实例Service状态消息处理函数
算法实现    :
引用全局变量:
输入参数说明: CMessage *const pcMsg--osp消息
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
06/03/16    3.9         陆雪忠        创建
====================================================================*/
void CPuiSsnIns::OnService(const CMessage *const pcMsg)
{
    CMonMsg cMonMsg( pcMsg->content, pcMsg->length );
    switch(pcMsg->event)
    {
    case OSP_DISCONNECT:
        {
            //Osp断链消息
            m_dwNodeId = INVALID_NODE;
            m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);

            post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_PUI_DISCONNECT_NTY);

            NextState(IDLE);

            break;
        }
    case EV_G200_FORCE_PUI_DISCONNECT_CMD:
        {
            //设备强制退网
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
            //Pu注册
            //Pu登陆请求
            cMonMsg.SetSessID(cMonMsg.GetXuIns(), m_instId);

            post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), pcMsg->event, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());

            break;
        }
    default:
        {
            //其它消息
            if(GETAPP(pcMsg->srcid) == AID_G200_VCSCHED)
            {
                post(m_dwDstIId, pcMsg->event, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen(), m_dwNodeId);
                G200Log(LOG_PUI_MOD, LOGLVL_DEBUG1, "%s[%s]:发送[%s]给Pui\n", m_alias, GetStrState(), OspEventDesc(pcMsg->event));
            }
            else
            {
                post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), pcMsg->event, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
                G200Log(LOG_PUI_MOD, LOGLVL_DEBUG1, "%s[%s]:收到Pui消息[%s]\n", m_alias, GetStrState(), OspEventDesc(pcMsg->event));
            }
            break;
        }
    }
}
