#include "VcSsn.h"
#include "xmlmsg.h"
#include "cmsevent.h"
extern CVcData g_cVcData;

void CVcSchedIns::ProcMsgFromG200MS(const CMessage* pcMsg)
{
    CMonMsg* pcMonMsg = (CMonMsg*)pcMsg->content;
	switch (pcMsg->event)
    {
    case EV_HS_SERVER_KEDACOMXMLDATA:
        {
            OnDealData(pcMsg);
        }
        break;
    default:
        break;
    }
}


BOOL32 CVcSchedIns::SendMsgToG200MS(CXmlMsg &cXmlMsg, const CMessage *const pcMsg)
{
    s32 nLength = 0;
	const s8 * pchMessage = cXmlMsg.GetMsgStr(nLength);
    if (NULL == pchMessage)
    {
        G200Log(LOG_CLIENTCFG_MOD,LOGLVL_DEBUG2,"[G200/VcSched][%d]:Deal with Message Failure![%d]\n",GetStrState(),pcMsg->event);
    }

	CMonMsg* pcMonMsg = (CMonMsg*)pcMsg->content;
    
    CMonMsg cMonMsg;
    cMonMsg.SetSessID(pcMonMsg->GetSessID());
    cMonMsg.SetReserved1(pcMonMsg->GetReserved1());
    cMonMsg.SetMsgBody(pchMessage,nLength);

	G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG2,"%s[%s]:Send Message[%s-%d] to G200ms\n",m_alias,GetStrState(),
		OspEventDesc(cXmlMsg.GetCommand()),cXmlMsg.GetCommand());
	post(pcMsg->srcid, EV_HS_SERVER_KEDACOMXMLDATA, cMonMsg.GetMonMsg(),
                 cMonMsg.GetMonMsgLen(), pcMsg->srcnode); 
	return TRUE;
}

void CVcSchedIns::DeleteEncoder(TKDMNO &tKdmNo,s32 nCurMatrixId,TVideoOutRelation* ptVideoOutRt)
{
    OnPuDisconn(tKdmNo);
    g_cVcData.DelVideoOutRT(nCurMatrixId,*ptVideoOutRt);

    //强制pu断链
    CMonMsg cMonMsg;
    cMonMsg.SetDstID(tKdmNo);
    cMonMsg.SetMsgBody(&tKdmNo,sizeof(TKDMNO));
    SendMsgToPuiSsn(ptVideoOutRt->GetEncData().GetInstId(),CMU_PUI_PU_FORCE_DISCONNECT_CMD,
        cMonMsg.GetMonMsg(),cMonMsg.GetMonMsgLen());
}

void CVcSchedIns::OnDealData(const CMessage* pcMsg)
{
    CMonMsg* pcMonMsg = (CMonMsg*)pcMsg->content;
	s8 *pBuf = (s8 *)pcMonMsg->GetMsgBody();
	s8 * end = strstr((s8*)pBuf , "/kedacomxmldata");
	*(end + 16) = 0;

    CXmlMsg cXmlMsg(pBuf);
	G200Log(LOG_SCHED_MOD,LOGLVL_DEBUG2,"%s[%s]:Receive Message[%s-%d]\n",m_alias,GetStrState(),
		OspEventDesc(cXmlMsg.GetCommand()),cXmlMsg.GetCommand());
    switch (cXmlMsg.GetCommand())
    {
    case BS_G200MS_UPDATE_MATRIX_REQ:
        {
            cXmlMsg.Go2Content();
            cXmlMsg.GoDownstairs();

            s32 nMatrixId = cXmlMsg.GetNodeIntValue("index");

            s32 nMatrixType = cXmlMsg.GetNodeIntValue("type");

            const s8* pszMatrixName = cXmlMsg.GetNodeStringValue("name");

            TMatrixData* ptMatrixData = g_cVcData.GetMatrixData(nMatrixId);
            ptMatrixData->SetMatrixType(nMatrixType);
            ptMatrixData->SetMatrixName(pszMatrixName);
            
        }
        break;
    case BS_G200MS_DELETE_MATRIX_REQ:
        {
            cXmlMsg.Go2Content();
            cXmlMsg.GoDownstairs();

            s32 nMatrixId = cXmlMsg.GetNodeIntValue();
            g_cVcData.DelMatrixData(nMatrixId);

            //删除矩阵上对应的编码器和摄像头        
        }
        break;
    case BS_G200MS_DELETE_CAMERA_REQ:
        {
            cXmlMsg.Go2Content();
            cXmlMsg.GoDownstairs();
                      
            //如果当前设备正在被浏览的话
            const s8* pszCameraNo = cXmlMsg.GetNodeStringValue("guid");
            TKDMNO tKdmNo;
            tKdmNo.SetNOString(pszCameraNo);
            TVideoInRelation*  ptVideoInRT = g_cVcData.GetVideoInRT(tKdmNo);
            if (ptVideoInRT)
            {
                g_cVcData.DelVideoInRT(*ptVideoInRT);
                NotifyOneCamStatus(tKdmNo,FALSE);
            }
        }
        break;
    case BS_G200MS_UPDATE_CAMERA_REQ:
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

            TKDMNO tKdmNo;
            tKdmNo.SetNOString(pszCameraNo);
            TVideoInRelation*  ptVideoInRT = g_cVcData.GetVideoInRT(tKdmNo);
            ptVideoInRT->SetCamAddr(nCurCamAddr);
            ptVideoInRT->SetCamType(nCurType);
            ptVideoInRT->SetMatrixID(nCurMatrixId);
            ptVideoInRT->SetVideoInPort(nCurPort);
            ptVideoInRT->GetCameraData().GetPuStatus().SetNOString(pszCameraNo);
		    ptVideoInRT->GetCameraData().SetDevName(pszCameraName);
            
        }
        break;
    case BS_G200MS_DELETE_ENCODER_REQ:
        {
            cXmlMsg.Go2Content();
            cXmlMsg.GoDownstairs();

            s32 nMatrixId = cXmlMsg.GetNodeIntValue();

            cXmlMsg.NextSibling();
            const s8* pszEncoderNo = cXmlMsg.GetNodeStringValue();

            TKDMNO tKdmNo;
            tKdmNo.SetNOString(pszEncoderNo);
            TVideoOutRelation* ptVideoOutRt = g_cVcData.GetVideoOutRt(tKdmNo);
            if (ptVideoOutRt)
            {
                DeleteEncoder(tKdmNo,nMatrixId,ptVideoOutRt);
            }
        }
        break;
    case BS_G200MS_UPDATE_ENCODER_REQ:
        {
            cXmlMsg.Go2Content();
            cXmlMsg.GoDownstairs();

            s32 nCurMatrixId = cXmlMsg.GetNodeIntValue();

            cXmlMsg.NextSibling();
            s32 nDesMatrixId = cXmlMsg.GetNodeIntValue();

            cXmlMsg.NextSibling();
            const s8* pszEncoderNo = cXmlMsg.GetNodeStringValue();

            cXmlMsg.NextSibling();
            const s8* pszEncoderName = cXmlMsg.GetNodeStringValue();

            cXmlMsg.NextSibling();
            BOOL32 bLinkStatus = cXmlMsg.GetNodeIntValue();
            
            TKDMNO tKdmNo;
            tKdmNo.SetNOString(pszEncoderNo);
            TVideoOutRelation* ptVideoOutRt = g_cVcData.GetVideoOutRt(tKdmNo);
            ptVideoOutRt->SetConnMatrix(bLinkStatus);
            ptVideoOutRt->GetEncData().SetDevName(pszEncoderName);
            ptVideoOutRt->SetMatrixID(nDesMatrixId);
        }
        break;
	case BS_G200MS_G200_SYSMAIN_REQ:
		{
			CXmlMsg cRetMsg;
			cRetMsg.SetCommand(G200_G200MS_BS_SYSMAIN_ACK);
			cRetMsg.Go2Content();
			cRetMsg.BuildNode("verinfo","%s CompileTime:%s %s",G200_CONFVERSION,__DATE__,__TIME__);
			cRetMsg.BuildNode("licenseinfo","%d",g_cVcData.m_cLicenseHandler.IsLicenseValid());
			cRetMsg.BuildNode("mchkey","%s",g_cVcData.m_cLicenseHandler.GetMacInfo());
			cRetMsg.BuildNode("maxdate","%s",g_cVcData.m_cLicenseHandler.GetMaxDate());
			cRetMsg.BuildNode("approval","%s",g_cVcData.m_cLicenseHandler.GetApprovalNo());
			cRetMsg.BuildNode("videonum","%d",g_cVcData.m_cLicenseHandler.GetMaxChannel());
			cRetMsg.BuildNode("encodernum","%d",g_cVcData.m_cLicenseHandler.GetMaxEncoderNum());
			SendMsgToG200MS(cRetMsg,pcMsg);
		}
		break;
	case BS_G200MS_G200_MPLATSTATUS_REQ:
		{
			CXmlMsg cRetMsg;
			cRetMsg.SetCommand(G200_G200MS_BS_MPLATSTATUS_ACK);
			cRetMsg.Go2Content();
			cRetMsg.BuildNode("mplatstatus","%d",g_cVcData.GetConnMplat());
			SendMsgToG200MS(cRetMsg,pcMsg);
		}
		break;
	case BS_G200MS_G200_DEVICESTATUS_REQ:
		{
			cXmlMsg.Go2Content();
            cXmlMsg.GoDownstairs();
			s32 nMatrixId = cXmlMsg.GetNodeIntValue("matrixid");

			CXmlMsg cRetMsg;
			cRetMsg.SetCommand(G200_G200MS_BS_DEVICESTATUS_ACK);
			cRetMsg.Go2Content();
			cRetMsg.BuildNode("encoderlist");
			cRetMsg.GoDownstairs();
			TVideoOutRTList* ptEncoderList = g_cVcData.GetVideoOutRTListPtr(nMatrixId);
			TKDMNO tKdmNoSerial;
			BOOL32 bFirst = FALSE;
			if (ptEncoderList != NULL && ptEncoderList->GetItemCount() > 0)
			{
				TVideoOutRelation* ptEncoder = ptEncoderList->PosFirst();
				for(;ptEncoder!=NULL;ptEncoder = ptEncoderList->PosNext(ptEncoder))
				{
					TEncData tEncData = ptEncoder->GetEncData();
					BOOL32 bStatus = tEncData.GetPuStatus().IsOnLine();

					//查找发送控制命令的编码器
					if (bStatus && ptEncoder->IsConnMatrix() && !bFirst)
					{
						tKdmNoSerial = tEncData.GetPuStatus().GetKdmNo();
						bFirst = TRUE;
					}

					cRetMsg.BuildNode("encoder");
					cRetMsg.GoDownstairs();
					cRetMsg.BuildNode("guid","%s",tEncData.GetPuStatus().GetNOString());
					cRetMsg.BuildNode("status","%d",tEncData.GetPuStatus().IsOnLine());
					cRetMsg.GoUpstairs();
				}
			}
			cRetMsg.GoUpstairs();

			//通过此串口发送控制命令
			cRetMsg.BuildNode("serial","%s",tKdmNoSerial.GetNOString());
			SendMsgToG200MS(cRetMsg,pcMsg);
		}
		break;
    default:
        break;
    }
}