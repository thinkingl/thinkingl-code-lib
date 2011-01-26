#include "srvmain.h"
#include "devreg.h"
#include "kdvsys.h"
#include "httpcommon.h"
#include "codingtransform.h"
#include "aaasdk.h"

const u32 g_dwMaxMsgLen = MON_OSP_MSG_BODY_MAX_LEN;

extern TG200CfgBase   g_tCfg;
extern SEMHANDLE      g_hMainSem;

BOOL32 CDevMgrData::IsHasInit3ALib()
{
    return m_bHasInit3ALib;
}


void CDevMgrData::SetInit3ALib(BOOL32 bStatus)
{
    m_bHasInit3ALib = bStatus;
}

CDevMgrData::CDevMgrData()
{
	m_bG200OnLine = FALSE;
	m_bInit3ASdk = FALSE;
    m_bLogin = FALSE;
    m_bHasInit3ALib = FALSE;
    m_bFirst = TRUE;
	memset(&m_tLincenseInfo,0,sizeof(TLicenseInfo));
}

BOOL32 CDevMgrData::LoadVideoInRtCfg()
{
	s8 szBindRtFileName[MAXLEN_FILEPATH+64] = {0};
	
	TVideoInItem tInItem;
	TBasePu tBasePu;

	s8 achKeyBuf[128 + 1];
    s8 achValueBuf[128 + 1];
    s8 achCameraName[MAXLEN_ALIAS + 1];
	s32 nRegisterStatus;
	s32 nMatrixId;
	s32 nVideoPort;
	s32 nType;
	s32 nCamAddr;
	
	sprintf(szBindRtFileName, "%s%s", g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
	
    BOOL32 bRet = GetNextLineString(szBindRtFileName,SECTION_CAMERADEVICE,achKeyBuf,sizeof(achKeyBuf),
    achValueBuf,sizeof(achValueBuf));
	while (bRet)
	{
        sscanf(achValueBuf,"%s%d%d%d%d%d",achCameraName,&nRegisterStatus,&nMatrixId,&nVideoPort,
			&nType,&nCamAddr);
        if (nRegisterStatus == 0)
        {
            G200MSLog(LOG_DEVMGR_MOD,LOGLVL_DEBUG2,"[G200MS/DevReg]:%s=%s\n",achKeyBuf,achValueBuf);
            //��ȡδע����豸
            tBasePu.SetNOString(achKeyBuf);
			tBasePu.SetObjName(achCameraName);
            tInItem.SetCam(tBasePu);
			
			tInItem.SetMatrixID(nMatrixId);
			tInItem.SetRegisterStatus(nRegisterStatus);
			tInItem.SetVideoInPort(nVideoPort);
			tInItem.SetCamAddr(nCamAddr);
			tInItem.SetCamType(nType);

            AddNeedRegDev(tInItem);
        }
        bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
    achValueBuf,sizeof(achValueBuf));
	}
	return TRUE;
}

BOOL32 CDevMgrData::GetLoginStatus()
{
    return m_bLogin;
}

void CDevMgrData::SetLoginStatus(BOOL32 bStatus)
{
    m_bLogin = bStatus;
}

BOOL32 CDevMgrData::IsFirstConn3A()
{
    return m_bFirst;
}

void CDevMgrData::SetFirstConn3AFlag(BOOL32 bFlag)
{
    m_bFirst = bFlag;
}

void CDevMgrData::SetLastError(s32 dwError)
{
	m_dwError = dwError;
}

s32 CDevMgrData::GetLastError()
{
	return m_dwError;
}

void CDevMgrData::SetLicenseInfo(TLicenseInfo& tLicenseInfo)
{
	m_tLincenseInfo = tLicenseInfo;
}

TLicenseInfo CDevMgrData::GetLicenseInfo() const
{
	return m_tLincenseInfo;
}
/*=======================================================
������    :  GetNicInfo
��  ��    :  ��ȡָ��ip��������Ϣ
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
NetConfigure CDevMgrData::GetNicInfo(const s8* pszIp)
{
	NetConfigure tNetConfigure;
#ifdef _LINUX_
	u8 byNicNum = m_cNetManager.GetNicNum();
    s8 achTempNicDevName[MAXLEN_ALIAS + 1];
	
    for (int nIndex = 0; (u8)nIndex < byNicNum; nIndex++)
    {
        sprintf(achTempNicDevName,"%s%d","eth",nIndex);
        tNetConfigure = m_cNetManager.GetNicInfo(achTempNicDevName);
        if (!strcmp(pszIp,tNetConfigure.GetNicIp()))
        {
            return tNetConfigure;
        }
    }
#endif
    //û���ҵ�ָ��ip������
    memset(&tNetConfigure, 0, sizeof(NetConfigure));
    return tNetConfigure;
}

/*=======================================================
������    :  SetNicInfo
��  ��    :  ����ָ��ip��������Ϣ
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
BOOL32 CDevMgrData::SetNicInfo(const s8* pszOldIp,const s8 *pszIp, const s8 *achMask, const s8 *achGateWay)
{
#ifdef _LINUX_
	NetConfigure tNetConfigure;
	u8 byNicNum = m_cNetManager.GetNicNum();
    s8 achTempNicDevName[MAXLEN_ALIAS + 1];
    for (int nIndex = 0; (u8)nIndex < byNicNum; nIndex++)
    {
        sprintf(achTempNicDevName,"%s%d","eth",nIndex);
        tNetConfigure = m_cNetManager.GetNicInfo(achTempNicDevName);
        if (!strcmp(pszOldIp,tNetConfigure.GetNicIp()))
        {
            return m_cNetManager.SetNicInfo(achTempNicDevName,pszIp,achMask,achGateWay);
        }
    }
#endif
    return FALSE;
}


TNeedRegDevList *CDevMgrData::GetNeedRegDevListPtr()
{
	return &m_tNeedRegDevList;
}

void CDevMgrData::AddNeedRegDev(const TVideoInItem& tItem)
{
	TVideoInItem* ptDev = m_tNeedRegDevList.AddItem();
	
	if ( NULL != ptDev )
	{
		*ptDev = tItem;
	}
}

THasRegDevList* CDevMgrData::GetHasRegDevListPtr()
{
	return &m_tHasRegDevList;
}

void CDevMgrData::AddHasRegDev(const TBasePu& tVideoInItem)
{
	TBasePu* ptDev = m_tHasRegDevList.AddItem();
	
	if ( NULL != ptDev )
	{
		*ptDev = tVideoInItem;
	}
}

BOOL32 CDevMgrData::bInit3ASdk()
{
	return m_bInit3ASdk;
}

void CDevMgrData::SetInit3ASdk(BOOL32 bInit)
{
	m_bInit3ASdk = bInit;
}

BOOL32 CDevMgrData::bG200OnLine()
{
	return m_bG200OnLine;
}

void CDevMgrData::SetG200OnLine(BOOL32 bOnline)
{
	m_bG200OnLine = bOnline;
}

CDevMgrIns::CDevMgrIns()
{
	m_dwNodeId = INVALID_NODE;
    m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);
}

LPCSTR CDevMgrIns::GetStrState()
{
	switch(CurState())
    {
    case STATE_IDLE:
        return "Idle";
    case STATE_SERVICE:
        return "Service";
    default:
        return "δ֪״̬";
    }
}


void CDevMgrIns::SendMsgToG200(CMonMsg& cMonMsg, u16 wEvent)
{
	G200MSLog(LOG_DEVMGR_MOD,LOGLVL_DEBUG2,"[G200ms/DevMgr-Daemon][%s]:Send Message[%s-%d] to G200\n",
		GetStrState(),OspEventDesc(wEvent),wEvent);
	post(m_dwDstIId, wEvent, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen(), m_dwNodeId);
}

void CDevMgrIns::SendMsgToG200(CMessage *const pcMsg,u16 wEvent)
{
	G200MSLog(LOG_DEVMGR_MOD,LOGLVL_DEBUG2,"[G200ms/DevMgr-Daemon][%s]:Send Message[%s-%d] to G200\n",
		GetStrState(),OspEventDesc(wEvent),wEvent);
	post(m_dwDstIId,pcMsg->event,pcMsg->content,pcMsg->length,m_dwNodeId);
}

void CDevMgrIns::StartDevReg()
{
	TNeedRegDevList *ptDevList = g_cDevMgrApp.GetNeedRegDevListPtr();
	if (ptDevList->GetItemCount() == 0)
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevReg] : û�м�⵽��Ҫע����豸��\n");
		return;
	}
	
	TVideoInItem *ptDev = ptDevList->PosFirst();
    BOOL32 bRet = FALSE;
	while(NULL != ptDev)
	{
		    bRet = RegisteCameraTo3A(ptDev->GetCam().GetNOString(),ptDev->GetCam().GetObjName());
			if (bRet)
			{
                //�޸������ļ��еļ�¼Ϊ�Ѿ�ע��
                ptDev->SetRegisterStatus(TRUE);
			    UpdateVideoInCfg(*ptDev);
			    
			    //��ӵ�m_tHasRegDevList�б���
                TBasePu tBasePu = ptDev->GetCam();
			    g_cDevMgrApp.AddHasRegDev(tBasePu);
			}
            //��m_tNeedRegDevList��ɾ����ǰע����豸����
			ptDevList->DelItem(ptDev);
			ptDev = ptDevList->PosCur();
	}

	if (g_cDevMgrApp.bG200OnLine())
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevReg] : ������ע��ɹ����豸��Ϣ�㲥��G200!\n");		
		NotifyHasRegDevInfo();
	}
}

void CDevMgrIns::ProcG200Reg(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;

	if (g_cDevMgrApp.bG200OnLine())
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevReg] : G200�Ѿ����ߣ�\n");
		post(pcMsg->srcid, pcMsg->event+2, pcMsg->content, pcMsg->length, pcMsg->srcnode);
		return;
	}
	TLicenseInfo* ptLicenseInfo = (TLicenseInfo*)pcMonMsg->GetMsgBody();
	g_cDevMgrApp.SetLicenseInfo(*ptLicenseInfo);

	m_dwDstIId = pcMsg->srcid;
	m_dwNodeId = pcMsg->srcnode;

	//ע����������
	OspNodeDiscCBReg(m_dwNodeId, m_appId, m_instId);
	
	//�ظ�ע��ɹ���Ϣ
	SendMsgToG200(*pcMonMsg, pcMsg->event+1);

	g_cDevMgrApp.SetG200OnLine(TRUE);

	//��ע��ɹ����豸��Ϣͬ����G200
	THasRegDevList* ptList = g_cDevMgrApp.GetHasRegDevListPtr();
	if ((NULL != ptList) && (ptList->GetItemCount() > 0))
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevReg] : G200���ߣ���ע��ɹ����豸��Ϣͬ����G200��\n");
		NotifyHasRegDevInfo();
	}

    NextState(STATE_SERVICE);
}

void CDevMgrIns::UpdateVideoInCfg(TVideoInItem& tItem)
{
	s8 achFileName[MAXLEN_FILEPATH+64] = {0};
	s8 szVideoInInfo[128] = {0};

	sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
	sprintf(szVideoInInfo,"%s %d %d %d %d %d",tItem.GetCam().GetObjName(),tItem.GetRegisterStatus(),tItem.GetMatrixID(),tItem.GetVideoInPort(),
		tItem.GetCamType(),tItem.GetCamAddr());

    SetRegKeyString(achFileName,SECTION_CAMERADEVICE,tItem.GetCam().GetNOString(),szVideoInInfo);
}

void CDevMgrIns::NotifyHasRegDevInfo()
{
	THasRegDevList* ptHasRegDevList = g_cDevMgrApp.GetHasRegDevListPtr();
	u32 dwNum = ptHasRegDevList->GetItemCount();
	if (0 == dwNum)
	{
		return;
	}

	TTotalNum tTotalNum;
	CMonMsg cMonMsg;
	cMonMsg.SetMsgBody(&tTotalNum, sizeof(tTotalNum));

	u32 dwToTalNum =  (g_dwMaxMsgLen - sizeof(TTotalNum)) / sizeof(TBasePu);
	u32 dwRealSendNum = 0;
	TBasePu *ptItem = ptHasRegDevList->PosFirst();
	for(; NULL != ptItem; ptItem = ptHasRegDevList->PosNext(ptItem))
	{
		if (dwRealSendNum < dwToTalNum)
		{
			cMonMsg.CatMsgBody(ptItem, sizeof(TBasePu));
			dwRealSendNum++;
		}
		else
		{
			TTotalNum *ptTotalNum = (TTotalNum *)cMonMsg.GetMsgBody();
			ptTotalNum->SetTotalNum(dwRealSendNum);
			SendMsgToG200(cMonMsg, G200MS_G200_ADD_BINDINRT_NTY);

			dwRealSendNum = 1;
			tTotalNum.SetTotalNum(dwRealSendNum);
			cMonMsg.SetMsgBody(&tTotalNum, sizeof(TTotalNum));
			cMonMsg.CatMsgBody(ptItem, sizeof(TBasePu));
		}
	}

	if (dwRealSendNum > 0)
	{
		TTotalNum *ptTotalNum = (TTotalNum *)cMonMsg.GetMsgBody();
		ptTotalNum->SetTotalNum(dwRealSendNum);

		SendMsgToG200(cMonMsg, G200MS_G200_ADD_BINDINRT_NTY);
	}

	ptHasRegDevList->Clear();
}

void CDevMgrIns::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
	G200MSLog(LOG_DEVMGR_MOD,LOGLVL_DEBUG2,"[G200ms/DevMgr-Daemon][%s]:Receive Message[%s-%d]\n",
        GetStrState(),OspEventDesc(pcMsg->event),pcMsg->event);

    if (OnCommonMsg(pcMsg))
	{
		return;
	}

	switch(CurState())
    {
    case STATE_IDLE:
        {
            OnIdle(pcMsg,pcApp);
        }
        break;
    case STATE_SERVICE:
        {
            OnService(pcMsg,pcApp);
        }
        break;
    default:
        {
        }
        break;
    }
}

void CDevMgrIns::OnIdle(CMessage *const pcMsg,CApp* pcApp)
{
	switch(pcMsg->event)
	{
	case EV_G200MS_DEVREG_POWERON:
		{
            post(MAKEIID(AID_G200MS_SDK3A,CInstance::DAEMON),EV_G200MS_INIT_3ASDK_TIMER);
		}
		break;
    case G200_G200MS_REG_REQ:
        {
            ProcG200Reg(pcMsg);
        }
		break;
    case OSP_DISCONNECT:
        {
            m_dwNodeId = INVALID_NODE;
			m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);
            g_cDevMgrApp.SetG200OnLine(FALSE);
        }
        break;
	default:
		{
            G200MSLog(LOG_CLIENTCONN_MOD,LOGLVL_DEBUG2,"[G200ms/DevMgr-Daemon][%s]:Can't Deal With Message[%s-%d]\n",
                GetStrState(),OspEventDesc(pcMsg->event),pcMsg->event);
		}
		break;
	}
}

void CDevMgrIns::OnService(CMessage *const pcMsg,CApp* pcApp)
{
	switch(pcMsg->event)
	{
    case EV_HS_SERVER_KEDACOMXMLDATA:
        {
            if (GETAPP(pcMsg->srcid) == AID_G200MS_CLIENTCONN && pcMsg->srcnode == 0)
            {
                //��Ϣ���Կͻ��˵���������
                OnDealData(pcMsg,pcApp);
            }
            else
            {
                //��Ϣ����g200������Ӧ,
                SendMsgToClientConn(pcMsg);
            }         
        }
        break;
    case EV_G200MS_CLIENTCONN_TIMER:
        ProcessClientConnTimeOut(pcMsg,pcApp);
        break;
    case OSP_DISCONNECT:
		{
			m_dwNodeId = INVALID_NODE;
			m_dwDstIId = MAKEIID(INVALID_APP, INVALID_INS);

			g_cDevMgrApp.SetG200OnLine(FALSE);
			//���û��˳�״̬
			g_cDevMgrApp.SetLoginStatus(FALSE);
            NextState(STATE_IDLE);
		}
		break;
	default:
		{
            G200MSLog(LOG_CLIENTCONN_MOD,LOGLVL_DEBUG2,"[G200ms/DevMgr-Daemon][%s]:Can't Deal With this Message[%s-%d]\n",
                GetStrState(),OspEventDesc(pcMsg->event),pcMsg->event);
		}
		break;
	}
}

BOOL32 CDevMgrIns::OnCommonMsg(CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
    case EV_G200MS_LOAD_CFG_TIMER:
	    {
		    g_cDevMgrApp.LoadVideoInRtCfg();
		    StartDevReg();
	    }
	    break;
	case EV_G200MS_PRINT_G200MS_STATUS_REQ:
		{
			OspPrintf(TRUE,FALSE,"*****************��ǰƽ̨״̬*******************\n");
			OspPrintf(TRUE,FALSE,"����3A:%s\n",GetStrBool(g_cDevMgrApp.bInit3ASdk()));
			OspPrintf(TRUE,FALSE,"G200����:%s\n",GetStrBool(g_cDevMgrApp.bG200OnLine()));
			OspPrintf(TRUE,FALSE,"�ͻ�������:%s\n",GetStrBool(g_cDevMgrApp.GetLoginStatus()));
		}
		break;
	case EV_G200MS_PRINT_NEED_REG_DEV_REQ:
		{
			TNeedRegDevList* ptNeedRegDevList = g_cDevMgrApp.GetNeedRegDevListPtr();
			u32 dwRegDevNum = 0;

			u32 *pdwMatrixID = (u32 *)pcMsg->content;
			if (*pdwMatrixID == 0)
			{
				//��ӡ������Ҫע����豸
				dwRegDevNum = ptNeedRegDevList->GetItemCount();
				OspPrintf(TRUE, FALSE, "\n������Ҫע����豸����Ϊ��%d\n", dwRegDevNum);
				OspPrintf(TRUE, FALSE, "\t��������˿ں�\t�豸���\t�豸����\n");
				OspPrintf(TRUE, FALSE, "------------------------------------------\n");

				TVideoInItem *ptItem = ptNeedRegDevList->PosFirst();
				for(; NULL != ptItem; ptItem = ptNeedRegDevList->PosNext(ptItem))
				{
					OspPrintf(TRUE, FALSE, "\t\t%d\t\t%s\t%s\n", ptItem->GetVideoInPort(), ptItem->GetCam().GetNOString(), ptItem->GetCam().GetObjName());
				}
			}
			else
			{
				//��ӡĳ����������Ҫע����豸
				u32 *pdwMatrixID = (u32 *)pcMsg->content;

				OspPrintf(TRUE, FALSE, "\n����[%d]����Ҫע����豸��Ϣ��\n", *pdwMatrixID);
				OspPrintf(TRUE, FALSE, "\t��������˿ں�\t�豸���\t�豸����\n");
				OspPrintf(TRUE, FALSE, "------------------------------------------\n");

				TVideoInItem *ptItem = ptNeedRegDevList->PosFirst();
				for(; NULL != ptItem; ptItem = ptNeedRegDevList->PosNext(ptItem))
				{
					if (ptItem->GetMatrixID() == *pdwMatrixID)
					{
						OspPrintf(TRUE, FALSE, "\t\t%d\t\t%s\t%s\n", ptItem->GetVideoInPort(), ptItem->GetCam().GetNOString(),
                            ptItem->GetCam().GetObjName());
					}
				}
			}
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	return TRUE;
}
/*=======================================================
������    :  SendMsgToClientConn
��  ��    :  ��Ӧ����Ϣ����ClientConnģ��
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::SendMsgToClientConn(CXmlMsg &cXmlMsg,CMessage *const pcMsg)
{
    s32 nLen = 0;
    const s8* pszXmlDoc =  cXmlMsg.GetMsgStr(nLen);
    if (pszXmlDoc == NULL)
    {
        G200MSLog(LOG_DEVMGR_MOD,LOGLVL_EXCEPTION,"[G200ms/DevMgr-Daemon][%s]: Get Xml String Failure[%s-%d]\n",
            GetStrState(),OspEventDesc(cXmlMsg.GetCommand()),cXmlMsg.GetCommand());
        return;
    }
    CMonMsg* pcMonMsg = (CMonMsg*)pcMsg->content;
    
    CMonMsg cMonMsg;
    cMonMsg.SetSessID(pcMonMsg->GetSessID());
    cMonMsg.SetReserved1(pcMonMsg->GetReserved1());
    cMonMsg.SetMsgBody(pszXmlDoc,nLen);
    
	G200MSLog(LOG_DEVMGR_MOD,LOGLVL_DEBUG2,"[200ms/DevMgr-Daemon][%s]:Send Message[%s-%d] to ClientConn\n",
		GetStrState(),OspEventDesc(cXmlMsg.GetCommand()),cXmlMsg.GetCommand());
    post(MAKEIID(AID_G200MS_CLIENTCONN,CInstance::DAEMON),pcMsg->event,cMonMsg.GetMonMsg(),
                 cMonMsg.GetMonMsgLen());
}

/*=======================================================
������    :  SendMsgToClientConn
��  ��    :  ������g200��Ӧ����Ϣת����ClientConnģ��
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::SendMsgToClientConn(CMessage *const pcMsg)
{
    post(MAKEIID(AID_G200MS_CLIENTCONN,CInstance::DAEMON),pcMsg->event,pcMsg->content,pcMsg->length);
}

void CDevMgrIns::OnDealData(CMessage* const pcMsg, CApp* pcApp)
{
    //����Ϣ����ע��ģ�鴦��
    CMonMsg *pcMonMsg = (CMonMsg*)pcMsg->content;
    
    s8 *pBuf = (s8 *)pcMonMsg->GetMsgBody();
    
    //s8* pBuf = (s8*)pcMsg->content;
    s8 * end = strstr((s8*)pBuf , "/kedacomxmldata");
	*(end + 16) = 0;

	if (NULL == pBuf)
	{
        return;
	}
    s32 nLen = CodingTransformer::UTF_8ToGB2312(m_achXmlCmd,XML_CMDSTR_LEN,pBuf,strlen(pBuf) + 1);

#ifdef _LINUX_
    if (nLen == -1)
    {
        return;
    }
#else
    if (nLen == 0)
    {
        return;
    }
#endif 
    m_achXmlCmd[nLen] = '\0';

    CXmlMsg cXmlMsg(m_achXmlCmd);
    G200MSLog(LOG_DEVMGR_MOD,LOGLVL_DEBUG2,"[G200ms/DevMgr-Daemon][%s]:Receive Message[%s-%d]\n",GetStrState(),OspEventDesc(cXmlMsg.GetCommand()),
		cXmlMsg.GetCommand());
    switch (cXmlMsg.GetCommand())
	{
		case BS_G200MS_USER_LOGIN_REQ:
			ProcessLogin(cXmlMsg,pcMsg,pcApp);
			break;
        case BS_G200MS_SYSQUIT_REQ:
            ProcessLogOut(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_HEARTBEAT_NTF:
            ProcessHeartBeatFromClient(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_ADD_MATRIX_REQ:
            ProcessAddMatrix(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_DELETE_MATRIX_REQ:
            ProcessDeleteMatrix(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_UPDATE_MATRIX_REQ:
            ProcessUpdateMatrix(cXmlMsg,pcMsg,pcApp);
            break;

        case BS_G200MS_ADD_ENCODER_REQ:
            ProcessAddEncoder(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_DELETE_ENCODER_REQ:
            ProcessDeleteEncoder(cXmlMsg,pcMsg,pcApp);
            break;
       case BS_G200MS_UPDATE_ENCODER_REQ:
           ProcessUpdateEncoder(cXmlMsg,pcMsg,pcApp);
            break;

        case BS_G200MS_ADD_CAMERA_REQ:
            ProcessAddCamera(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_DELETE_CAMERA_REQ:
            ProcessDeleteCamera(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_UPDATE_CAMERA_REQ:
            ProcessUpdateCamera(cXmlMsg,pcMsg,pcApp);
            break;

        case BS_G200MS_UPDATE_USER_REQ:
            ProcessUpdateUser(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_ADDLINK_REQ:
            ProcessAddLink(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_DELLINK_REQ:
            ProcessDelLink(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_MATRIXSINFO_REQ:
            ProcessGetMatrixList(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_DEVICESLIST_REQ:
            ProcessGetDeviceList(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_SYSCONF_REQ:
            ProcessGetSysConf(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_SYSTEMREBOOT_REQ:
            ProcessSystemReboot(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_SYSPARASET_REQ:
            ProcessSetSysPara(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_SYSIPSET_REQ:
            ProcessSetSysIp(cXmlMsg,pcMsg,pcApp);
            break;
        case BS_G200MS_G200_SYSMAIN_REQ:
		case BS_G200MS_G200_MPLATSTATUS_REQ:     //��ȡ���ӵ��ϼ�ƽ̨״̬
		case BS_G200MS_G200_DEVICESTATUS_REQ:
			SendMsgToG200(pcMsg,cXmlMsg.GetCommand());
            break;		
		case BS_G200MS_EXPORTCONFIG_REQ:
			ProcessG200ConfigTar(cXmlMsg,pcMsg,pcApp);
			break;
		case BS_G200MS_3ASTATUS_REQ:
			ProcessGet3AStatus(cXmlMsg,pcMsg,pcApp);
			break;
		case BS_G200MS_CAMERALIST_REQ:
			ProcessGetCameraList(cXmlMsg,pcMsg,pcApp);
			break;
		case BS_G200MS_ENCODERLIST_REQ:
			ProcessGetEncoderList(cXmlMsg,pcMsg,pcApp);
			break;
		default:
            G200MSLog(LOG_CLIENTCONN_MOD,LOGLVL_DEBUG2,"[G200ms/DevMgr-Daemon][%s]:Can't Deal With this Message[%s-%d]\n",
                GetStrState(),OspEventDesc(cXmlMsg.GetCommand()),cXmlMsg.GetCommand());
			break;
	}
}

/*=======================================================
������    :  ProcessGet3AStatus
��  ��    :  ��ȡ����3A��״̬
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/5/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessGet3AStatus(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
	CXmlMsg cRetMsg;
	cRetMsg.SetCommand(G200MS_BS_3ASTATUS_ACK);
	cRetMsg.Go2Content();
	cRetMsg.BuildNode("status3a","%d",g_cDevMgrApp.bInit3ASdk());
	SendMsgToClientConn(cRetMsg,pcMsg);
}

void CDevMgrIns::ProcessLogOut(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    KillTimer(EV_G200MS_CLIENTCONN_TIMER);
    g_cDevMgrApp.SetLoginStatus(FALSE);
    CXmlMsg cRetMsg;
    cRetMsg.SetCommand(G200MS_BS_SYSQUIT_ACK);
    SendMsgToClientConn(cRetMsg,pcMsg);
}

void CDevMgrIns::ProcessSetSysPara(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    CXmlMsg cRetMsg;
    BOOL32 bRet = UpdateSysParaToFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_SYSPARASET_ACK);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_SYSPARASET_NACK);
    }
    SendMsgToClientConn(cRetMsg,pcMsg);

    //֪ͨϵͳ����

}

BOOL32 CDevMgrIns::UpdateSysParaToFile(CXmlMsg& cXmlMsg)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();
    s8 achIp[MAX_IP_ADDR] = {0};
	s8 ach3aIp[MAX_IP_ADDR] = {0};
    s8 achKdmNo[LEN_KDM_NO + 1] = {0};
	s8 ach3AUser[MAXLEN_ALIAS + 1] = {0};
	s8 ach3APsw[MAXLEN_ALIAS + 1] = {0};

    s32 nPort;
	s32 n3APort;
	s32 nMPlatType;

    const s8* pszIp = cXmlMsg.GetNodeStringValue("mplatip");
    if (pszIp != NULL)
    {
        strncpy(achIp,pszIp,MAX_IP_ADDR -1);
        achIp[MAX_IP_ADDR -1] = 0;
    }
    
	const s8* psz3aIp = cXmlMsg.GetNodeStringValue("ip3a");
	if (psz3aIp != NULL)
	{
		strncpy(ach3aIp,psz3aIp,MAX_IP_ADDR -1);
		ach3aIp[MAX_IP_ADDR - 1] = 0;
	}
	
	const s8* pszUser3A = cXmlMsg.GetNodeStringValue("user3a");
	if (NULL != pszUser3A)
	{
		strncpy(ach3AUser,cXmlMsg.GetNodeStringValue("user3a"),MAXLEN_ALIAS);
		ach3AUser[MAXLEN_ALIAS] = 0;
	}

	const s8* pszPsw3A = cXmlMsg.GetNodeStringValue("psw3a");
	if (NULL != pszPsw3A)
	{
		strncpy(ach3APsw,cXmlMsg.GetNodeStringValue("psw3a"),MAXLEN_ALIAS);
		ach3APsw[MAXLEN_ALIAS] = 0;
	}

	n3APort = cXmlMsg.GetNodeIntValue("port3a");

    nPort = cXmlMsg.GetNodeIntValue("mplatport");

	nMPlatType = cXmlMsg.GetNodeIntValue("mplattype");
    
    const s8* pszKdmNo = cXmlMsg.GetNodeStringValue("no");
    if (pszKdmNo != NULL)
    {
         strncpy(achKdmNo,pszKdmNo,LEN_KDM_NO);
         achKdmNo[LEN_KDM_NO] = 0;
    }

	u32 dwMplatIp,dw3aIp;
	dwMplatIp = inet_addr(achIp);
	dw3aIp = inet_addr(ach3aIp);
	if (dwMplatIp == -1 || dw3aIp == -1)
	{
		CHECK_WRITE_DB_RET(FALSE,"Receive Wrong Mplat or 3A Ip Addr!\n");
	}
	

    BOOL32 bRet;
    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);

	if (dwMplatIp != g_tCfg.GetMPlatIP())
	{
		bRet = SetRegKeyString(achFileName,SECTION_MPLAT,KEY_MPLATIP,achIp);
		CHECK_WRITE_DB_RET(bRet,"Update DB File MPlatIp:Failure!\n");
		g_tCfg.SetMPlatIP(achIp);//����ƽ̨ip
	}
    
	if (nPort != g_tCfg.GetMPlatPort())
	{
		bRet = SetRegKeyInt(achFileName,SECTION_MPLAT,KEY_MPLATPORT,nPort);
		CHECK_WRITE_DB_RET(bRet,"Update DB File MPlatPort:Failure!\n");
		g_tCfg.SetMPlatPort(nPort);
	}
    
	if (dw3aIp != g_tCfg.Get3AIP())
	{
		bRet = SetRegKeyString(achFileName,SECTION_MPLAT,KEY_3AIP,ach3aIp);
		CHECK_WRITE_DB_RET(bRet,"Update DB File 3AIp:Failure!\n");
		g_tCfg.Set3AIP(ach3aIp);//����3Aip
	}

	if (n3APort != g_tCfg.Get3APort())
	{
		bRet = SetRegKeyInt(achFileName,SECTION_MPLAT,KEY_3APORT,n3APort);
		CHECK_WRITE_DB_RET(bRet,"Update DB File 3aPort:Failure!\n");
		g_tCfg.Set3APort(n3APort);
	}
	
	if (strcmp(g_tCfg.GetBaseCmu().GetNOString(),achKdmNo))
	{
		bRet = SetRegKeyString(achFileName,SECTION_SELFPLAT,KEY_SELFPLAT_NO,achKdmNo);
		CHECK_WRITE_DB_RET(bRet,"Update DB File SPLAT_NO:Failure!\n");
		g_tCfg.SetG200Id(achKdmNo);

		//�޸�pui�ı��
		s8 achPuiid[LEN_KDM_NO + 1] = {0};
		GetRegKeyString(achFileName,SECTION_PUI,KEY_PUIID,"55010000000000000027001000000000",achPuiid,sizeof(achPuiid));
		strncpy(achKdmNo + 18, achPuiid + 18,14);
		bRet = SetRegKeyString(achFileName, SECTION_PUI,KEY_PUIID,achKdmNo);
		CHECK_WRITE_DB_RET(bRet,"Update DB File PUIID:Failure!\n");
	}
    
	if (nMPlatType != g_tCfg.GetMplatType())
	{
		bRet = SetRegKeyInt(achFileName,SECTION_MPLAT,KEY_MPLATTYPE,nMPlatType);
		CHECK_WRITE_DB_RET(bRet,"Update DB File MPLATTYPE:Failure!\n");
		g_tCfg.SetMplatType(nMPlatType);

		//ִ�нű��ļ�������֧�ֲ�ͬƽ̨���͵Ŀ�ִ���ļ�
#ifdef _LINUX_
		s8 szSysCmd[512];
		sprintf(szSysCmd,"/opt/kdm/g200/script/mplattype.sh %d",nMPlatType);
		u32 dwRet = OspSystem(szSysCmd);
		dwRet = WIFEXITED(dwRet)? WEXITSTATUS(dwRet):-1;
		if (0 != dwRet)
		{
			G200MSLog(LOG_DEVMGR_MOD,LOGLVL_DEBUG2,"ִ���滻ƽ̨���ͣ�%d�Ľű�ʧ��!",nMPlatType);
			return FALSE;
		}
#else
		return FALSE;
#endif
	}

	bRet = SetRegKeyString(achFileName,SECTION_MPLAT,KEY_3AUSER,ach3AUser);
	CHECK_WRITE_DB_RET(bRet,"Update DB File KEY_3AUSER:Failure!\n");

	bRet = SetRegKeyString(achFileName,SECTION_MPLAT,KEY_3APSW,ach3APsw);
	CHECK_WRITE_DB_RET(bRet,"Update DB File KEY_3APSW:Failure!\n");
    return TRUE;
}

void CDevMgrIns::ProcessG200ConfigTar(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
	CXmlMsg cRetMsg;
#ifdef _LINUX_
	s8 szSysCmd[512];
    sprintf(szSysCmd,"/opt/kdm/g200/script/export.sh");
    u32 dwRet = OspSystem(szSysCmd);
	if (dwRet == 0)
	{
		cRetMsg.SetCommand(G200MS_BS_EXPORTCONFIG_ACK);
	}
	else
	{
		cRetMsg.SetCommand(G200MS_BS_EXPORTCONFIG_NACK);
	}
#else
	cRetMsg.SetCommand(G200MS_BS_EXPORTCONFIG_NACK);
#endif
	SendMsgToClientConn(cRetMsg,pcMsg);
}
void CDevMgrIns::ProcessSetSysIp(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    CXmlMsg cRetMsg;
    BOOL32 bRet = UpdateSysIpToFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_SYSIPSET_ACK);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_SYSIPSET_NACK);
    }
    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::UpdateSysIpToFile(CXmlMsg& cXmlMsg)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    s8 achHostIp[20] ={0};
    s8 achIp[20] = {0};
    s8 achSubMask[20] = {0};
    s8 achDefaultGate[20] = {0};

    const s8* pszHostIp = cXmlMsg.GetNodeStringValue("hostip");
    if (pszHostIp != NULL)
    {
        strncpy(achHostIp,pszHostIp,19);
        achHostIp[19] = 0;
    }
    
    const s8* pszIp = cXmlMsg.GetNodeStringValue("selip");
    if (pszIp != NULL)
    {
        strncpy(achIp,pszIp,19);
        achIp[19] = 0;
    }
    
    const s8* pszSubMask = cXmlMsg.GetNodeStringValue("submask");
    if (pszSubMask != NULL)
    {
        strncpy(achSubMask,pszSubMask,19);
        achSubMask[19] = 0;
    }
    
    const s8* pszDefaultMask = cXmlMsg.GetNodeStringValue("defaultgate");
    if (pszDefaultMask != NULL)
    {
        strncpy(achDefaultGate,pszDefaultMask,19);
        achDefaultGate[19] = 0;
    }
    return g_cDevMgrApp.SetNicInfo(achHostIp,achIp,achSubMask,achDefaultGate);
}
void CDevMgrIns::ProcessClientConnTimeOut(CMessage *const pcMsg,CApp* pcApp)
{
    //�û�������ʱ
    g_cDevMgrApp.SetLoginStatus(FALSE);
}


void CDevMgrIns::ProcessHeartBeatFromClient(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    //�յ��ͻ��˵�������ȡ����һ�����õ�������ʱ�¼�,���¼�ʱ
    KillTimer(EV_G200MS_CLIENTCONN_TIMER);
	CXmlMsg cRetXml;
	cRetXml.SetCommand(G200MS_BS_HEARTBEAT_NTF);
	SendMsgToClientConn(cRetXml,pcMsg);
    SetTimer(EV_G200MS_CLIENTCONN_TIMER,G200MS_CLIENTCONN_INTV);
}

void CDevMgrIns::ProcessSystemReboot(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    CXmlMsg cRetXml;
    cRetXml.SetCommand(G200MS_BS_SYSTEMREBOOT_ACK);
    SendMsgToClientConn(cRetXml,pcMsg);
	OspDelay(10*1000);
#ifdef _LINUX_
    s8 szSysCmd[512];
    sprintf(szSysCmd,"reboot -f >/dev/null 2>&1");
    OspSystem(szSysCmd);
#else
    //ϵͳ����
    OSVERSIONINFO osv;
    osv.dwOSVersionInfoSize = sizeof(osv);
    GetVersionEx(&osv);
    if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        HANDLE hProcess, hToken;
		TOKEN_PRIVILEGES Privileges;
		hProcess = GetCurrentProcess();
		OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken);
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &Privileges.Privileges[0].Luid);
		Privileges.PrivilegeCount = 1;
		Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &Privileges, 0, (PTOKEN_PRIVILEGES)NULL, NULL);
    }
    ExitWindowsEx(EWX_FORCE | EWX_REBOOT,0);
#endif
}


void CDevMgrIns::ProcessGetSysConf(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();
    
    const s8* pszHostIp = cXmlMsg.GetNodeStringValue("hostip");
    
    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
    s8 achIp[MAX_IP_ADDR];
    s32 nPort;
	s8 ach3AIp[MAX_IP_ADDR];
	s32 n3APort;
	s32 nMPlatType = 1;
	s8 ach3AUser[MAXLEN_ALIAS + 1] = {0};
	s8 ach3APsw[MAXLEN_ALIAS + 1] = {0};

	s8 achKdmNo[LEN_KDM_NO + 1];
    GetRegKeyString(achFileName,SECTION_MPLAT,KEY_MPLATIP,"127.0.0.1",achIp,sizeof(achIp));
    GetRegKeyInt(achFileName,SECTION_MPLAT,KEY_MPLATPORT,1722,&nPort);
    GetRegKeyString(achFileName,SECTION_MPLAT,KEY_3AIP,"127.0.0.1",ach3AIp,sizeof(ach3AIp));
	GetRegKeyInt(achFileName,SECTION_MPLAT,KEY_3APORT,30000,&n3APort);
	GetRegKeyInt(achFileName,SECTION_MPLAT,KEY_MPLATTYPE,1,&nMPlatType);
	GetRegKeyString(achFileName,SECTION_MPLAT,KEY_3AUSER,"admin@fj",ach3AUser,sizeof(ach3AUser));
	GetRegKeyString(achFileName,SECTION_MPLAT,KEY_3APSW,"admin",ach3APsw,sizeof(ach3APsw));

    GetRegKeyString(achFileName,SECTION_SELFPLAT,KEY_SELFPLAT_NO,"",achKdmNo,sizeof(achKdmNo));
	
    CXmlMsg cRetXml;
    cRetXml.SetCommand(G200MS_BS_SYSCONF_ACK);
    cRetXml.Go2Content();
    cRetXml.BuildNode("mplatip","%s",achIp);
    cRetXml.BuildNode("mplatport","%d",nPort);
	cRetXml.BuildNode("ip3a","%s",ach3AIp);
	cRetXml.BuildNode("port3a","%d",n3APort);
	cRetXml.BuildNode("mplattype","%d",nMPlatType);
    cRetXml.BuildNode("No","%s",achKdmNo);
	cRetXml.BuildNode("user3a","%s",ach3AUser);
	cRetXml.BuildNode("psw3a","%s",ach3APsw);

    //��ȡip��ַ
    NetConfigure tNetConfigure = g_cDevMgrApp.GetNicInfo(pszHostIp);
    cRetXml.BuildNode("selIp","%s",tNetConfigure.GetNicIp());
    cRetXml.BuildNode("subMask","%s",tNetConfigure.GetNicNetMask());
    cRetXml.BuildNode("defaultGate","%s",tNetConfigure.GetNicGateWay());
    SendMsgToClientConn(cRetXml,pcMsg);
}

/*=======================================================
������    :  ProcessGetMatrixList
��  ��    :  ��ȡ���еľ����豸
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessGetMatrixList(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);

    s8 szMatrixName[MAXLEN_ALIAS + 1] = {0};
    s32 nMatrixInportNum;
    s32 nMatrixOutportNum;
    s32 nMatrixType;
    s8 achKeyBuf[128 + 1] = {0};
    s8 achValueBuf[128 + 1] = {0};

    BOOL32 bRet = GetNextLineString(achFileName,SECTION_MATRIXDEVICE,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    CXmlMsg cRetMsg;
    cRetMsg.SetCommand(G200MS_BS_MATRIXSINFO_ACK);
    cRetMsg.Go2Content();
    while (bRet)
    {
        sscanf(achValueBuf,"%s%d%d%d",szMatrixName,&nMatrixType,&nMatrixInportNum,&nMatrixOutportNum);
        cRetMsg.BuildNode("matrix");
        cRetMsg.GoDownstairs();
        cRetMsg.BuildNode("index","%s",achKeyBuf);
        cRetMsg.BuildNode("name","%s",szMatrixName);
        cRetMsg.BuildNode("type","%d",nMatrixType);
        cRetMsg.BuildNode("inport","%d",nMatrixInportNum);
        cRetMsg.BuildNode("outport","%d",nMatrixOutportNum);
        cRetMsg.GoUpstairs(); 
        bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    }
    SendMsgToClientConn(cRetMsg,pcMsg);
}
/*=======================================================
������    :  ProcessGetDeviceList
��  ��    :  ��ȡ���е�����ͷ���������豸���Լ������������ӹ�ϵ
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
2010/7/27     V1.0      ����ƽ                  ȡ������
=========================================================*/
void CDevMgrIns::ProcessGetDeviceList(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);

    s8 achKeyBuf[128 + 1] = {0};
    s8 achValueBuf[128 + 1] = {0};
    s32 nMatrixId;

    s8 achCameraName[MAXLEN_ALIAS + 1];
	s32 nRegisterStatus;
	s32 nVideoPort;
	s32 nType;
	s32 nCamAddr;

    s8 szEncoderName[MAXLEN_NAME + 1] = {0};
    s32 nVideoSrcPortNum = 0;
	s32 nConnMatrix;

    //��ȡ����ͷ�б�
    CXmlMsg cRetMsg;
    cRetMsg.SetCommand(G200MS_BS_DEVICESLIST_ACK);
    cRetMsg.Go2Content();
    cRetMsg.BuildNode("cameralist");
    cRetMsg.GoDownstairs();//current node cameralist
    BOOL32 bRet = GetNextLineString(achFileName,SECTION_CAMERADEVICE,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    while (bRet)
    {
        sscanf(achValueBuf,"%s%d%d%d%d%d",achCameraName,&nRegisterStatus,&nMatrixId,&nVideoPort,
			&nType,&nCamAddr);
        cRetMsg.BuildNode("camera");
        cRetMsg.GoDownstairs();//current node camera
        cRetMsg.BuildNode("guid","%s",achKeyBuf);
        cRetMsg.BuildNode("name","%s",achCameraName);
        cRetMsg.BuildNode("status","%d",nRegisterStatus);
        cRetMsg.BuildNode("matrixid","%d",nMatrixId);
        cRetMsg.BuildNode("videoport","%d",nVideoPort);
        cRetMsg.BuildNode("type","%d",nType);
        cRetMsg.BuildNode("addr","%d",nCamAddr);
        cRetMsg.GoUpstairs();//current node cameralist
        bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
            achValueBuf,sizeof(achValueBuf));
    }
    cRetMsg.GoUpstairs();//current node content

    //��ȡ�������б�
    cRetMsg.BuildNode("encoderlist");
    cRetMsg.GoDownstairs();//current node encoderlist
    bRet = GetNextLineString(achFileName,SECTION_ENCODERDEVICE,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    while (bRet)
    {
        sscanf(achValueBuf,"%s%d%d%d",szEncoderName,&nVideoSrcPortNum,&nConnMatrix,&nMatrixId);
        cRetMsg.BuildNode("encoder");
        cRetMsg.GoDownstairs();//current node encoder
        cRetMsg.BuildNode("guid","%s",achKeyBuf);
        cRetMsg.BuildNode("name","%s",szEncoderName);
        cRetMsg.BuildNode("matrixid","%d",nMatrixId);
        cRetMsg.BuildNode("link","%d",nConnMatrix);
        cRetMsg.BuildNode("portnum","%d",nVideoSrcPortNum);

        //��ȡ���������ӹ�ϵ
        for (s32 nSrcIndex = 0; nSrcIndex < nVideoSrcPortNum; nSrcIndex++)
        {
            s8 achVideoSrc[5];
            s8 achMatrixPort[5];
            sprintf(achVideoSrc,"%d",nSrcIndex);
            bRet = GetRegKeyString(achFileName,achKeyBuf,achVideoSrc,NULL,achMatrixPort,sizeof(achMatrixPort));
            if (bRet)
            {
                cRetMsg.BuildNode("conn");
                cRetMsg.GoDownstairs();
                cRetMsg.BuildNode("encoderport","%s",achVideoSrc);
                cRetMsg.BuildNode("matrixport","%s",achMatrixPort);
                cRetMsg.GoUpstairs();
            }
        }

        cRetMsg.GoUpstairs();//current node encoderlist
        bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
            achValueBuf,sizeof(achValueBuf));
    }

    SendMsgToClientConn(cRetMsg,pcMsg);
}


//=============================================================================
//  �� �� ���� ProcessGetCameraList
//  ��    �ܣ� 
//  �㷨ʵ�֣� 
//  ȫ�ֱ����� 
//  ��    ���� CXmlMsg& cXmlMsg
//             CMessage *const pcMsg
//             CApp* pcApp
//  �� �� ֵ�� void 
//-----------------------------------------------------------------------------
//  �޸ļ�¼��
//  ��  ��	  �汾		�޸���		�߶���    �޸�����
// 2010-7-28  1.0	    ����ƽ                  ����
//=============================================================================
void CDevMgrIns::ProcessGetCameraList(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
	s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
	
    s8 achKeyBuf[128 + 1] = {0};
    s8 achValueBuf[128 + 1] = {0};
    s32 nMatrixId;
	
    s8 achCameraName[MAXLEN_ALIAS + 1];
	s32 nRegisterStatus;
	s32 nVideoPort;
	s32 nType;
	s32 nCamAddr;
	
	cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();
    
    s32 nMatrixWebIndex = cXmlMsg.GetNodeIntValue("index");
	s32 nStartIndex = cXmlMsg.GetNodeIntValue("start");

	//��ȡ����ͷ�б�
	s32 nTotalInserNum = 0;//��ǰ�ҵ�������ͷ��
	s32 nTravalNum = 0;//������������ͷ��

    CXmlMsg cRetMsg;
    cRetMsg.SetCommand(G200MS_BS_CAMERALIST_ACK);
    cRetMsg.Go2Content();
    cRetMsg.BuildNode("cameralist");
    cRetMsg.GoDownstairs();//current node cameralist
    BOOL32 bRet = GetNextLineString(achFileName,SECTION_CAMERADEVICE,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    while (bRet)
    {
        sscanf(achValueBuf,"%s%d%d%d%d%d",achCameraName,&nRegisterStatus,&nMatrixId,&nVideoPort,
			&nType,&nCamAddr);
		if (nMatrixId == nMatrixWebIndex)
		{
			//�ҵ��˾��������ͷ
			nTravalNum++;
			if (nTravalNum >= nStartIndex)
			{
				cRetMsg.BuildNode("camera");
				cRetMsg.GoDownstairs();//current node camera
				cRetMsg.BuildNode("guid","%s",achKeyBuf);
				cRetMsg.BuildNode("name","%s",achCameraName);
				cRetMsg.BuildNode("status","%d",nRegisterStatus);
				cRetMsg.BuildNode("matrixid","%d",nMatrixId);
				cRetMsg.BuildNode("videoport","%d",nVideoPort);
				cRetMsg.BuildNode("type","%d",nType);
				cRetMsg.BuildNode("addr","%d",nCamAddr);
				cRetMsg.GoUpstairs();//current node cameralist
				nTotalInserNum++;
			}

			if (nTotalInserNum >= GETMAXCAMERA_PERTIME)
			{
				break;//�Ѿ���ȡ����޶ȵ��豸
			}
		}
        bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
            achValueBuf,sizeof(achValueBuf));
    }
    cRetMsg.GoUpstairs();//current node content
	if (bRet)
	{
		cRetMsg.BuildNode("over","%d",0);
	}
	else
	{
		cRetMsg.BuildNode("over","%d",1);
	}
	SendMsgToClientConn(cRetMsg,pcMsg);
}



//=============================================================================
//  �� �� ���� ProcessGetEncoderList
//  ��    �ܣ� 
//  �㷨ʵ�֣� 
//  ȫ�ֱ����� 
//  ��    ���� CXmlMsg& cXmlMsg
//             CMessage *const pcMsg
//             CApp* pcApp
//  �� �� ֵ�� void 
//-----------------------------------------------------------------------------
//  �޸ļ�¼��
//  ��  ��	  �汾		�޸���		�߶���    �޸�����
// 2010-7-28  1.0	    ����ƽ                  ����
//=============================================================================
void CDevMgrIns::ProcessGetEncoderList(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
	s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
	
    s8 achKeyBuf[128 + 1] = {0};
    s8 achValueBuf[128 + 1] = {0};

	s8 szEncoderName[MAXLEN_NAME + 1] = {0};
    s32 nVideoSrcPortNum = 0;
	s32 nConnMatrix;
	s32 nMatrixId;


	
	cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();
    s32 nMatrixWebIndex = cXmlMsg.GetNodeIntValue("index");
	s32 nStartIndex = cXmlMsg.GetNodeIntValue("start");
	
	s32 nTotalInserNum = 0;//��ǰ�ҵ�������ͷ��
	s32 nTravalNum = 0;

	//��ȡ����ͷ�б�
    CXmlMsg cRetMsg;
    cRetMsg.SetCommand(G200MS_BS_ENCODERLIST_ACK);
    cRetMsg.Go2Content();


	//��ȡ�������б�
    cRetMsg.BuildNode("encoderlist");
    cRetMsg.GoDownstairs();//current node encoderlist
    BOOL32 bRet = GetNextLineString(achFileName,SECTION_ENCODERDEVICE,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
	BOOL32 bConnRet = FALSE;
    while (bRet)
    {
        sscanf(achValueBuf,"%s%d%d%d",szEncoderName,&nVideoSrcPortNum,&nConnMatrix,&nMatrixId);
		if (nMatrixId == nMatrixWebIndex)
		{
			nTravalNum++;
			if (nTravalNum >= nStartIndex)
			{
				cRetMsg.BuildNode("encoder");
				cRetMsg.GoDownstairs();//current node encoder
				cRetMsg.BuildNode("guid","%s",achKeyBuf);
				cRetMsg.BuildNode("name","%s",szEncoderName);
				cRetMsg.BuildNode("matrixid","%d",nMatrixId);
				cRetMsg.BuildNode("link","%d",nConnMatrix);
				cRetMsg.BuildNode("portnum","%d",nVideoSrcPortNum);
				
				//��ȡ���������ӹ�ϵ
				for (s32 nSrcIndex = 0; nSrcIndex < nVideoSrcPortNum; nSrcIndex++)
				{
					s8 achVideoSrc[5];
					s8 achMatrixPort[5];
					sprintf(achVideoSrc,"%d",nSrcIndex);
					bConnRet = GetRegKeyString(achFileName,achKeyBuf,achVideoSrc,NULL,achMatrixPort,sizeof(achMatrixPort));
					if (bConnRet)
					{
						cRetMsg.BuildNode("conn");
						cRetMsg.GoDownstairs();
						cRetMsg.BuildNode("encoderport","%s",achVideoSrc);
						cRetMsg.BuildNode("matrixport","%s",achMatrixPort);
						cRetMsg.GoUpstairs();
					}
				}
				
				cRetMsg.GoUpstairs();//current node encoderlist
				nTotalInserNum++;
			}

			if (nTotalInserNum >= GETMAXENCODER_PERTIME)
			{
				break;//�Ѿ���ȡ����޶ȵ��豸
			}
		}
        
        bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
            achValueBuf,sizeof(achValueBuf));
    }
	cRetMsg.GoUpstairs();//current node content
	if (bRet)
	{
		cRetMsg.BuildNode("over","%d",0);
	}
	else
	{
		cRetMsg.BuildNode("over","%d",1);
	}
    SendMsgToClientConn(cRetMsg,pcMsg);
}
/*=======================================================
������    :  ProcessAddLink
��  ��    :  ���ñ�������ƵԴ�˿����������˿ڵ����ӹ�ϵ
             ���Բ�����޸�
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessAddLink(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = InsertLinkToFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_ADDLINK_ACK);

        //SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_ADDLINK_NACK);
    }

    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::InsertLinkToFile(CXmlMsg &cXmlMsg)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    s8 achKdmNo[LEN_KDM_NO + 1] = {0};
    const s8* pszEnocderNo = cXmlMsg.GetNodeStringValue("guid");
    if (pszEnocderNo != NULL)
    {
        strncpy(achKdmNo,pszEnocderNo,LEN_KDM_NO);
        achKdmNo[LEN_KDM_NO] = 0;
    }

    s32 nEncoderSrcPort = cXmlMsg.GetNodeIntValue("encoderport");

    s32 nMatrixPort = cXmlMsg.GetNodeIntValue("matrixoutport");

    s8 achFileName[MAXLEN_FILENAME + 1];
    s8 achKeyBuf[128 + 1];
    BOOL32 bRet;
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);

    sprintf(achKeyBuf,"%d",nEncoderSrcPort);
    bRet = SetRegKeyInt(achFileName,achKdmNo,achKeyBuf,nMatrixPort);
    CHECK_WRITE_DB_RET(bRet,"Write DBFile Encoder Link:Failure!\n");

    return TRUE;
}

/*=======================================================
������    :  ProcessDelLink
��  ��    :  ɾ����������ƵԴ�˿����������˿ڵ�����
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessDelLink(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = DeleteLinkFromFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_DELLINK_ACK);

        //SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_DELLINK_NACK);
    }

    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::DeleteLinkFromFile(CXmlMsg &cXmlMsg)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();


    s8 achKdmNo[LEN_KDM_NO + 1] = {0};
    const s8* pszEnocderNo = cXmlMsg.GetNodeStringValue("guid");
    if (pszEnocderNo != NULL)
    {
        strncpy(achKdmNo,pszEnocderNo,LEN_KDM_NO);
        achKdmNo[LEN_KDM_NO] = 0;
    }

    s32 nEncoderSrcPort = cXmlMsg.GetNodeIntValue("encoderport");
    
    s8 achFileName[MAXLEN_FILENAME + 1];
    s8 achKeyBuf[128 + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
    sprintf(achKeyBuf,"%d",nEncoderSrcPort);
    
    BOOL32 bRet;
    bRet = DeleteRegKey(achFileName,achKdmNo,achKeyBuf);
    CHECK_WRITE_DB_RET(bRet,"Delete DBFile Encoder Link Item:Failure!\n");
    
    return TRUE;
}


void CDevMgrIns::ProcessUpdateUser(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = UpdateUserFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_UPDATE_USER_ACK);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_UPDATE_USER_NACK);
    }
    SendMsgToClientConn(cRetMsg,pcMsg);
}
BOOL32 CDevMgrIns::UpdateUserFile(CXmlMsg &cXmlMsg)
{
    s8 achG200FileName[MAXLEN_FILENAME+ 1];
    sprintf(achG200FileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
    
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();
    s8 achOldPsw[MAXLEN_ALIAS + 1] = {0};
    const s8* pszLastPsw = cXmlMsg.GetNodeStringValue("oldpsw");
    if (pszLastPsw != NULL)
    {
        strncpy(achOldPsw,pszLastPsw,MAXLEN_ALIAS);
        achOldPsw[MAXLEN_ALIAS] = 0;
    }
    

    s8 achUserName[MAXLEN_ALIAS + 1] = {0};
    const s8* pszUserName = cXmlMsg.GetNodeStringValue("username");
    if (pszUserName != NULL)
    {
        strncpy(achUserName,pszUserName,MAXLEN_ALIAS);
        achUserName[MAXLEN_ALIAS] = 0;
    }


    s8 achUserPsw[MAXLEN_ALIAS + 1] = {0};
    const s8* pszUserPsw = cXmlMsg.GetNodeStringValue("newpsw");
    if (pszUserPsw != NULL)
    {
        strncpy(achUserPsw,pszUserPsw,MAXLEN_ALIAS);
        achUserPsw[MAXLEN_ALIAS] = 0;
    }
    

    s8 achPsw[MAXLEN_ALIAS + 1];
    GetRegKeyString(achG200FileName,SECTION_CLIENT,KEY_PSW,"888888",achPsw,sizeof(achPsw));

    BOOL32  bRet;
    if (!strcmp(achOldPsw,achPsw))
    {
        bRet = SetRegKeyString(achG200FileName,SECTION_CLIENT,KEY_USERNAME,achUserName);
        CHECK_WRITE_DB_RET(bRet,"Write DBFile UserName,Failure!\n");
        bRet = SetRegKeyString(achG200FileName,SECTION_CLIENT,KEY_PSW,achUserPsw);
        CHECK_WRITE_DB_RET(bRet,"Write DBFile UserPassword,Failure!\n");
        return TRUE;
    }
    return FALSE;
}
void CDevMgrIns::ProcessLogin(CXmlMsg &cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    CXmlMsg cRetMsg;
    if (g_cDevMgrApp.GetLoginStatus())
    {
        //�û��Ѿ���¼
        cRetMsg.SetCommand(G200MS_BS_USER_LOGIN_NACK);
        cRetMsg.Go2Content();
        cRetMsg.BuildNode("status","%d",ERROR_USERHASLOGINNED);
        SendMsgToClientConn(cRetMsg,pcMsg);
        return;
    }
       
    cXmlMsg.Go2Content();//<content></content>
    cXmlMsg.GoDownstairs();//username

    s8 achTempUserName[MAXLEN_ALIAS + 1] = {0};
	const s8 * pchUserName  = cXmlMsg.GetNodeStringValue("username");
    if (pchUserName != NULL)
    {
        strncpy(achTempUserName,pchUserName,MAXLEN_ALIAS);
        achTempUserName[MAXLEN_ALIAS] = 0;
    }

    s8 achTempPsw[MAXLEN_ALIAS + 1] = {0};
	const s8 * pchPassword  = cXmlMsg.GetNodeStringValue("password");
	if (pchPassword != NULL)
	{
        strncpy(achTempPsw,pchPassword,MAXLEN_ALIAS);
        achTempPsw[MAXLEN_ALIAS] = 0;
	}
    
    s8 achUserName[MAXLEN_ALIAS];
    s8 achPsw[MAXLEN_ALIAS];
    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
    GetRegKeyString(achFileName,SECTION_CLIENT,KEY_USERNAME,"admin",achUserName,MAXLEN_ALIAS);
    GetRegKeyString(achFileName,SECTION_CLIENT,KEY_PSW,"888888",achPsw,MAXLEN_ALIAS);

    if (!strcmp(achUserName,achTempUserName) && !strcmp(achPsw,achTempPsw))
    {
        cRetMsg.SetCommand(G200MS_BS_USER_LOGIN_ACK);
        g_cDevMgrApp.SetLoginStatus(TRUE);

        //������ʱ��
        SetTimer(EV_G200MS_CLIENTCONN_TIMER,G200MS_CLIENTCONN_INTV);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_USER_LOGIN_NACK);
        cRetMsg.Go2Content();
        cRetMsg.BuildNode("status","%d",ERROR_USERNAME_PSW_NOEXISTING);
    }
    SendMsgToClientConn(cRetMsg,pcMsg);
}

/*=======================================================
������    :  ProcessAddMatrix
��  ��    :  ��Ӿ���
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessAddMatrix(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    s32 nMatrixId = 0;
    TMatrixItem tMatrixItem;
    bRet = InsertMatrixToFile(cXmlMsg,nMatrixId,tMatrixItem);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_ADD_MATRIX_ACK);
        cRetMsg.Go2Content();
        cRetMsg.BuildNode("index","%d",nMatrixId);

        //����Ϣת����g200�����޸��ڴ�
        //CMonMsg cMonMsg;
        //cMonMsg.SetMsgBody(&tMatrixItem,sizeof(TMatrixItem));
        //SendMsgToG200(cMonMsg,BS_G200MS_ADD_MATRIX_REQ);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_ADD_MATRIX_NACK);
		cRetMsg.Go2Content();
		cRetMsg.BuildNode("status","%d",g_cDevMgrApp.GetLastError());
    }
    
    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::InsertMatrixToFile(CXmlMsg &cXmlMsg,s32& nMatrixId,TMatrixItem &tMatrixItem)
{
    nMatrixId = 0;
    memset(&tMatrixItem,0,sizeof(TMatrixItem));

	TLicenseInfo tLicenseInfo = g_cDevMgrApp.GetLicenseInfo();
	if (!tLicenseInfo.bValid)
	{
		g_cDevMgrApp.SetLastError(ERROR_LICENSE_INVALID);
		return FALSE;
	}

	//�ж��Ƿ񳬹�֧�ֵ�������
	s32 nMatrixNum = g_tCfg.GetMatrixNum();
	if (nMatrixNum >= G200_MAX_MATRIX_NUM)
	{
		g_cDevMgrApp.SetLastError(ERROR_EXCEEDLIMIT);
		return FALSE;
	}

    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();
    const s8* pszMatrixName = cXmlMsg.GetNodeStringValue("name");

    s32 nMatrixType = cXmlMsg.GetNodeIntValue("type");
    
    s32 nMatrixInportNum = cXmlMsg.GetNodeIntValue("inport");

    s32 nMatrixOutportNum = cXmlMsg.GetNodeIntValue("outport");

    

	s32 nMaxMatrixId = g_tCfg.GetMaxMatrixId();
    s8 achG200FileName[MAXLEN_FILENAME + 1];
    sprintf(achG200FileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);

    BOOL32 bRet;
    bRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURMAXMATRIXID,nMaxMatrixId + 1);//�޸��ļ�id
    CHECK_WRITE_DB_RET(bRet,"Write DB Matrix Cur Id, Failure!\n");
    g_tCfg.SetMaxMatrixId(nMaxMatrixId + 1);//�޸��ڴ�id

    //��������Ϣд���ļ�
    s8 achKeyBuf[128 + 1] = {0};
    s8 achValueBuf[128 + 1] = {0};
    sprintf(achKeyBuf,"%d",nMaxMatrixId);
    sprintf(achValueBuf,"%s\t%d\t%d\t%d",pszMatrixName,nMatrixType,nMatrixInportNum,nMatrixOutportNum);

    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
    bRet = SetRegKeyString(achFileName,SECTION_MATRIXDEVICE,achKeyBuf,achValueBuf);
    CHECK_WRITE_DB_RET(bRet,"Write DB Matrix Item,Failure!\n");

	//����������1
	bRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURMATRIXNUM,nMatrixNum + 1);
	CHECK_WRITE_DB_RET(bRet,"Write DB Matrix Cur Num, Failure!\n");
	g_tCfg.SetMatrixNum(nMatrixNum + 1);

    nMatrixId = nMaxMatrixId;
    tMatrixItem.SetMatrixID(nMaxMatrixId);
    tMatrixItem.SetMatrixType(nMatrixType);
    tMatrixItem.SetMatrixName(pszMatrixName);
    tMatrixItem.SetMatrixInportNum(nMatrixInportNum);
    tMatrixItem.SetMatrixOutportNum(nMatrixOutportNum);
    return TRUE;
}

/*=======================================================
������    :  ProcessDeleteMatrix
��  ��    :  ɾ������
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessDeleteMatrix(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = DeleteMatrixFromFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_DELETE_MATRIX_ACK);

        //����g200����
        //SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_DELETE_MATRIX_NACK);
    }

    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::DeleteMatrixFromFile(CXmlMsg &cXmlMsg)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    s32 nTempMatrixId = cXmlMsg.GetNodeIntValue("index");
    s8 achFileName[MAXLEN_FILENAME + 1];//����˾��������ͷ���������ļ���
	s8 achG200FileName[MAXLEN_FILENAME + 1];//��������豸�ĵ�ǰ��
    sprintf(achG200FileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
    //�޸�����ͷ�����ľ���
    s8 achKeyBuf[128 + 1];
    s8 achValueBuf[128 + 1];
    s8 achCameraName[MAXLEN_ALIAS + 1];
	s32 nRegisterStatus;
	s32 nMatrixId;
	s32 nVideoPort;
	s32 nType;
	s32 nCamAddr;
	
	s32 nCamNum = g_tCfg.GetCamNum();
	s32 nEncoderNum = g_tCfg.GetEncoderNum();
	s32 nMatrixNum = g_tCfg.GetMatrixNum();

    BOOL32 bWriteRet;
    BOOL32 bReadRet = GetNextLineString(achFileName,SECTION_CAMERADEVICE,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    while (bReadRet)
    {
        sscanf(achValueBuf,"%s%d%d%d%d%d",achCameraName,&nRegisterStatus,&nMatrixId,&nVideoPort,&nType,&nCamAddr);
        if (nMatrixId == nTempMatrixId)
        {
            //���˾����µ���������ͷ����
            bWriteRet = UnRegisteCameraFrom3A(achKeyBuf);
            CHECK_WRITE_DB_RET(bWriteRet,"Unregiste Camera From 3A:Failure!\n");

            bWriteRet = DeleteRegKey(achFileName,SECTION_CAMERADEVICE,achKeyBuf);
            CHECK_WRITE_DB_RET(bWriteRet,"Delete DB File-Camera offline: Failure!\n");

			bWriteRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURCAMNUM,nCamNum - 1);
			CHECK_WRITE_DB_RET(bWriteRet,"Delete Matrix-Camera,Write DB File Camera Num: Failure!\n");
			g_tCfg.SetCamNum(nCamNum - 1);

        }  
        bReadRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
            achValueBuf,sizeof(achValueBuf));
    }

    //�޸ı�������������
    s8 szEncoderName[MAXLEN_NAME + 1] = {0};
    s32 nVideoSrcPortNum = 0;
	s32 nConnMatrix;
    bReadRet = GetNextLineString(achFileName,SECTION_ENCODERDEVICE,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    while (bReadRet)
    {
        sscanf(achValueBuf,"%s%d%d%d",szEncoderName,&nVideoSrcPortNum,&nConnMatrix,&nMatrixId);
        if (nMatrixId == nTempMatrixId)
        {
            
            bWriteRet = DeleteRegKey(achFileName,SECTION_ENCODERDEVICE,achKeyBuf);
            CHECK_WRITE_DB_RET(bWriteRet,"Delete DB File-Encoder offline: Failure!\n");
            
            //ɾ�����ӹ�ϵ
            DeleteSec(achFileName,achKeyBuf);

			bWriteRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURENCODERNUM,nEncoderNum - 1);
			CHECK_WRITE_DB_RET(bWriteRet,"Delete Matrix-Encoder,Write DB File Encoder Num: Failure!\n");
			g_tCfg.SetEncoderNum(nEncoderNum - 1);
        }
        bReadRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    }

    sprintf(achKeyBuf,"%d",nTempMatrixId);
    bWriteRet = DeleteRegKey(achFileName,SECTION_MATRIXDEVICE,achKeyBuf);
    CHECK_WRITE_DB_RET(bWriteRet,"Delete Matrix Item: Failure!\n");

	bWriteRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURMATRIXNUM,nMatrixNum - 1);
	CHECK_WRITE_DB_RET(bWriteRet,"Delete Matrix,Write DB File Matrix Num: Failure!\n");
	g_tCfg.SetMatrixNum(nMatrixNum - 1);

    return TRUE;
}
/*=======================================================
������    :  ProcessUpdateMatrix
��  ��    :  �޸ľ���
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessUpdateMatrix(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = UpdateMatrixToFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_UPDATE_MATRIX_ACK);

        //����g200����
        //SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_UPDATE_MATRIX_NACK);
    }

    SendMsgToClientConn(cRetMsg,pcMsg);
}
BOOL32 CDevMgrIns::UpdateMatrixToFile(CXmlMsg &cXmlMsg)
{
    //�����޸�ֻ���޸����ƺ����ͣ����������Ķ˿��������޸�
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    s32 nMatrixId = cXmlMsg.GetNodeIntValue("index");

    s32 nMatrixType = cXmlMsg.GetNodeIntValue("type");

    const s8* pszMatrixName = cXmlMsg.GetNodeStringValue("name");

    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);

    s8 achKeyBuf[128 + 1];
    s8 achValueBuf[128 + 1];
    sprintf(achKeyBuf,"%d",nMatrixId);

    GetRegKeyString(achFileName,SECTION_MATRIXDEVICE,achKeyBuf,NULL,achValueBuf,sizeof(achValueBuf));
    s8 achMatrixName[MAXLEN_ALIAS + 1];
    s32 nTempMatrixType;
    s32 nMatrixInportNum;
    s32 nMatrixOutportNum;
    sscanf(achValueBuf,"%s%d%d%d",achMatrixName,&nTempMatrixType,&nMatrixInportNum,&nMatrixOutportNum);

    sprintf(achValueBuf,"%s %d %d %d",pszMatrixName,nMatrixType,nMatrixInportNum,nMatrixOutportNum);
    BOOL32 bRet = SetRegKeyString(achFileName,SECTION_MATRIXDEVICE,achKeyBuf,achValueBuf);
    CHECK_WRITE_DB_RET(bRet,"Update DB Matrix Item:Failure!");

    return TRUE;
}

/*=======================================================
������    :  ProcessDeleteEncoder
��  ��    :  ɾ��������
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessDeleteEncoder(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = DeleteEncoderFromFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_DELETE_ENCODER_ACK);
        //SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_DELETE_ENCODER_NACK);
    }

    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::DeleteEncoderFromFile(CXmlMsg &cXmlMsg)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    const s8* pszEncoderNo = cXmlMsg.GetNodeStringValue("guid");

    s8 achFileName[MAXLEN_FILENAME + 1];//�����������ͷ�����������ļ���
	s8 achValueBuff[128];
	s8 szEncoderName[MAXLEN_NAME + 1] = {0};
    s32 nVideoSrcPortNum = 0;
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
    
	//��ȡ��ǰ������ռ�õ���ƵԴ�˿���
	GetRegKeyString(achFileName,SECTION_ENCODERDEVICE,pszEncoderNo,"",achValueBuff,sizeof(achValueBuff));
	sscanf(achValueBuff,"%s%d",szEncoderName,&nVideoSrcPortNum);

    BOOL32 bRet;
    bRet = DeleteRegKey(achFileName,SECTION_ENCODERDEVICE,pszEncoderNo);
    CHECK_WRITE_DB_RET(bRet,"Delete DB Enocder Item:Failure!\n");

    DeleteSec(achFileName,pszEncoderNo);
   
	s8 achG200FileName[MAXLEN_FILENAME + 1];
	s32 nEncoderNum = g_tCfg.GetEncoderNum();
    sprintf(achG200FileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
	bRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURCAMNUM,nEncoderNum - nVideoSrcPortNum);
	CHECK_WRITE_DB_RET(bRet,"Delete Encoder,Write DB File Encoder Num:Failure!\n");
	g_tCfg.SetCamNum(nEncoderNum - nVideoSrcPortNum);

    return TRUE;
}
/*=======================================================
������    :  ProcessUpdateEncoder
��  ��    :  �޸ı�����
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessUpdateEncoder(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = UpdateEncoderToFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_UPDATE_ENCODER_ACK);
        
        //SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_UPDATE_ENCODER_NACK);
    }

    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::UpdateEncoderToFile(CXmlMsg &cXmlMsg)
{
    //�޸ı�����ֻ���޸ı����������ơ��Լ�����������󴮿ڵ�����״̬
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    s8 achEncoderNo[LEN_KDM_NO + 1] = {0};
    const s8* pszEncoderNo = cXmlMsg.GetNodeStringValue("guid");
    if (pszEncoderNo != NULL)
    {
        strncpy(achEncoderNo,pszEncoderNo,LEN_KDM_NO);
        achEncoderNo[LEN_KDM_NO] = '\0';
    }


    s8 achNewEncoderName[MAXLEN_ALIAS + 1];
    const s8* pszEncoderName = cXmlMsg.GetNodeStringValue("name");
    if (pszEncoderName != NULL)
    {
        strncpy(achNewEncoderName,pszEncoderName,MAXLEN_ALIAS);
        achNewEncoderName[MAXLEN_ALIAS] = '\0';
    }

    
    BOOL32 bLinkStatus = cXmlMsg.GetNodeIntValue("link");

    s32 nCurMatrixId = cXmlMsg.GetNodeIntValue("matrixid");

    s8 achCurFileName[MAXLEN_FILENAME + 1];
    sprintf(achCurFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
    s8 achValueBuf[128 + 1];
    s8 achOldEncoderName[MAXLEN_ALIAS + 1];
    s32 nPortNum;
    BOOL32 bRet;
       
    GetRegKeyString(achCurFileName,SECTION_ENCODERDEVICE,achEncoderNo,NULL,achValueBuf,sizeof(achValueBuf));
    sscanf(achValueBuf,"%s%d",achOldEncoderName,&nPortNum);//��ȡԭ�ȵ���ƵԴ�˿���

    sprintf(achValueBuf,"%s %d %d %d",achNewEncoderName,nPortNum,bLinkStatus,nCurMatrixId);

    bRet =SetRegKeyString(achCurFileName,SECTION_ENCODERDEVICE,achEncoderNo,achValueBuf);
    CHECK_WRITE_DB_RET(bRet,"Update DBFile Encoder Item:Failure!\n");

    return TRUE;
}
/*=======================================================
������    :  ProcessDeleteCamera
��  ��    :  ɾ������ͷ
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessDeleteCamera(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = DeleteCameraFromFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_DELETE_CAMERA_ACK);
        //SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_DELETE_CAMERA_NACK);
    }
    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::DeleteCameraFromFile(CXmlMsg &cXmlMsg)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    const s8* pszCameraNo = cXmlMsg.GetNodeStringValue("guid");

    BOOL32 bRet;
    bRet = UnRegisteCameraFrom3A((s8*)pszCameraNo);
    CHECK_WRITE_DB_RET(bRet,"Unregiste Camera From 3A:Failure!\n");
    
    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);

    bRet = DeleteRegKey(achFileName,SECTION_CAMERADEVICE,pszCameraNo);
    CHECK_WRITE_DB_RET(bRet,"Delete DBFile Camera Item:Failure!\n");
    
	s8 achG200FileName[MAXLEN_FILENAME + 1];
	s32 nCamNum = g_tCfg.GetCamNum();
    sprintf(achG200FileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
	bRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURCAMNUM,nCamNum - 1);
	CHECK_WRITE_DB_RET(bRet,"Delete Camera,Write DB File Camera Num:Failure!\n");
	g_tCfg.SetCamNum(nCamNum - 1);

    return TRUE;
}
/*=======================================================
������    :  ProcessUpdateCamera
��  ��    :  �޸�����ͷ
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessUpdateCamera(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    BOOL32 bRet;
    CXmlMsg cRetMsg;
    bRet = UpdateCameraToFile(cXmlMsg);
    if (bRet)
    {
        cRetMsg.SetCommand(G200MS_BS_UPDATE_CAMERA_ACK);
        //SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200MS_BS_UPDATE_CAMERA_NACK);
    }

    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::UpdateCameraToFile(CXmlMsg &cXmlMsg)
{
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    s8 achCamNo[LEN_KDM_NO + 1] = {0};
    const s8* pszCameraNo = cXmlMsg.GetNodeStringValue("guid");
    if (pszCameraNo != NULL)
    {
        strncpy(achCamNo,pszCameraNo,LEN_KDM_NO);
        achCamNo[LEN_KDM_NO] = '\0';
    }
    
    s8 achTempCameraName[MAXLEN_ALIAS + 1];
    const s8* pszCameraName = cXmlMsg.GetNodeStringValue("name");
    if (pszCameraName != NULL)
    {
        strncpy(achTempCameraName,pszCameraName,MAXLEN_ALIAS);
        achTempCameraName[MAXLEN_ALIAS] = '\0';
    }
    
    s32 nCurMatrixId = cXmlMsg.GetNodeIntValue("matrixid");

    s32 nCurPort = cXmlMsg.GetNodeIntValue("videoport");

    s32 nCurType =cXmlMsg.GetNodeIntValue("type");

    s32 nCurCamAddr = cXmlMsg.GetNodeIntValue("addr");

    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);


    s8 achValueBuf[128 + 1];
    BOOL32 bRet;
    s8 achCameraName[MAXLEN_ALIAS + 1];
	s32 nRegisterStatus;

    bRet = ModifyCameraTo3A(achCamNo,achTempCameraName);
    CHECK_WRITE_DB_RET(bRet,"Modify Camera to 3A: Failure!\n");

    GetRegKeyString(achFileName,SECTION_CAMERADEVICE,achCamNo,NULL,achValueBuf,sizeof(achValueBuf));
    sscanf(achValueBuf,"%s%d",achCameraName,&nRegisterStatus);//��ȡ��ע��״̬
    sprintf(achValueBuf,"%s %d %d %d %d %d",achTempCameraName,nRegisterStatus,nCurMatrixId,nCurPort,nCurType,nCurCamAddr);

    bRet =SetRegKeyString(achFileName,SECTION_CAMERADEVICE,achCamNo,achValueBuf);
    CHECK_WRITE_DB_RET(bRet,"Update DB File-Camera item: Failure!\n");
    return TRUE;
}
/*=======================================================
������    :  ProcessAddEncoder
��  ��    :  ��ӱ�����
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessAddEncoder(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
    CXmlMsg cRetMsg;
    BOOL32 bRet;
    s8 achKdmNo[LEN_KDM_NO + 1] = {0};
    TVideoOutItem tOutItem;
    bRet = InsertEncoderToFile(cXmlMsg,achKdmNo,tOutItem);
    if (bRet)
    {
        cRetMsg.SetCommand(G200_BS_ADD_ENCODER_ACK);
        cRetMsg.Go2Content();
        cRetMsg.BuildNode("guid","%s",achKdmNo);
        
        //����Ϣת����g200�����޸��ڴ�
//         CMonMsg cMonMsg;
//         cMonMsg.SetMsgBody(&tOutItem,sizeof(TVideoOutItem));
//         SendMsgToG200(cMonMsg,BS_G200MS_ADD_ENCODER_REQ);
    }
    else
    {
        cRetMsg.SetCommand(G200_BS_ADD_ENCODER_NACK);
		cRetMsg.Go2Content();
		cRetMsg.BuildNode("status","%d",g_cDevMgrApp.GetLastError());
    }
    
    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::InsertEncoderToFile(CXmlMsg &cXmlMsg,s8* pszKdmNo,TVideoOutItem& tOutItem)
{
    memset(&tOutItem,0,sizeof(TVideoOutItem));

    TLicenseInfo tLicenseInfo = g_cDevMgrApp.GetLicenseInfo();
	s32 nEncoderNum = g_tCfg.GetEncoderNum();
	if (!tLicenseInfo.bValid)
	{
		g_cDevMgrApp.SetLastError(ERROR_LICENSE_INVALID);
		return FALSE;
	}
	
    cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();
    s32 nMatrixId = cXmlMsg.GetNodeIntValue("matrixid");

    const s8* pszEncoderName = cXmlMsg.GetNodeStringValue("name");
    
    BOOL32 bLinkStatus = cXmlMsg.GetNodeIntValue("link");

    s32 nVideoSrcPortNum = cXmlMsg.GetNodeIntValue("portnum");

	s32 nCurTotalNum = nEncoderNum + nVideoSrcPortNum;
	if ( nCurTotalNum > tLicenseInfo.nMaxEncoderNum)
	{
		//����ͨ���������涨��ֵ
		g_cDevMgrApp.SetLastError(ERROR_EXCEEDLIMIT);
		return FALSE;
	}

    BOOL32 bRet;
    s32 nEncoderNO = g_tCfg.GetMaxEncoderNO();
    //���ɱ��������
    s8 achKdmNo[LEN_KDM_NO + 1] = {0};
    sprintf(achKdmNo,"%s112000000%05d",g_tCfg.GetKdmNoHead(),nEncoderNO);
    s8 achG200FileName[MAXLEN_FILENAME + 1];
    sprintf(achG200FileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
    bRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURMAXENCODERNO,nEncoderNO + 1);//�޸��ļ�id
    CHECK_WRITE_DB_RET(bRet,"Write DB Encoder CurId,Failure!");
    g_tCfg.SetMaxEncoderNO(nEncoderNO + 1);

    s8 achKeyBuf[128 + 1] = {0};
    s8 achValueBuf[128 + 1] = {0};
    s8 achFileName[MAXLEN_FILENAME];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);

    sprintf(achKeyBuf,"%s",achKdmNo);
    sprintf(achValueBuf,"%s %d %d %d",pszEncoderName,nVideoSrcPortNum,bLinkStatus,nMatrixId);
    bRet = SetRegKeyString(achFileName,SECTION_ENCODERDEVICE,achKeyBuf,achValueBuf);
    CHECK_WRITE_DB_RET(bRet,"Write DB Encoder Item,Failure!");

	//���ӵ�ǰ��������
	bRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURENCODERNUM,nCurTotalNum);
	CHECK_WRITE_DB_RET(bRet,"Add Encoder,Write DB Encoder Num,Failure!");
	g_tCfg.SetEncoderNum(nCurTotalNum);

    strncpy(pszKdmNo,achKdmNo,LEN_KDM_NO);

    TBasePu tBasePu;
    tBasePu.SetNOString(achKdmNo);
    tBasePu.SetObjName(pszEncoderName);
    tOutItem.SetEncoder(tBasePu);
    tOutItem.SetConnMatrix(bLinkStatus);
    tOutItem.SetVideoSrcPortNum(nVideoSrcPortNum);
    return TRUE;
}
/*=======================================================
������    :  ProcessAddCamera
��  ��    :  �������ͷ
�㷨ʵ��  :  
����˵��  :  			 
����ֵ˵��:  
----------------------------------------------------------
�޸ļ�¼  :
��   ��       �汾      �޸���     �߶���      �޸ļ�¼
2010/2/26     V1.0      ����ƽ                  ����
=========================================================*/
void CDevMgrIns::ProcessAddCamera(CXmlMsg& cXmlMsg,CMessage *const pcMsg,CApp* pcApp)
{
   
    CXmlMsg cRetMsg;
    BOOL32 bRet;
    s8 achKdmNo[LEN_KDM_NO + 1] = {0};
    TVideoInItem tInItem;
    bRet = InsertCameraToFile(cXmlMsg,achKdmNo,tInItem);
    if (bRet)
    {
        cRetMsg.SetCommand(G200_BS_ADD_CAMERA_ACK);
        cRetMsg.Go2Content();
        cRetMsg.BuildNode("guid","%s",achKdmNo);

        //����Ϣת����g200�����޸��ڴ�
//         CMonMsg cMonMsg;
//         cMonMsg.SetMsgBody(&tInItem,sizeof(TVideoInItem));
//         SendMsgToG200(pcMsg);
    }
    else
    {
        cRetMsg.SetCommand(G200_BS_ADD_CAMERA_NACK);
		cRetMsg.Go2Content();
		cRetMsg.BuildNode("status","%d",g_cDevMgrApp.GetLastError());
    }
    SendMsgToClientConn(cRetMsg,pcMsg);
}

BOOL32 CDevMgrIns::InsertCameraToFile(CXmlMsg &cXmlMsg,s8* pszKdmNo,TVideoInItem& tInItem)
{
    memset(&tInItem,0,sizeof(TVideoInItem));
    
    TLicenseInfo tLicenseInfo = g_cDevMgrApp.GetLicenseInfo();
	s32  nCamNum = g_tCfg.GetCamNum();

	if (!tLicenseInfo.bValid)
	{
		g_cDevMgrApp.SetLastError(ERROR_LICENSE_INVALID);
		return FALSE;
	}

	if ( nCamNum >= tLicenseInfo.nMaxVideoNum)
	{
		g_cDevMgrApp.SetLastError(ERROR_EXCEEDLIMIT);
		return FALSE;
	}

	cXmlMsg.Go2Content();
    cXmlMsg.GoDownstairs();

    s32 nMatrixId = cXmlMsg.GetNodeIntValue("matrixid");

    s32 nMatrixPort = cXmlMsg.GetNodeIntValue("videoport");
    
    s32 nCamType = cXmlMsg.GetNodeIntValue("type");

    const s8* pszCamName = cXmlMsg.GetNodeStringValue("name");

    s32 nCamAddr = cXmlMsg.GetNodeIntValue("addr");

    s32 nCamNO = g_tCfg.GetMaxCamNO();
    s8 achKdmNo[LEN_KDM_NO + 1] = {0};
    BOOL32 bRet;
    sprintf(achKdmNo,"%s112%06d00000",g_tCfg.GetKdmNoHead(),nCamNO);
    
    bRet = RegisteCameraTo3A(achKdmNo,pszCamName);
    CHECK_WRITE_DB_RET(bRet,"Registe Camera To 3A,Failure!\n");

    s8 achG200FileName[MAXLEN_FILENAME + 1];
    sprintf(achG200FileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_NAME);
    bRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURMAXCAMNO,nCamNO + 1);//д�ļ�
    CHECK_WRITE_DB_RET(bRet,"Write DB Camera CurId,Failure!\n");
    g_tCfg.SetMaxCamNO(nCamNO + 1);//д�ڴ�

    s8 achFileName[MAXLEN_FILENAME + 1];
    sprintf(achFileName,"%s%s",g_tCfg.GetBindRtCfgPath(),CONFIG_FILE_DEVICES);
    
    s8 achValueBuf[128 + 1] = {0};
    sprintf(achValueBuf,"%s %d %d %d %d %d",pszCamName,1,nMatrixId,nMatrixPort,nCamType,nCamAddr);
    bRet = SetRegKeyString(achFileName,SECTION_CAMERADEVICE,achKdmNo,achValueBuf);
    CHECK_WRITE_DB_RET(bRet,"Write DB Camera Item,Failure!\n");
    
	//д��ǰ����ͷ��
	bRet = SetRegKeyInt(achG200FileName,SECTION_DEVICENO,KEY_CURCAMNUM,nCamNum + 1);
	CHECK_WRITE_DB_RET(bRet,"Add Cam,Write DB Camera Num,Failure!\n");
	g_tCfg.SetCamNum(nCamNum + 1);

    strncpy(pszKdmNo,achKdmNo,LEN_KDM_NO);
    
    TBasePu tBasePu;
    tBasePu.SetNOString(achKdmNo);
    tBasePu.SetObjName(pszCamName);
    tInItem.SetCam(tBasePu);
    tInItem.SetRegisterStatus(TRUE);

    tInItem.SetVideoInPort(nMatrixPort);
    tInItem.SetCamAddr(nCamAddr);
    tInItem.SetCamType(nCamType);
    return TRUE;
}

BOOL32 CDevMgrIns::RegisteCameraTo3A(const s8* pszKdmNo,const s8* pszCameraName)
{
    BOOL32 bRet = FALSE;
    if (!g_cDevMgrApp.bInit3ASdk())
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr-Daemon][%s] : δ��ʼ��3ASSdk���������豸ע�ᣡ\n",GetStrState());
		return FALSE;
	}
    
    unsigned short nRet = VS_AAAOK;
	VS_DEV_REG_INFO tRegInfo = {0};
	time_t nCreateTime;
	time(&nCreateTime);
	
	TKDMTime tExpireTime;
	tExpireTime.SetYear(2037);
	tExpireTime.SetMonth(12);
	tExpireTime.SetDay(31);
	tExpireTime.SetHour(0);
	tExpireTime.SetMinute(0);
	tExpireTime.SetSecond(0);
	
    memset(&tRegInfo, 0, sizeof(VS_DEV_REG_INFO));
	strncpy(tRegInfo.abyDevNo, pszKdmNo, VS_AAA_MAXLEN_DEV_NO);
	strncpy(tRegInfo.achDevName, pszCameraName, VS_AAA_MAXLEN_ALIAS);
	strcpy(tRegInfo.achManufacturer, "kedacom");
	strcpy(tRegInfo.achNetAddress, "1.0.0.0");
	tRegInfo.wCameraNum = 1;
	tRegInfo.bUsedFlag = TRUE;
	tRegInfo.byDeviceType = 22;
	
	tRegInfo.dwCreateDate = (u32)nCreateTime;
	tRegInfo.dwExpireDate = (u32)tExpireTime.GetTime();
	
	nRet = VS_DevRegReq(tRegInfo, sizeof(VS_DEV_REG_INFO));   
	if (VS_AAAOK != nRet)
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr-Daemon][%s] : ע���豸[%s -- %s]ʧ��, �����룺%d��\n", 
				                                 GetStrState(), pszKdmNo, pszCameraName, nRet);
	}
	else
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr][%s] : ע���豸[%s -- %s]�ɹ���\n", 
				                                 GetStrState(), pszKdmNo, pszCameraName);
        bRet = TRUE;
    }
    return bRet;
}

BOOL32 CDevMgrIns::ModifyCameraTo3A(const s8* pszKdmNo,const s8* pszCameraName)
{
    BOOL32 bRet = FALSE;
    if (!g_cDevMgrApp.bInit3ASdk())
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr-Daemon][%s] : δ��ʼ��3ASSdk���������豸ע�ᣡ\n",GetStrState());
		return FALSE;
	}

    unsigned short nRet = VS_AAAOK;
	VS_DEV_REG_INFO tRegInfo = {0};
	time_t nCreateTime;
	time(&nCreateTime);
	
	TKDMTime tExpireTime;
	tExpireTime.SetYear(2037);
	tExpireTime.SetMonth(12);
	tExpireTime.SetDay(31);
	tExpireTime.SetHour(0);
	tExpireTime.SetMinute(0);
	tExpireTime.SetSecond(0);
	
    memset(&tRegInfo, 0, sizeof(VS_DEV_REG_INFO));
	strncpy(tRegInfo.abyDevNo, pszKdmNo, VS_AAA_MAXLEN_DEV_NO);
	strncpy(tRegInfo.achDevName, pszCameraName, VS_AAA_MAXLEN_ALIAS);
	strcpy(tRegInfo.achManufacturer, "kedacom");
	strcpy(tRegInfo.achNetAddress, "1.0.0.0");
	tRegInfo.wCameraNum = 1;
	tRegInfo.bUsedFlag = TRUE;
	tRegInfo.byDeviceType = 22;
	
	tRegInfo.dwCreateDate = (u32)nCreateTime;
	tRegInfo.dwExpireDate = (u32)tExpireTime.GetTime();

    nRet = VS_DevInfoModReq(tRegInfo,sizeof(VS_DEV_REG_INFO));
    if (VS_AAAOK != nRet)
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr-Daemon][%s] : �޸�3A�豸[%s -- %s]ʧ��, �����룺%d��\n", 
				                                 GetStrState(), pszKdmNo, pszCameraName, nRet);
	}
	else
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr][%s] : �޸�3A�豸[%s -- %s]�ɹ���\n", 
				                                 GetStrState(), pszKdmNo, pszCameraName);
        bRet = TRUE;
    }
    return bRet;
}


BOOL32 CDevMgrIns::UnRegisteCameraFrom3A(s8* pszKdmNo)
{
    BOOL32 bRet = FALSE;
    if (!g_cDevMgrApp.bInit3ASdk())
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr-Daemon][%s] : δ��ʼ��3ASSdk���������豸ע�ᣡ\n",GetStrState());
		return FALSE;
	}

    unsigned short nRet = VS_AAAOK;
    nRet = VS_DevRegUnReq(pszKdmNo,LEN_KDM_NO);
    if (VS_AAAOK != nRet)
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr-Daemon][%s] : ע��3A�豸[%s]ʧ��, �����룺%d��\n", 
				                                 GetStrState(), pszKdmNo, nRet);
	}
	else
	{
		G200MSLog(LOG_DEVMGR_MOD, LOGLVL_DEBUG1, "[G200MS/DevMgr][%s] : ע��3A�豸[%s -- %s]�ɹ���\n", 
				                                 GetStrState(), pszKdmNo);
        bRet = TRUE;
    }
    return bRet;
}