#include "CfgMgr.h"
#include "G200Data.h"

const u32 g_dwMaxMsgLen = MON_OSP_MSG_BODY_MAX_LEN;
CCfgMgrAppData::CCfgMgrAppData()
{
	m_dwRealUsedMatrixNum = 0;

	memset(m_tMatrixItem, 0, sizeof(m_tMatrixItem));
	memset(m_abyCfgPath, 0, sizeof(m_abyCfgPath));
}


void CCfgMgrAppData::SetCfgPath(LPCSTR lpszPath)
{
	if (NULL == lpszPath || strlen(lpszPath) == 0)
	{
		return;
	}

	u32 dwLen = strlen(lpszPath) >= MAXLEN_FILEPATH ? MAXLEN_FILEPATH : strlen(lpszPath);
	strncpy(m_abyCfgPath, lpszPath, dwLen);
	if (m_abyCfgPath[dwLen-1] != '/')
	{
		m_abyCfgPath[dwLen] = '/';
		m_abyCfgPath[dwLen+1] = '\0';
	}
}

LPCSTR CCfgMgrAppData::GetCfgPath()
{
	return m_abyCfgPath;
}

TItemList* CCfgMgrAppData::GetItemListPtr()
{
	return &m_tItemList;
}

void CCfgMgrAppData::AddItem(TVideoOutItem& tItem)
{
    if (IsHasItem(tItem.GetEncoder().GetKdmNo())) return;

	TVideoOutItem* ptItem = m_tItemList.AddItem();
	if ( NULL != ptItem )
	{
		*ptItem = tItem;
	}
}

BOOL32 CCfgMgrAppData::IsHasItem(const TKDMNO& tKdmNo)
{
	TVideoOutItem *ptItem = m_tItemList.PosFirst();
	for(; NULL != ptItem; ptItem = m_tItemList.PosNext(ptItem))
	{
		if (ptItem->GetEncoder().GetKdmNo() == tKdmNo)
		{
			return TRUE;
		}
	}

	return FALSE;
}

TVideoOutItem* CCfgMgrAppData::GetItem(const TKDMNO& tKdmNo)
{
	TVideoOutItem *ptItem = m_tItemList.PosFirst();
	for(; NULL != ptItem; ptItem = m_tItemList.PosNext(ptItem))
	{
		if (ptItem->GetEncoder().GetKdmNo() == tKdmNo)
		{
			return ptItem;
		}
	}

	return NULL;
}

void CCfgMgrAppData::ClearItemList()
{
	m_tItemList.Clear();
}

TWaitForRegDevList* CCfgMgrAppData::GetRegDevListPtr()
{
	return &m_tWaitForRegDevList;
}

void CCfgMgrAppData::AddDev(const TVideoInItem& tVideoInItem)
{
	TVideoInItem* ptDev = m_tWaitForRegDevList.AddItem();

	if ( NULL != ptDev )
	{
		*ptDev = tVideoInItem;
	}
}

BOOL32 CCfgMgrAppData::ReadBindRtCfg(LPCSTR lpszCfgFilePath)
{
	s8 szCfgFileName[MAXLEN_FILEPATH+64] = {0};

	if (NULL == lpszCfgFilePath || strlen(lpszCfgFilePath) == 0)
	{
		G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "Invalid path of matrix cfg file!\n");
		return FALSE;
	}

	strncpy(szCfgFileName, lpszCfgFilePath, strlen(lpszCfgFilePath));
	UnifyDir(szCfgFileName, strlen(szCfgFileName));
	SetCfgPath(szCfgFileName);
	
	//读取矩阵配置文件
	ReadMatrixCfg();

	//读取绑定关系配置文件中视频输入绑定关系
	ReadVideoInCfg();

	//读取绑定关系配置文件中视频输出绑定关系
	ReadVideoOutCfg();
	
	return TRUE;
}

/**************************************************************
函数名:     GetMatrixType
功能:       获取矩阵类型
算法实现:	
参数说明:   s32 nMatrixId:矩阵编号
返回值说明: 默认返回零
----------------------------------------------------------------
修改记录:
日期           版本              修改人              修改内容
***************************************************************/
u8 CCfgMgrAppData::GetMatrixType(s32 nMatrixId)
{
	for(s32 index = 0; index < m_dwRealUsedMatrixNum; index++)
	{
		
		if (nMatrixId == m_tMatrixItem[index].GetMatrixID())
		{
			return m_tMatrixItem[index].GetMatrixType();
		}
	}
	return 0;
}
/************************************************************************/
/*          |      GroupID     |   MatrixType    |      IsUsed          */
/*----------------------------------------------------------------------*/
/*Entry0=   |        1         |       PELCO     |        1             */
/************************************************************************/
BOOL32 CCfgMgrAppData::ReadMatrixCfg()
{
	u32 dwUsedMatrixNum = 0;
    u32 dwMaxMatrixNum = G200_MAX_MATRIX_NUM;

    s8 szMatrixName[MAXLEN_ALIAS + 1] = {0};
    s32 nMatrixInportNum = 0;
    s32 nMatrixOutportNum = 0;
    s32 nMatrixType;

	CMonMsg cMonMsg;
	TTotalNum tTotalNum;
	tTotalNum.SetTotalNum(dwUsedMatrixNum);
	cMonMsg.SetMsgBody(&tTotalNum, sizeof(TTotalNum));

	if (!g_cVcData.m_cLicenseHandler.IsLicenseValid())
	{
		dwMaxMatrixNum = 0;
	}

    s8 szKeyBuf[128 + 1];
    s8 szValueBuf[128 + 1];
    s8 achFileName[MAXLEN_FILENAME] = {0};
    sprintf(achFileName,"%s%s",GetCfgPath(),CONFIG_FILE_DEVICES);
    BOOL32 bRet = GetNextLineString(achFileName,SECTION_MATRIXDEVICE,szKeyBuf,sizeof(szKeyBuf),
        szValueBuf,sizeof(szValueBuf));
    while (bRet)
    {      
        if (dwUsedMatrixNum >= dwMaxMatrixNum)
        {
			break;
        }

		m_tMatrixItem[dwUsedMatrixNum].SetMatrixID(atoi(szKeyBuf));
        sscanf(szValueBuf,"%s%d%d%d",szMatrixName,&nMatrixType,&nMatrixInportNum,&nMatrixOutportNum);
        m_tMatrixItem[dwUsedMatrixNum].SetMatrixName(szMatrixName);
        m_tMatrixItem[dwUsedMatrixNum].SetMatrixType(nMatrixType);
        m_tMatrixItem[dwUsedMatrixNum].SetMatrixInportNum(nMatrixInportNum);
        m_tMatrixItem[dwUsedMatrixNum].SetMatrixOutportNum(nMatrixOutportNum);
        cMonMsg.CatMsgBody(&m_tMatrixItem[dwUsedMatrixNum], sizeof(TMatrixItem));

		G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : MatrixID:%s, MatrixType:%d\n", szKeyBuf, nMatrixType);
        dwUsedMatrixNum++;
        bRet = GetNextLineString(NULL,NULL,szKeyBuf,sizeof(szKeyBuf),
        szValueBuf,sizeof(szValueBuf));
    }

	TTotalNum *ptTotalNum = (TTotalNum *)cMonMsg.GetMsgBody();
	ptTotalNum->SetTotalNum(dwUsedMatrixNum);
	OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_CFGMGR_GET_MATRIX_NTY, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());

	m_dwRealUsedMatrixNum = dwUsedMatrixNum;

	G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : 读取的矩阵数目 ：%d\n", dwUsedMatrixNum);

	return TRUE;
}


/************************************************************************/
/*          |      VideoInPort     |   CamNo          |      IsReg      */
/*----------------------------------------------------------------------*/
/*Entry0=   |        1             |  55.....1....    |        1        */
/************************************************************************/
BOOL32 CCfgMgrAppData::ReadVideoInCfg()
{
	u32 dwRealRegNum = 0;//每个矩阵总的摄像头设备数
	u32 dwRealSendNum = 0;
	s8 szBindRtFileName[MAXLEN_FILEPATH+64] = {0};

	TVideoInItem tInItem;
	TBasePu tBasePu;

	CMonMsg cMonMsg;
	TTotalNum tTotalNum;
	tTotalNum.SetTotalNum(dwRealSendNum);
	cMonMsg.SetMsgBody(&tTotalNum, sizeof(TTotalNum));
	u32 dwToTalNum =  (g_dwMaxMsgLen - sizeof(TTotalNum)) / sizeof(TVideoInItem);
	
	sprintf(szBindRtFileName,"%s%s",GetCfgPath(),CONFIG_FILE_DEVICES);
   
    s8 achKeyBuf[128 + 1];
    s8 achValueBuf[128 + 1];
    s8 achCameraName[MAXLEN_ALIAS + 1];
	s32 nRegisterStatus;
	s32 nMatrixId;
	s32 nVideoPort;
	s32 nType;
	s32 nCamAddr;

	u32 nMaxCamNum = 0;
    //通过license控制支持的摄像头数
	if (g_cVcData.m_cLicenseHandler.IsLicenseValid())
	{
		nMaxCamNum = g_cVcData.m_cLicenseHandler.GetMaxChannel();
	}
	

	BOOL32 bRet = GetNextLineString(szBindRtFileName,SECTION_CAMERADEVICE,achKeyBuf,sizeof(achKeyBuf),
		achValueBuf,sizeof(achValueBuf));
	while (bRet)
	{
		if (dwRealRegNum >= nMaxCamNum)
		{
			//超过支持的最大通道数
			break;
		}
		sscanf(achValueBuf,"%s%d%d%d%d%d",achCameraName,&nRegisterStatus,&nMatrixId,&nVideoPort,
			&nType,&nCamAddr);
		tBasePu.SetNOString(achKeyBuf);
		tBasePu.SetObjName(achCameraName);
		tInItem.SetCam(tBasePu);
		
		tInItem.SetMatrixID(nMatrixId);
		tInItem.SetMatrixType(GetMatrixType(nMatrixId));
		tInItem.SetRegisterStatus(nRegisterStatus);
		tInItem.SetVideoInPort(nVideoPort);
		tInItem.SetCamAddr(nCamAddr);
		tInItem.SetCamType(nType);
		
		dwRealRegNum++;

		if (++dwRealSendNum < dwToTalNum)
		{
			cMonMsg.CatMsgBody(&tInItem,sizeof(TVideoInItem));
		}
		else
		{
			TTotalNum *ptToTalNum = (TTotalNum *)cMonMsg.GetMsgBody();
			ptToTalNum->SetTotalNum(dwRealSendNum-1);
			OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_CFGMGR_GET_BINDINRT_NTY, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
			
			dwRealSendNum = 1;
			tTotalNum.SetTotalNum(dwRealSendNum);
			cMonMsg.SetMsgBody(&tTotalNum, sizeof(TTotalNum));
			cMonMsg.CatMsgBody(&tInItem, sizeof(TVideoInItem));
		}
		bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
			achValueBuf,sizeof(achValueBuf));
	 }
	G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : total cam num is %d\n", dwRealRegNum);
	dwRealRegNum = 0;

	if (dwRealSendNum > 0 || dwRealRegNum == 0)
	{
		G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : Finish send videoin bind relationship data, last msg include %d msgbody!\n", dwRealSendNum);

		cMonMsg.SetReserved1((u32)1);

		TTotalNum *ptToTalNum = (TTotalNum *)cMonMsg.GetMsgBody();
		ptToTalNum->SetTotalNum(dwRealSendNum);
		OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_CFGMGR_GET_BINDINRT_NTY, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	}

	return TRUE;
}

/**************************************************************
函数名:     ReadVideoOutCfg
功能:       读取编码器配置
算法实现:	
参数说明:   
返回值说明: FALSE   失败
            TRUE    成功
----------------------------------------------------------------
修改记录:
日期           版本              修改人              修改内容
***************************************************************/
BOOL32 CCfgMgrAppData::ReadVideoOutCfg()
{
	u32 dwRealEncNum = 0;
	u32 dwCurTotalChannel = 0;
	s8 szBindRtFileName[MAXLEN_FILEPATH+64] = {0};

	s8 szEncoderName[MAXLEN_NAME + 1] = {0};
    s32 nVideoSrcPortNum = 0;
	s32 nConnMatrix;
	s32 nMatrixId;

	s8 achKeyBuf[128 + 1];
    s8 achValueBuf[128 + 1];
    BOOL32 bRet;
	TVideoOutItem tOutItem;

	u32 dwMaxEncoderNum = 0;
	if (g_cVcData.m_cLicenseHandler.IsLicenseValid())
	{
		dwMaxEncoderNum = g_cVcData.m_cLicenseHandler.GetMaxEncoderNum();
	}

	sprintf(szBindRtFileName,"%s%s",GetCfgPath(),CONFIG_FILE_DEVICES);

    bRet = GetNextLineString(szBindRtFileName,SECTION_ENCODERDEVICE,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    while (bRet)
    {
        
		sscanf(achValueBuf,"%s%d%d%d",szEncoderName,&nVideoSrcPortNum,&nConnMatrix,&nMatrixId);
        TBasePu tBasePu;
        tBasePu.SetNOString(achKeyBuf);
        tBasePu.SetObjName(szEncoderName);
        tOutItem.SetEncoder(tBasePu);
		dwCurTotalChannel += nVideoSrcPortNum;//当前通道数累加
		
		if (dwCurTotalChannel > dwMaxEncoderNum)
        {
			//达到license规定的最大通道数
			break;
        }
        OspPrintf(TRUE,FALSE, "读取编码器(%s)的配置\n",szEncoderName);

        tOutItem.SetConnMatrix(nConnMatrix);
        tOutItem.SetVideoSrcPortNum(nVideoSrcPortNum);
		tOutItem.SetMatrixID(nMatrixId);
        AddItem(tOutItem);
		dwRealEncNum++;

        bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
        achValueBuf,sizeof(achValueBuf));
    }
	
	//向VcSched发送输出绑定信息
	u32 dwRealSendNum = 0;
	TTotalNum tTotalNum;
	tTotalNum.SetTotalNum(dwRealSendNum);

	CMonMsg cMonMsg;
	cMonMsg.SetMsgBody(&tTotalNum, sizeof(TTotalNum));
	u32 dwToTalNum =  (g_dwMaxMsgLen - sizeof(TTotalNum)) / sizeof(TVideoOutItem);

	TVideoOutItem *ptItem = m_tItemList.PosFirst();
	for(; NULL != ptItem; ptItem = m_tItemList.PosNext(ptItem))
	{
		//在此设置编码器与矩阵的连接关系
        TBasePu& tBasePu = ptItem->GetEncoder();
        bRet = GetNextLineString(szBindRtFileName,tBasePu.GetNOString(),achKeyBuf,sizeof(achKeyBuf),
            achValueBuf,sizeof(achValueBuf));
        while (bRet)
        {
            ptItem->SetVideoOutPort(atoi(achKeyBuf),atoi(achValueBuf));
            bRet = GetNextLineString(NULL,NULL,achKeyBuf,sizeof(achKeyBuf),
            achValueBuf,sizeof(achValueBuf));
        }

        if (++dwRealSendNum < dwToTalNum)
		{
			cMonMsg.CatMsgBody(ptItem, sizeof(TVideoOutItem));
		}
		else
		{
			TTotalNum *ptTotalNum = (TTotalNum *)cMonMsg.GetMsgBody();
			ptTotalNum->SetTotalNum(dwRealSendNum-1);
			OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_CFGMGR_GET_BINDOUTRT_NTY, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());

			dwRealSendNum = 1;
			tTotalNum.SetTotalNum(dwRealSendNum);
			cMonMsg.SetMsgBody(&tTotalNum, sizeof(TTotalNum));
			cMonMsg.CatMsgBody(ptItem, sizeof(TVideoOutItem));
		}
	}

	if (dwRealSendNum > 0 || dwRealEncNum == 0)
	{
		G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : Finish send videoout bind relationship data, last msg include %d msgbody!\n", dwRealSendNum);
		
		cMonMsg.SetReserved1((u32)1);
		
		TTotalNum *ptToTalNum = (TTotalNum *)cMonMsg.GetMsgBody();
		ptToTalNum->SetTotalNum(dwRealSendNum);
		OspPost(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), EV_G200_CFGMGR_GET_BINDOUTRT_NTY, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	}

	ClearItemList();
	
	return TRUE;
}

BOOL32 CCfgMgrAppData::bRegSrvOnSvr()
{
	return m_bRegSrvOnSvr;
}

void CCfgMgrAppData::SetRegSrvOnSvr(BOOL32 bOnSvr)
{
	m_bRegSrvOnSvr = bOnSvr;
}

CCfgMgrIns::CCfgMgrIns()
{
	m_dwNodeId = INVALID_NODE;
	m_dwDstId = MAKEIID(INVALID_APP, INVALID_INS);
}

void CCfgMgrIns::SendMsgToVcSched(const CMonMsg &cMonMsg, u16 wEvent)
{
	post(MAKEIID(AID_G200_VCSCHED, CInstance::DAEMON), wEvent, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
}


void CCfgMgrIns::StartDevReg()
{
	
}

LPCSTR CCfgMgrIns::GetStrState()
{
	switch(CurState())
    {
    case IDLE:
        return "Idle";
    case SERVICE:
        return "Service";
    default:
        return "未知状态";
    }
}

void CCfgMgrIns::ConnToRegSrv()
{
	//向注册服务器建链，并在建链成功后发起注册请求，然后进入服务态
	u32 dwNode = (u32)OspConnectTcpNode(g_cVcData.m_tG200Cfg.GetSelfPlatIP(),
		                                g_cVcData.m_tG200Cfg.GetRegSrvListenPort(),
		                                10, 3, 15000);
	if (INVALID_NODE != dwNode)
	{
		m_dwNodeId = dwNode;
		OspNodeDiscCBReg(m_dwNodeId, m_appId, m_instId);
		
		RegisterToRegSrv();
	}
	else
	{
		SetTimer(EV_G200_CONN_REGSRV_TIMER, G200_CONN_REGSRV_INTV);
	}
}

void CCfgMgrIns::RegisterToRegSrv()
{
	G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : 向设备注册服务器进行注册！\n");
	TLicenseInfo tLicenseInfo = {0};
	tLicenseInfo.bValid = g_cVcData.m_cLicenseHandler.IsLicenseValid();
	tLicenseInfo.nMaxVideoNum = g_cVcData.m_cLicenseHandler.GetMaxChannel();
	tLicenseInfo.nMaxEncoderNum = g_cVcData.m_cLicenseHandler.GetMaxEncoderNum();

	CMonMsg cMonMsg;
	cMonMsg.SetMsgBody(&tLicenseInfo,sizeof(TLicenseInfo));
	post(MAKEIID(AID_G200MS_DEVMGR, CInstance::DAEMON), G200_G200MS_REG_REQ, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen(), m_dwNodeId);
	SetTimer(EV_G200_REG_REGSRV_TIMER, G200_REG_REGSRV_INTV);
}


void CCfgMgrIns::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
	if (pcMsg->event == EV_G200_SET_INST_ALIAS)
	{
        char szAlias[MAX_ALIAS_LEN]={0};
        strcpy(szAlias, "[G200/CfgMgr-Daemon]");
        pcApp->SetInstAlias(CInstance::DAEMON, szAlias, (u8)strlen(szAlias));

		return;
	}
    G200Log(LOG_CFG_MOD,LOGLVL_DEBUG2,"%s[%s]:Receive Message[%s]\n",m_alias,GetStrState(),OspEventDesc(pcMsg->event));
	if (OnCommonMsg(pcMsg))
	{
		return;
	}

	switch(CurState())
    {
    case IDLE:
        {
            OnIdle(pcMsg);
            break;
        }
    case SERVICE:
        {
            OnService(pcMsg);
            break;
        }
    default:
        {
            G200Log(LOG_CFG_MOD, LOGLVL_EXCEPTION, "%s[%s-%d]:未定义状态\n", m_alias, GetStrState(), CurState());
            break;
        }
    }
}

BOOL32 CCfgMgrIns::OnCommonMsg(const CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case EV_G200_CFGMGR_GET_BINDRT_CMD:
		{
			s8 abyPath[MAXLEN_FILEPATH+1] = {0};
			if (pcMsg->content == NULL || pcMsg->length == 0)
			{
				G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : 无效的矩阵配置文件的路径！\n");
				break;
			}
			
			CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
			u16 byMsgLen = sizeof(abyPath) < pcMonMsg->GetMsgBodyLen() ? sizeof(abyPath) : pcMonMsg->GetMsgBodyLen();
			pcMonMsg->GetMsgBody(abyPath, byMsgLen);
			g_cCfgMgrApp.ReadBindRtCfg(abyPath);
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CCfgMgrIns::OnIdle(const CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case EV_G200_CFGMGR_POWERON:
		{
			post(MAKEIID(m_appId,m_instId),EV_G200_CONN_REGSRV_TIMER);
		}
		break;
	case EV_G200_CONN_REGSRV_TIMER:
		{
			G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : 向设备注册服务器建链！\n");
			ConnToRegSrv();
		}
		break;
	case EV_G200_REG_REGSRV_TIMER:
		{
			if (g_cCfgMgrApp.bRegSrvOnSvr())
			{
				return;
			}

			RegisterToRegSrv();
		}
		break;
	case G200MS_G200_REG_ACK:
		{
			G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : 向设备注册服务器注册成功！\n");

			g_cCfgMgrApp.SetRegSrvOnSvr(TRUE);

			m_dwDstId = pcMsg->srcid;

			KillTimer(EV_G200_REG_REGSRV_TIMER);
			NextState(SERVICE);
		}
		break;
	case G200MS_G200_REG_NACK:
		{
			G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : 向设备注册服务器注册失败！\n");

			g_cCfgMgrApp.SetRegSrvOnSvr(FALSE);
		}
		break;
	case OSP_DISCONNECT:
		{
			KillTimer(EV_G200_REG_REGSRV_TIMER);
			m_dwNodeId = INVALID_NODE;
			m_dwDstId = MAKEIID(INVALID_APP, INVALID_INS);

			g_cCfgMgrApp.SetRegSrvOnSvr(FALSE);

			SetTimer(EV_G200_CONN_REGSRV_TIMER, G200_CONN_REGSRV_INTV);
		}
		break;
	default:
		{
			break;
		}
	}
}

void CCfgMgrIns::OnService(const CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case G200MS_G200_ADD_BINDINRT_NTY:
		{
			G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr] : 收到来自设备注册服务器添加绑定关系的通知消息！\n");

			CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
			SendMsgToVcSched(*pcMonMsg, EV_G200_CFGMGR_ADD_BINDINRT_NTY);
		}
		break;
	case OSP_DISCONNECT:
		{
			m_dwNodeId = INVALID_NODE;
			m_dwDstId = MAKEIID(INVALID_APP, INVALID_INS);

			g_cCfgMgrApp.SetRegSrvOnSvr(FALSE);
			
			NextState(IDLE);
			SetTimer(EV_G200_CONN_REGSRV_TIMER, G200_CONN_REGSRV_INTV);
		}
		break;
	default:
		{
            if (GETAPP(pcMsg->srcid) == AID_G200_VCSCHED)
            {
                //发送消息给g200ms
                post(m_dwDstId,pcMsg->event,pcMsg->content,pcMsg->length,m_dwNodeId);
                G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr-Daemon][%s]:向msg200发送消息[%s-%d]\n", 
                    GetStrState(),OspEventDesc(pcMsg->event),pcMsg->event);
            }
            else
            {
                //将msg200的消息转发给中心调度模块
                post(MAKEIID(AID_G200_VCSCHED,CInstance::DAEMON),pcMsg->event,pcMsg->content,pcMsg->length);
                G200Log(LOG_CFG_MOD, LOGLVL_DEBUG1, "[G200/CfgMgr-Daemon][%s] : 收到来自msg200的消息[%s-%d]\n", 
                    GetStrState(),OspEventDesc(pcMsg->event),pcMsg->event);
            }
		}
		break;
	}
}