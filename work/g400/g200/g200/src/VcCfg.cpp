#include "VcSsn.h"
#include "G200Data.h"
#include "CfgMgr.h"

extern CVcData g_cVcData;

BOOL32 CVcSchedIns::SendMsgToCfgMgr(const CMonMsg &cMonMsg, u16 wEvent)
{
	post(MAKEIID(AID_G200_CFG, CInstance::DAEMON), wEvent, cMonMsg.GetMonMsg(), cMonMsg.GetMonMsgLen());
	return TRUE;
}

void CVcSchedIns::ProcGetMatrixInfoNty(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;

	TTotalNum *ptTotalNum = (TTotalNum *)pcMonMsg->GetMsgBody();
	u32 dwNum = ptTotalNum->GetTotalNum();
	if (dwNum == 0)
	{
		return;
	}

	OspPrintf(TRUE, FALSE, "获取到%d台矩阵信息！\n", dwNum);
	TMatrixData tMatrixData;
	TMatrixItem *ptMatrixItem  = NULL;
	for(u32 dwIndex = 0; dwIndex < dwNum; dwIndex++)
	{
		ptMatrixItem = (TMatrixItem *)pcMonMsg->GetMsgBody(sizeof(TTotalNum) + dwIndex * sizeof(TMatrixItem));
		if (NULL != ptMatrixItem)
		{
			tMatrixData.SetMatrixID(ptMatrixItem->GetMatrixID());
            tMatrixData.SetMatrixName(ptMatrixItem->GetMatrixName());
			tMatrixData.SetMatrixType(ptMatrixItem->GetMatrixType());            
            tMatrixData.SetMatrixInportNum(ptMatrixItem->GetMatrixInportNum());
            tMatrixData.SetMatrixOutportNum(ptMatrixItem->GetMatrixOutportNum());
			g_cVcData.AddMatrixData(tMatrixData);
		}
	}
}

void CVcSchedIns::ProcGetBindInRtNty(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;

	TTotalNum *ptTotalNum = (TTotalNum *)pcMonMsg->GetMsgBody();
	u32 dwNum = ptTotalNum->GetTotalNum();
	if (dwNum > 0)
	{
		OspPrintf(TRUE, FALSE, "获取到%d台摄像头信息！\n", dwNum);
		TVideoInRelation tInRt;
		TVideoInItem *ptItem = NULL;
		for(u32 byIndex = 0; byIndex < dwNum; byIndex++)
		{
			ptItem = (TVideoInItem *)pcMonMsg->GetMsgBody(sizeof(TTotalNum) + byIndex * sizeof(TVideoInItem));
			if (NULL != ptItem)
			{
				tInRt.SetMatrixID(ptItem->GetMatrixID());
				tInRt.SetMatrixType(ptItem->GetMatrixType());
				tInRt.SetVideoInPort(ptItem->GetVideoInPort());
				tInRt.GetCameraData().GetPuStatus().SetKdmNO(ptItem->GetCam().GetKdmNo());
				tInRt.GetCameraData().SetDevName(ptItem->GetCam().GetObjName());
                tInRt.SetRegisterStatus(ptItem->GetRegisterStatus());
                tInRt.SetCamType(ptItem->GetCamType());
                tInRt.SetCamAddr(ptItem->GetCamAddr());
                
				g_cVcData.AddVideoInRT(tInRt);
			}
		}
	}
	
	if (pcMonMsg->GetReserved1() == 1)
	{
		OspPrintf(TRUE, FALSE, "完成摄像头信息的获取！\n");
		g_cVcSchedApp.SetHasFinishGetVideoInRt(TRUE);
		
		if (g_cVcSchedApp.HasFinishGetVideoOutRt())
		{
			NextState(STATE_SERVICE);
		}
	}
}

void CVcSchedIns::ProcGetBindOutRtNty(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;

	TTotalNum *ptTotalNum = (TTotalNum *)pcMonMsg->GetMsgBody();
	u32 dwNum = ptTotalNum->GetTotalNum();
	if (dwNum > 0)
	{
		OspPrintf(TRUE, FALSE, "获取到%d台编码器信息！\n", dwNum);
		TVideoOutRelation tOutRt;
		TVideoOutItem *ptItem = NULL;
		u8 byVidChnId = 0;
		for(u32 byIndex = 0; byIndex < dwNum; byIndex++)
		{
			ptItem = (TVideoOutItem *)pcMonMsg->GetMsgBody(sizeof(TTotalNum) + byIndex * sizeof(TVideoOutItem));
			if (NULL != ptItem)
			{
				memset(&tOutRt, 0, sizeof(tOutRt));
				tOutRt.SetMatrixID(ptItem->GetMatrixID());
				tOutRt.GetEncData().GetPuStatus().SetKdmNO(ptItem->GetEncoder().GetKdmNo());
                tOutRt.GetEncData().SetDevName(ptItem->GetEncoder().GetObjName());
				tOutRt.SetConnMatrix(ptItem->IsConnMatrix());
				tOutRt.SetVideoSrcPortNum(ptItem->GetVideoSrcPortNum());
				TEncVideoInputPortInfo tInfo;
				u16 wPort = 0;
				for(byVidChnId = 0; byVidChnId < MAX_PU_CHNNUM; byVidChnId++)
				{
					tInfo = tOutRt.GetEncVideoInputPortInfo(byVidChnId);
					wPort = ptItem->GetVideoOutPort(byVidChnId);
					if (wPort != 0)
					{
						tInfo.SetConnVideoOutPort(TRUE);
						tInfo.SetVideoOutPort(wPort);
						tOutRt.SetEncVideoInputPortInfo(byVidChnId, tInfo);
					}
				}
				
				g_cVcData.AddVideoOutRT(tOutRt.GetMatrixID(), tOutRt);
			}
		}//for
	}
	
	if (pcMonMsg->GetReserved1() == 1)
	{
		OspPrintf(TRUE, FALSE, "完成编码器信息的获取！\n");
		g_cVcSchedApp.SetHasFinishGetVideoOutRt(TRUE);
		
		if (g_cVcSchedApp.HasFinishGetVideoInRt())
		{
			NextState(STATE_SERVICE);
		}
	}
}

void CVcSchedIns::ProcAddBindInRtNty(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;
	
	TTotalNum *ptTotalNum = (TTotalNum *)pcMonMsg->GetMsgBody();
	u32 dwNum = ptTotalNum->GetTotalNum();
	if (dwNum == 0)
	{
		return;
	}
	
	OspPrintf(TRUE, FALSE, "添加%d条视频输入绑定信息！\n", dwNum);
	TVideoInRelation *ptInRt = NULL;
	TBasePu *ptItem = NULL;

	TMatrixData *ptMatrixData = NULL;
	for(u32 byIndex = 0; byIndex < dwNum; byIndex++)
	{
		ptItem = (TBasePu *)pcMonMsg->GetMsgBody(sizeof(TTotalNum) + byIndex * sizeof(TBasePu));
		if (NULL == ptItem)
		{
			continue;
		}
		if ((ptInRt = g_cVcData.GetVideoInRT(ptItem->GetKdmNo())) != NULL)
		{
			ptInRt->SetRegisterStatus(TRUE);

			ptMatrixData = g_cVcData.GetMatrixData(ptInRt->GetMatrixID());
			if (NULL != ptMatrixData)
			{
				if (ptMatrixData->GetOnlineEncNum() > 0)
			    {
				    NotifyOneCamStatus(ptItem->GetKdmNo(), TRUE);
			    }
			}		
		}
	}
}

void CVcSchedIns::ProcMsgFromCfgMgr(const CMessage * pcMsg)
{
	CMonMsg *pcMonMsg = (CMonMsg *)pcMsg->content;

	switch(pcMsg->event)
	{
	case EV_G200_CFGMGR_GET_MATRIX_NTY:
		{
			ProcGetMatrixInfoNty(pcMsg);
		}
		break;
	case EV_G200_CFGMGR_GET_BINDINRT_NTY:
		{
			ProcGetBindInRtNty(pcMsg);
		}
		break;
	case EV_G200_CFGMGR_GET_BINDOUTRT_NTY:
		{
			ProcGetBindOutRtNty(pcMsg);
		}
		break;
	default:
		{
			G200Log(LOG_SCHED_MOD, LOGLVL_DEBUG1, "[ERROR/VcCfg] : 收到无法处理的消息[%s-%d]\n",OspEventDesc(pcMsg->event),pcMsg->event);
			break;
		}
	}
}