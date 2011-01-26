#include "G200Data.h"


#define CHECK_ADD_ITEM_RET(data, errstring) \
    if(data == NULL) {G200Log(LOG_MAIN_MOD, LOGLVL_EXCEPTION, errstring); return;}

#define CHECK_ADD_ITEM_RETNULL(data, errstring) \
    if(data == NULL) {G200Log(LOG_MAIN_MOD, LOGLVL_EXCEPTION, errstring); return NULL;}


TPuiData::TPuiData()
{
	memset(this, 0, sizeof(TPuiData));
}

void TPuiData::PrintData(u8 byOffsetSize) const
{
	OspPrintf(TRUE, FALSE, "%sPui Id:\t%s\n", GetOffset(byOffsetSize), m_tBasePui.GetKdmNo().GetNOString());
    OspPrintf(TRUE, FALSE, "%sPui Name:\t%s\n", GetOffset(byOffsetSize), m_tBasePui.GetObjName());

	s8 szIp[20];
	IPtoString(m_atDevIp[0].GetIpAddr(), szIp, sizeof(szIp));
	u16 dwPort;
	if (0==m_atDevIp[0].GetPort() && 0!=m_atDevIp[0].GetIpAddr())
	{
		dwPort = 5510;
	}
	else
	{
		dwPort = m_atDevIp[0].GetPort();
	}
	OspPrintf(TRUE, FALSE, "%sIp:\t%s : %d\n", GetOffset(byOffsetSize), szIp, dwPort);
    OspPrintf(TRUE, FALSE, "%s在线:\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsConnected()));
    OspPrintf(TRUE, FALSE, "%s启用:\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsUsed()));
    OspPrintf(TRUE, FALSE, "%s退网:\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsDel()));
    OspPrintf(TRUE, FALSE, "%s实例Id:\t%d\n", GetOffset(byOffsetSize), GetInstId());
	
    TKDMTime tConnTime(GetRegTime());
    char szConnTime[64];
    tConnTime.GetString(szConnTime, 64);
    OspPrintf(TRUE, FALSE, "%s注册时间:\t%s\n", GetOffset(byOffsetSize), szConnTime);
	OspPrintf(TRUE, FALSE, "%s最大前端:\t%d\n", GetOffset(byOffsetSize), m_dwMaxPuNum);
	OspPrintf(TRUE, FALSE, "%s上线前端:\t%d\n", GetOffset(byOffsetSize), m_dwCurPuNum);
}

void TPuiData::SetConnected(BOOL32 bConnected)
{
	if(bConnected)
    {
        m_byDevStat |= (0x1 << 0);
        SetRegTime();
    }
    else
    {
        m_byDevStat &= ~(0x1 << 0);
    }
}

void TPuiData::SetInstId(u16 wInstId)
{
	m_wInstId = wInstId;
}

void TPuiData::SetBasePui(const TBasePui& tBasePui)
{
	m_tBasePui = tBasePui;
}
const TBasePui& TPuiData::GetBasePui(void) const
{
	return m_tBasePui;
}

TBasePui& TPuiData::GetBasePui(void)
{
	return m_tBasePui;
}

BOOL32 TPuiData::IsConnected(void)const
{
	return ((m_byDevStat & (0x1 << 0)) != 0);
}
u16 TPuiData::GetInstId(void) const
{
	return m_wInstId;
}

void TPuiData::SetUsed(BOOL32 bUsed)
{
	if(bUsed)
    {
        m_byDevStat |= (0x1 << 1);
    }
    else
    {
        m_byDevStat &= ~(0x1 << 1);
    }
}

BOOL32 TPuiData::IsUsed(void)const
{
	return ((m_byDevStat & (0x1 << 1)) != 0);
}

void TPuiData::SetDel(BOOL32 bDel)
{
	if(bDel)
    {
        m_byDevStat |= (0x1 << 2);
    }
    else
    {
        m_byDevStat &= ~(0x1 << 2);
    }
}

BOOL32 TPuiData::IsDel(void)const
{
	return ((m_byDevStat & (0x1 << 2)) != 0);
}

void TPuiData::SetSure(BOOL32 bSure)
{
	if(bSure)
    {
        m_byDevStat |= (0x1 << 3);
    }
    else
    {
        m_byDevStat &= ~(0x1 << 3);
    }
}

BOOL32 TPuiData::IsSure(void)const
{
	return ((m_byDevStat & (0x1 << 3)) != 0);
}

void TPuiData::SetRegTime()
{
	time((time_t*)&m_nRegTime);
}

s32 TPuiData::GetRegTime() const
{
	return m_nRegTime;
}

void TPuiData::SetCurPuNum(u32 dwNum)
{
	m_dwCurPuNum = dwNum;
}

u32  TPuiData::GetCurPuNum() const
{
	return m_dwCurPuNum;
}

void TPuiData::AddCurPuNum()
{
	m_dwCurPuNum++;
}

void TPuiData::DelCurPuNum()
{
	m_dwCurPuNum = m_dwCurPuNum > 0 ? (m_dwCurPuNum - 1) : 0;
}

void TPuiData::SetMaxPuNum(u32 dwNum)
{
	m_dwMaxPuNum = dwNum;
}

u32  TPuiData::GetMaxPuNum() const
{
	return m_dwMaxPuNum;
}

void TPuiData::SetIpNum(u8 byNum)
{
	m_byIpNum = byNum;
}

u8 TPuiData::GetIpNum() const
{
	return m_byIpNum;
}

void TPuiData::SetIpAddr(u8 byIndex, const TIpAddr& tIpAddr)
{
	m_atDevIp[byIndex] = tIpAddr;
}

const TIpAddr& TPuiData::GetIpAddr(u8 byIndex) const
{
	if (byIndex < IP_NUM)
		return m_atDevIp[byIndex];

	return m_atDevIp[0];
}


TVideoSrcInfo::TVideoSrcInfo()
{
	memset(this, 0, sizeof(TVideoSrcInfo));
}

void TVideoSrcInfo::SetCamType(const u8 byCamType)
{
	m_byCamType = byCamType;
}

u8 TVideoSrcInfo::GetCamType(void)
{
	return m_byCamType;
}

void TVideoSrcInfo::SetCamId(const u16 wCamId)
{
	m_wCamId = wCamId;
}

u16 TVideoSrcInfo::GetCamId()
{
	return m_wCamId;
}

void TVideoSrcInfo::SetCamParseWay(const u8 byCamParseWay)
{
	m_byCamParseWay = byCamParseWay;
}

u8 TVideoSrcInfo::GetCamParseWay()
{
	return m_byCamParseWay;
}

void TVideoSrcInfo::SetCamCtrlWay(const u8 byCamCtrlWay)
{
	m_byCamCtrlWay = byCamCtrlWay;
}

u8 TVideoSrcInfo::GetCamCtrlWay()
{
	return m_byCamCtrlWay;
}

void TVideoSrcInfo::SetCamAutoRun(const u8 byCamAutoRun)
{
	m_byCamAutoRun = byCamAutoRun;
}

u8 TVideoSrcInfo::GetCamAutoRun()
{
	return m_byCamAutoRun;
}

void TVideoSrcInfo::SetTransType(const u8 byTransType)
{
	m_byTransType = byTransType;
}

u8 TVideoSrcInfo::GetTransType()
{
	return m_byTransType;
}

LPCSTR TVideoSrcInfo::GetVideoSrcAlias() const
{
	return szVideoSrcAlias;
}
//设置指定视频源别名
void TVideoSrcInfo::SetVideoSrcAlias(LPCSTR lpszSrcVideoChnAlias)
{
	strncpy(szVideoSrcAlias, lpszSrcVideoChnAlias, sizeof(szVideoSrcAlias) - 1); 
	szVideoSrcAlias[sizeof(szVideoSrcAlias) - 1] = 0;
}

TMatrixData::TMatrixData()
{
	memset(this, 0, sizeof(TMatrixData));
	
	m_bNeedNtyCamStatus = TRUE;
}
void TMatrixData::SetMatrixName(const s8* pszMatrixName)
{
    if (pszMatrixName == NULL)
    {
        return;
    }
    strncpy(m_achMatrixName,pszMatrixName,MAXLEN_NAME);
    m_achMatrixName[MAXLEN_NAME] = '\0';
}

const s8* TMatrixData::GetMatrixName()
{
    return m_achMatrixName;
}

void TMatrixData::SetMatrixInportNum(s32 nPortNum)
{
    m_nMatrixInportNum = nPortNum;
}
s32 TMatrixData::GetMatrixInportNum()
{
    return m_nMatrixInportNum;
}

void TMatrixData::SetMatrixOutportNum(s32 nPortNum)
{
    m_nMatrixOutPortNum = nPortNum;
}
s32 TMatrixData::GetMatrixOutportNum()
{
    return m_nMatrixOutPortNum;
}

u32 TMatrixData::GetMatrixID()
{
	return m_dwMatrixID;
}

void TMatrixData::SetMatrixID(const u32 dwMatrixID)
{
	m_dwMatrixID = dwMatrixID;
}

u8 TMatrixData::GetMatrixType()
{
	return m_byMatrixType;
}

void TMatrixData::SetMatrixType(const u8 byMatrixType)
{
	m_byMatrixType = byMatrixType;
}

BOOL32 TMatrixData::IsNeedNtyCamStatus()
{
	return m_bNeedNtyCamStatus;
}

void TMatrixData::SetNeedNtyCamStatus(BOOL32 bNeed)
{
	m_bNeedNtyCamStatus = bNeed;
}

u16 TMatrixData::GetOnlineEncNum()
{
	return m_wOnlineEncNum;
}

void TMatrixData::SetOnlineEncNum(const u16 wNum)
{
	m_wOnlineEncNum = wNum;
}

void TMatrixData::AddOnlineEncNum()
{
	m_wOnlineEncNum++;
}

void TMatrixData::SubOnlineEncNum()
{
	if (m_wOnlineEncNum > 0)
	{
		m_wOnlineEncNum--;
	}
	else
	{
		m_wOnlineEncNum = 0;
		SetNeedNtyCamStatus(TRUE);
	}
}

TCameraData::TCameraData()
{
    memset(this,0,sizeof(TCameraData));
    m_tPuStatus.SetUsed(TRUE);
    m_tPuStatus.SetAlarmInNum(0);
    m_tPuStatus.SetAlarmOutNum(0);
    m_tPuStatus.SetAudioDecNum(0);
    m_tPuStatus.SetCapItemNum(0);
    m_tPuStatus.SetDirectTransNum(0);
    m_tPuStatus.SetSerialPortNum(0);
    m_tPuStatus.SetVideoEncNum(1);
	m_tPuStatus.SetVideoDecNum(1);
	m_tPuStatus.SetAudioEncNum(1);
	m_tPuStatus.SetAudioDecNum(1);
    m_tPuStatus.SetVideoInPortNum(1);
	
    m_tPuStatus.SetVidSrcChn(0,0);
    m_tPuStatus.SetVideoDecNum(0);

    TVideoEncInputPortParam tParam;
	tParam.SetValidVideoInputPortNum(1);
	tParam.SetValidVideoInputPort(0, 0);
	tParam.SetUsedVideoInputPortNum(1);
	tParam.SetUsedVideoInputPort(0, 0);
    m_tPuStatus.SetVideoEncInputPortParam(0,tParam);
}

void TCameraData::PrintData(u8 byOffsetSize)
{
	OspPrintf(TRUE, FALSE, "%sCam Id       :\t%s\n", GetOffset(byOffsetSize), m_tPuStatus.GetKdmNo().GetNOString());
	
    OspPrintf(TRUE, FALSE, "%s启用            :\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsUsed()));
    OspPrintf(TRUE, FALSE, "%s退网            :\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsDel()));
    OspPrintf(TRUE, FALSE, "%s与3a同步        :\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsSure()));
 
    OspPrintf(TRUE, FALSE, "%sPu状态\n", GetOffset(byOffsetSize));

    GetPuStatus().PrintData(byOffsetSize + 1);
}

void TCameraData::SetPuStatus(const TPuStatus& tPuStatus)
{
	m_tPuStatus = tPuStatus;
}

/*
功能:重置pustatus的内部状态
*/
void TCameraData::SetPuOnline(BOOL32 bIsOnline)
{
	TPuStatus tTempPuStatus;
    tTempPuStatus.SetKdmNO(m_tPuStatus.GetKdmNo());
	u8 byIndex;
	for (byIndex = 0; byIndex < MAX_PU_CHNNUM; byIndex++)
	{
		tTempPuStatus.SetSrcVideoChnAlias(byIndex, m_tPuStatus.GetSrcVideoChnAlias(byIndex));
		tTempPuStatus.SetVideoCamId(byIndex, m_tPuStatus.GetVideoCamId(byIndex));
	}
	
    tTempPuStatus.SetUsed(m_tPuStatus.IsUsed());
	
    SetPuStatus(tTempPuStatus);
}

TPuStatus* TCameraData::GetPuStatusPtr()
{
	return &m_tPuStatus;
}

TPuStatus& TCameraData::GetPuStatus(void)
{
	return m_tPuStatus;
}

TVideoSrcInfo& TCameraData::GetVideoSrcInfo()
{
	return m_tVideoSrcInfo;
}

void TCameraData::SetVideoSrcInfo(const TVideoSrcInfo& tVideoSrcInfo)
{
	m_tVideoSrcInfo = tVideoSrcInfo;
}

BOOL32 TCameraData::IsUsed() const
{
	return m_tPuStatus.IsUsed();
}

void TCameraData::SetUsed(BOOL32 bIsUsed)
{
	m_tPuStatus.SetUsed(bIsUsed);
}

void TCameraData::SetDel(BOOL32 bDel)
{
	if(bDel)
    {
        m_byDevStat |= (0x1 << 0);
    }
    else
    {
        m_byDevStat &= ~(0x1 << 0);
    }
}
BOOL32 TCameraData::IsDel(void)const
{
	return ((m_byDevStat & (0x1 << 0)) != 0);
}

BOOL32 TCameraData::IsStop(s32 dwTime) const
{
	return FALSE;
}

BOOL32 TCameraData::IsOnLine(void) const
{
	return m_tPuStatus.IsOnLine();
}

void TCameraData::SetSure(BOOL32 bSure)
{
	if(bSure)
    {
        m_byDevStat |= (0x1 << 1);
    }
    else
    {
        m_byDevStat &= ~(0x1 << 1);
    }
}

BOOL32 TCameraData::IsSure(void)const
{
	return ((m_byDevStat & (0x1 << 1)) != 0);	
}

void TCameraData::SetDevName(LPCSTR lpszname)
{
    if (NULL == lpszname)
    {
        return;
    }
    strncpy(m_szDevName,lpszname,sizeof(m_szDevName) - 1);
    m_szDevName[sizeof(m_szDevName) - 1] = 0;
}
LPCSTR TCameraData::GetDevName()
{
    return m_szDevName;
}
TEncData::TEncData()
{
	memset(this, 0, sizeof(TEncData));
}


void TEncData::PrintData(u8 byOffsetSize)
{
	OspPrintf(TRUE, FALSE, "%sPu Id       :\t%s\n", GetOffset(byOffsetSize), m_tPuStatus.GetKdmNo().GetNOString());
    OspPrintf(TRUE, FALSE, "%sManuFacturer:\t%s\n", GetOffset(byOffsetSize), m_szManuFac);
	
    char szIP[MAX_IP_ADDR + 1] = {0};
    ::IPtoString(GetIpAddr(), szIP, sizeof(szIP));
    OspPrintf(TRUE, FALSE, "%sPu IP           :\t%s\n", GetOffset(byOffsetSize), szIP);
    OspPrintf(TRUE, FALSE, "%sPu start port   :\t%d\n", GetOffset(byOffsetSize), GetStartPort());
    OspPrintf(TRUE, FALSE, "%s实例号          :\t%d\n", GetOffset(byOffsetSize), GetInstId());
    OspPrintf(TRUE, FALSE, "%s启用            :\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsUsed()));
    OspPrintf(TRUE, FALSE, "%s退网            :\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsDel()));
    OspPrintf(TRUE, FALSE, "%s获取Cap         :\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsHasGetCap()));
    OspPrintf(TRUE, FALSE, "%s获取状态        :\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsHasGetStatus()));
    OspPrintf(TRUE, FALSE, "%s与3a同步        :\t%s\n", GetOffset(byOffsetSize), GetStrBool(IsSure()));
    OspPrintf(TRUE, FALSE, "%s监控点数        :\t%d\n", GetOffset(byOffsetSize), GetCamNum());

    OspPrintf(TRUE, FALSE, "%sPu状态\n", GetOffset(byOffsetSize));
    memset(szIP, 0, sizeof(szIP));
    ::IPtoString( GetPuiIpAddr(), szIP ,sizeof(szIP));
    OspPrintf(TRUE, FALSE, "%s登陆的Pui的Ip为:\t%s\n", GetOffset(byOffsetSize), szIP);
  
    GetPuStatus().PrintData(byOffsetSize + 1);
}

void TEncData::SetPuStatus(const TPuStatus& tPuStatus)
{
	m_tPuStatus = tPuStatus;
}

void TEncData::SetPuOnline(BOOL32 bIsOnline)
{
	TPuStatus tTempPuStatus;
    tTempPuStatus.SetKdmNO(m_tPuStatus.GetKdmNo());
	u8 byIndex;
	for (byIndex = 0; byIndex < MAX_PU_CHNNUM; byIndex++)
	{
		tTempPuStatus.SetSrcVideoChnAlias(byIndex, m_tPuStatus.GetSrcVideoChnAlias(byIndex));
	}
	
    tTempPuStatus.SetUsed(m_tPuStatus.IsUsed());
	
    SetPuStatus(tTempPuStatus);
}

TPuStatus* TEncData::GetPuStatusPtr()
{
	return &m_tPuStatus;
}

TPuStatus& TEncData::GetPuStatus(void)
{
	return m_tPuStatus;
}

void TEncData::SetBasePuVideoChnInfo(const TBasePuVideoChnInfo& tBasePuVideoChnInfo)
{
	m_tBasePuVideoChnInfo = tBasePuVideoChnInfo;
}

const TBasePuVideoChnInfo& TEncData::GetBasePuVideoChnInfo(void) const
{
	return m_tBasePuVideoChnInfo;
}

void TEncData::ParsePuVideoChnInfo()
{
	const TBasePuVideoChnInfo& tBasePuVideoChnInfo = m_tBasePuVideoChnInfo;
    TPuStatus& tPuStatus = m_tPuStatus;
	
    u8 byVidCnt;
    u8 byVidIndex;
    for(u8 byEncChn = 0;byEncChn<min(MAX_PU_CHNNUM, tPuStatus.GetVideoEncNum());byEncChn++ )
    {
        TVideoEncInputPortParam& tVideoEncInputPortParam = tPuStatus.GetVideoEncInputPortParam(byEncChn);
		u8 byValidVidNum = tVideoEncInputPortParam.GetValidVideoInputPortNum();
        for(byVidIndex=0, byVidCnt=0;byVidIndex<min(MAX_PU_CHNNUM, byValidVidNum);byVidIndex++ )
        {
            if(tBasePuVideoChnInfo.IsSrcVideoUsed(tVideoEncInputPortParam.GetValidVideoInputPort(byVidIndex)))
            {
                tVideoEncInputPortParam.SetUsedVideoInputPort(byVidCnt, tVideoEncInputPortParam.GetValidVideoInputPort(byVidIndex));
				byVidCnt++;
            }
        }
        tVideoEncInputPortParam.SetUsedVideoInputPortNum(byVidCnt);
    }
    for(byVidIndex = 0;byVidIndex<MAX_PU_CHNNUM;byVidIndex++ )
    {
        tPuStatus.SetVidCamType(byVidIndex, tBasePuVideoChnInfo.GetSrcVideoCamType(byVidIndex));
		tPuStatus.SetVideoCamId(byVidIndex, (u8)tBasePuVideoChnInfo.GetSrcVideoCamId(byVidIndex));
    }
}

BOOL32 TEncData::IsUsed() const
{
	return m_tPuStatus.IsUsed();
}

void TEncData::SetUsed(const BOOL32 bIsUsed)
{
	m_tPuStatus.SetUsed(bIsUsed);
}

u32 TEncData::GetIpAddr() const
{
	return m_dwIpAddr;
}

void TEncData::SetIpAddr(const u32 dwIpAddr)
{
	m_dwIpAddr = dwIpAddr;
}

u16 TEncData::GetInstId(void) const
{
	return m_wInstId;
}

void TEncData::SetInstId(const u16 wInstId)
{
	m_wInstId = wInstId;
}

void TEncData::SetDel(const BOOL32 bDel)
{
	if(bDel)
    {
        m_byDevStat |= (0x1 << 2);
    }
    else
    {
        m_byDevStat &= ~(0x1 << 2);
    }
}

BOOL32 TEncData::IsDel(void)const
{
	return ((m_byDevStat & (0x1 << 2)) != 0);
}
void TEncData::SetSwitchType(const u8 bySwitchType)
{
	m_bySwitchType = bySwitchType;
}

u8 TEncData::GetSwitchType(void)const
{
	return m_bySwitchType;
}

void TEncData::SetTransportAddr(const TTransportAddr& tTransportAddr)
{
	m_tTransportAddr = tTransportAddr;
}

const TTransportAddr&  TEncData::GetTransportAddr(void)const
{
	return m_tTransportAddr;
}

u16 TEncData::GetStartPort()const
{
	return ntohs(m_wStartPort);
}

void TEncData::SetStartPort(u16 wStartPort)
{
	m_wStartPort = htons(wStartPort);
}

BOOL32 TEncData::IsOnLine(void) const
{
	return m_tPuStatus.IsOnLine();
}

BOOL32 TEncData::IsHasGetCap() const
{
	return ((m_byDevStat & 0x1) != 0);
}

void TEncData::HasGetCap(const BOOL32 bIsHasGetCap)
{
	if(bIsHasGetCap)
    {
        m_byDevStat |= (0x1);
    }
    else
    {
        m_byDevStat &= ~(0x1);
    }
}

BOOL32 TEncData::IsHasGetStatus() const
{
	return ((m_byDevStat & (0x1 << 1)) != 0);
}

void TEncData::HasGetStatus(const BOOL32 bIsHasGetStatus)
{
	if(bIsHasGetStatus)
    {
        m_byDevStat |= (0x1 << 1);
    }
    else
    {
        m_byDevStat &= ~(0x1 << 1);
    }
}

void TEncData::SetSure(const BOOL32 bSure)
{
	if(bSure)
    {
        m_byDevStat |= (0x1 << 3);
    }
    else
    {
        m_byDevStat &= ~(0x1 << 3);
    }
}

BOOL32 TEncData::IsSure(void)const
{
	return ((m_byDevStat & (0x1 << 3)) != 0);
}

BOOL32 TEncData::IsCanUse() const
{
	if( TRUE == IsOnLine() &&
        TRUE == IsHasGetCap() &&
        TRUE == IsHasGetStatus() )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void TEncData::SetCamNum(const u16 wCamNum)
{
	m_wCamNum = wCamNum;
}

u16 TEncData::GetCamNum() const
{
	return m_wCamNum;
}

void TEncData::SetManuFac(LPCSTR szManuFac)
{
	strncpy(m_szManuFac, szManuFac, MAXLEN_DEVICE_MANUFACTURER);
	m_szManuFac[MAXLEN_DEVICE_MANUFACTURER] = 0;
}

LPCSTR TEncData::GetManuFac() const
{
	return m_szManuFac;
}

u32 TEncData::GetPuiIpAddr() const
{
	return m_dwPuiAddr;
}

void TEncData::SetPuiIpAddr(const u32 dwIpAddr)
{
	m_dwPuiAddr = dwIpAddr;
}

void TEncData::SetDevName(const s8* lpszname)
{
    if (NULL == lpszname)
    {
        return;
    }
    strncpy(m_szDevName,lpszname,sizeof(m_szDevName) - 1);
    m_szDevName[sizeof(m_szDevName) - 1] = 0;
}
const s8* TEncData::GetDevName()
{
    return m_szDevName;
}

TVideoInRelation::TVideoInRelation()
{

}

u32 TVideoInRelation::GetMatrixID()
{
	return m_dwMatrixID;
}

void TVideoInRelation::SetMatrixID(const u32 dwMatrixID)
{
	m_dwMatrixID = dwMatrixID;
}

u8 TVideoInRelation::GetMatrixType()
{
	return m_byMatrixType;
}

void TVideoInRelation::SetMatrixType(const u8 byMatrixType)
{
	m_byMatrixType = byMatrixType;
}

u16 TVideoInRelation::GetVideoInPort()
{
	return m_wVideoInPort;
}

void TVideoInRelation::SetVideoInPort(const u16 wVideoInPort)
{
	m_wVideoInPort = wVideoInPort;
}

TCameraData& TVideoInRelation::GetCameraData()
{
	return m_tCamData;
}

void TVideoInRelation::SetCameraData(const TCameraData& tCamData)
{
	memcpy(&m_tCamData, &tCamData, sizeof(TCameraData));
}
void TVideoInRelation::SetRegisterStatus(BOOL32 bRegiste)
{
    m_bRegiste = bRegiste;
}
BOOL32 TVideoInRelation::GetRegisterStatus()
{
    return m_bRegiste;
}

void TVideoInRelation::SetCamAddr(s32 nCamAddr)
{
    m_nCamAddr = nCamAddr;
}
s32 TVideoInRelation::GetCamAddr()
{
    return m_nCamAddr;
}

void TVideoInRelation::SetCamType(s32 nType)
{
    m_nCamType = nType;
}
s32 TVideoInRelation::GetCamType()
{
    return m_nCamType;
}


TEncVideoInputPortInfo::TEncVideoInputPortInfo()
{
	memset(this, 0, sizeof(TEncVideoInputPortInfo));
}

BOOL32 TEncVideoInputPortInfo::IsConnVideoOutPort()
{
	return m_bIsConnVideoOutPort;
}

void TEncVideoInputPortInfo::SetConnVideoOutPort(BOOL32 bConn)
{
	m_bIsConnVideoOutPort = bConn;
}

BOOL32 TEncVideoInputPortInfo::IsUsed()
{
	return m_bIsUsed;
}

void TEncVideoInputPortInfo::SetUsed(BOOL32 bUsed)
{
	m_bIsUsed = bUsed;
}

u16 TEncVideoInputPortInfo::GetCurMatrixInputPort()
{
	return m_wCurMatrixInPutPort;
}

void TEncVideoInputPortInfo::SetCurMatrixInputPort(const u16 wCurInputPort)
{
	m_wCurMatrixInPutPort = wCurInputPort;
}

u16 TEncVideoInputPortInfo::GetVideoOutPort()
{
	return m_wVideoOutPort;
}

void TEncVideoInputPortInfo::SetVideoOutPort(const u16 wPort)
{
	m_wVideoOutPort = wPort;
}


TVideoOutRelation::TVideoOutRelation()
{
	memset(this, 0, sizeof(TVideoOutRelation));
}


u32 TVideoOutRelation::GetMatrixID()
{
	return m_dwMatrixID;
}

void TVideoOutRelation::SetMatrixID(const u32 dwMatrixID)
{
	m_dwMatrixID = dwMatrixID;
}

BOOL32 TVideoOutRelation::IsConnMatrix()
{
	return m_bIsConnMatrix;
}
void TVideoOutRelation::SetConnMatrix(BOOL32 bConnMatrix)
{
	m_bIsConnMatrix = bConnMatrix;
}

TEncVideoInputPortInfo& TVideoOutRelation::GetEncVideoInputPortInfo(const u8 byVidChnId)
{
	if (byVidChnId >= MAX_PU_CHNNUM)
	{
		return m_atEncVideoInputPortInfo[byVidChnId-1];
	}

	return m_atEncVideoInputPortInfo[byVidChnId];
}

void TVideoOutRelation::SetEncVideoInputPortInfo(const u8 byVidChnId, const TEncVideoInputPortInfo& tInfo)
{
	if (byVidChnId >= MAX_PU_CHNNUM)
	{
		return;
	}

	memcpy(&m_atEncVideoInputPortInfo[byVidChnId], &tInfo, sizeof(TEncVideoInputPortInfo));
}

TEncData& TVideoOutRelation::GetEncData()
{
	return m_tEncDevData;
}

void TVideoOutRelation::SetEncData(const TEncData& tEncData)
{
	memcpy(&m_tEncDevData, &tEncData, sizeof(TEncData));
}
void TVideoOutRelation::SetVideoSrcPortNum(s32 nPortNum)
{
    m_nVideoSrcPortNum = nPortNum;
}

s32 TVideoOutRelation::GetVideoSrcPortNum()
{
    return m_nVideoSrcPortNum;
}

BOOL32 TVideoOutRelation::IsCanUse(u8 *pbyEncChnId, u8 *pbyVidChnId, u16 *pwValidOutPort)
{
	BOOL32 bFindValidVicChn = FALSE;
	TPuStatus& tPuStatus = GetEncData().GetPuStatus();
	if (!GetEncData().IsOnLine())
	{
		OspPrintf(TRUE, FALSE, "编码器[%s]不在线！\n", tPuStatus.GetNOString());
		return FALSE;
	}

	u8 byIndex = 0;
	for(byIndex = 0; byIndex < MAX_PU_CHNNUM; byIndex++)
	{
		if (m_atEncVideoInputPortInfo[byIndex].IsConnVideoOutPort() && !m_atEncVideoInputPortInfo[byIndex].IsUsed())
		{
			*pbyVidChnId = byIndex;
			*pwValidOutPort = m_atEncVideoInputPortInfo[byIndex].GetVideoOutPort();
			m_atEncVideoInputPortInfo[byIndex].SetUsed(TRUE);

			bFindValidVicChn = TRUE;
			break;
		}
	}

	if (bFindValidVicChn == FALSE)
	{
		OspPrintf(TRUE, FALSE, "编码器--%s的视频源通道已被用尽!\n",m_tEncDevData.GetDevName());
		return FALSE;
	}

	TVideoEncInputPortParam tParam;
	u8 byNum;

	for(byIndex = 0; byIndex < tPuStatus.GetVideoEncNum(); byIndex++)
	{
		tParam = tPuStatus.GetVideoEncInputPortParam(byIndex);
		byNum = tParam.GetUsedVideoInputPortNum();
		for(u8 byVidChnId = 0; byVidChnId < byNum; byVidChnId++)
		{
			if (tParam.GetUsedVideoInputPort(byVidChnId) != *pbyVidChnId)
			{
				continue;
			}
			else if (!tPuStatus.IsEncChnSendVideo(byIndex))
			{
				tPuStatus.SetEncChnIsSendVideo(byIndex, TRUE);
				*pbyEncChnId = byIndex;
				OspPrintf(TRUE, FALSE, "[TVideoOutRelation] : 选择编码器[%s]的编码通道[%d]\n",
					GetEncData().GetPuStatus().GetNOString(), byIndex);
				return TRUE;
			}
		}
	}

	*pbyEncChnId = 0xff;
	*pbyVidChnId = 0xff;
	*pwValidOutPort = 0;

	return FALSE;
}

void TVideoOutRelation::ResetEncStatus(const u8 byEncChnId, const u8 byVidChnId)
{
	GetEncVideoInputPortInfo(byVidChnId).SetUsed(FALSE);
	GetEncVideoInputPortInfo(byVidChnId).SetCurMatrixInputPort(0);

	m_tEncDevData.GetPuStatus().SetEncChnIsSendVideo(byEncChnId, FALSE);
}

TSwitchData::TSwitchData()
{
	memset(this, 0, sizeof(TSwitchData));
}

void TSwitchData::SetMatrixID(const u32 dwMatrixID)
{
	m_dwMatrixID = dwMatrixID;
}

u32 TSwitchData::GetMatrixID()
{
	return m_dwMatrixID;
}

void TSwitchData::SetEncNO(const TKDMNO& tKdmNo)
{
	m_tEncNO = tKdmNo;
}

TKDMNO& TSwitchData::GetEncNO()
{
	return m_tEncNO;
}

void TSwitchData::SetCamNO(const TKDMNO& tKdmNo)
{
	m_tCamNO = tKdmNo;
}

TKDMNO& TSwitchData::GetCamNO()
{
	return m_tCamNO;
}

void TSwitchData::SetEncChnId(const u8& byEncChnId)
{
	m_byEncChnId = byEncChnId;
}

u8 TSwitchData::GetEncChnId()
{
	return m_byEncChnId;
}

void TSwitchData::SetVidChnId(const u8 byVidChnId)
{
	m_byVidChnId = byVidChnId;
}

u8 TSwitchData::GetVidChnId()
{
	return m_byVidChnId;
}

void TSwitchData::SetVideoOutPort(const u16 wOutPort)
{
	m_wVideoOutPort = wOutPort;
}

u16 TSwitchData::GetVideoOutPort()
{
	return m_wVideoOutPort;
}

void TSwitchData::SetVideoInPort(const u16 wInPort)
{
	m_wVideoInPort = wInPort;
}

u16 TSwitchData::GetVideoInPort()
{
	return m_wVideoInPort;
}

void TSwitchData::SetVideoIp(u32 ip)
{
	m_dwIp = ip;
}

u32  TSwitchData::GetVideoIp()
{
	return m_dwIp;
}

void TSwitchData::SetVideoIpPort(u16 port)
{
	m_wPort = port;
}
u16  TSwitchData::GetVideoIpPort()
{
	return m_wPort;
}

void TSwitchData::SetAudioIp(u32 dwIp)
{
	m_dwAudioIp = dwIp;
}

u32 TSwitchData::GetAudioIp()
{
	return m_dwAudioIp;
}

void TSwitchData::SetAudioIpPort(u16 wIpPort)
{
	m_wAudioPort = wIpPort;
}

u16 TSwitchData::GetAudioIpPort()
{
	return m_wAudioPort;
}

CVcData::CVcData()
{
	
}

CVcData::~CVcData()
{
	
}

TMatrixDataList* CVcData::GetMatrixDataListPtr()
{
	return &m_tMatrixDataList;
}

void CVcData::AddMatrixData(const TMatrixData& tMatrixData)
{
	TMatrixData *ptTemp  = m_tMatrixDataList.AddItem();
	CHECK_ADD_ITEM_RET(ptTemp, "Can't malloc memory for add TMatrixInfo\n");
	
	memset(ptTemp, 0, sizeof(TMatrixData));
	memcpy(ptTemp, &tMatrixData, sizeof(TMatrixData));
}

void CVcData::DelMatrixData(const u32 dwMatrixID)
{
	TMatrixData* ptInfo = m_tMatrixDataList.PosFirst();
	for(; NULL != ptInfo; ptInfo = m_tMatrixDataList.PosNext(ptInfo))
	{
		if (ptInfo->GetMatrixID() == dwMatrixID)
		{
			m_tMatrixDataList.DelItem(ptInfo);
			return;
		}
	}
}

TMatrixData* CVcData::GetMatrixData(const u32 dwMatrixID)
{
	TMatrixData* ptInfo = m_tMatrixDataList.PosFirst();
	for(; NULL != ptInfo; ptInfo = m_tMatrixDataList.PosNext(ptInfo))
	{
		if (ptInfo->GetMatrixID() == dwMatrixID)
		{
			return ptInfo;
		}
	}

	return NULL;
}

TVideoInRTList* CVcData::GetVideoInRTListPtr()
{
	return &m_tVideoInRTList;
}

TVideoInRelation* CVcData::GetVideoInRT(const u32 dwMatrixID)
{
	TVideoInRelation *ptRelation = m_tVideoInRTList.PosFirst();
	for (; NULL != ptRelation; ptRelation = m_tVideoInRTList.PosNext(ptRelation))
	{
		if (ptRelation->GetMatrixID() == dwMatrixID)
		{
			return ptRelation;
		}
	}

	return NULL;
}

TVideoInRelation* CVcData::GetVideoInRT(const TKDMNO& tVidiconID)
{
	TVideoInRelation *ptRelation = m_tVideoInRTList.PosFirst();
	for (; NULL != ptRelation; ptRelation = m_tVideoInRTList.PosNext(ptRelation))
	{
		if (ptRelation->GetCameraData().GetPuStatus().GetKdmNo() == tVidiconID.GetKdmNo())
		{
			return ptRelation;
		}
	}
	
	return NULL;
}

//lqp
TVideoOutRTList* CVcData::GetVideoOutRTListPtr(const u32 dwMatrixID)
{
    s32 nListIndex = GetEncdrListIndex(dwMatrixID);
    if (nListIndex == -1)
    {
        return NULL;
    }
	return &m_atVideoOutRTList[nListIndex];
}

u32 CVcData::GetMatrixID(const TKDMNO& tKdmNo)
{
	s8 szMatrix[6] = {0};

	strncpy(szMatrix, tKdmNo.GetNOString()+21, 5);

	OspPrintf(TRUE, FALSE, "szMatrix : %s\n", szMatrix);

	return atoi(szMatrix);
}

/*************************************************
函数名:
功能:     通过矩阵编号查找矩阵的编码器列表
实现方式: 
参数列表:    [in]u32 matrixIndex  矩阵编号 
返回值列表:  -1:查找失败
              大于等于零:表示查找成功
修改记录
----------------------------------------------------
版本号          日期         修改人         修改结果   
****************************************************/
s32 CVcData::GetEncdrListIndex(u32 dwMatrixIndex)
{
    s32 nRet = -1;
    for (s32 i = 0; i < G200_MAX_MATRIX_NUM; i++)
    {
        if (m_atVideoOutRTList[i].GetItemCount())
        {
            TVideoOutRelation *ptOutRT = m_atVideoOutRTList[i].PosFirst();
            if (ptOutRT->GetMatrixID() == dwMatrixIndex)
            {
                nRet = i;
                break;
            }
        }
        else
        {
            continue;
        }
    }
    return nRet;
}

/***************************************************
函数名：
功能：查找一个可以存放当前子项的列表
实现方式：
参数列表：
返回值列表：-1：表示现在已经没有可以容纳此类矩阵的列表了
修改记录
----------------------------------------------------
版本号          日期         修改人         修改结果   
****************************************************/
s32 CVcData::GetEncdrContainerindex(u32 dwMatrixIndex)
{
    s32 nRet = -1;
	BOOL32 bIdleFlag = FALSE;
    for (s32 i = 0; i < G200_MAX_MATRIX_NUM; i++)
    {
        if (m_atVideoOutRTList[i].GetItemCount())
        {
            TVideoOutRelation *ptOutRT = m_atVideoOutRTList[i].PosFirst();
            if (ptOutRT->GetMatrixID() == dwMatrixIndex)
            {
                nRet = i;
                break;
            }
        }
        else if(!bIdleFlag)
        {
            //保留空列表索引，在轮询所有的列表之后没有找到指定编码器时返回
            nRet = i;
			bIdleFlag = TRUE;
            continue;
        }
    }
    return nRet;
}

/***************************************************************
函数名：
功能：通过编号查找编码器
实现方式：轮询所有编码器列表查找指定编码器
修改记录
----------------------------------------------------
版本号          日期         修改人         修改结果   
                              lqp
*****************************************************************/
TVideoOutRelation* CVcData::GetVideoOutRt(const TKDMNO& tKdmNo)
{
    for (s32 i = 0; i < G200_MAX_MATRIX_NUM; i++)
	{
        if (!m_atVideoOutRTList[i].GetItemCount())
        {
           continue;
        }
        TVideoOutRelation *ptRt = m_atVideoOutRTList[i].PosFirst();
	    for(; NULL != ptRt; ptRt = m_atVideoOutRTList[i].PosNext(ptRt))
	    {
		    if (ptRt->GetEncData().GetPuStatus().GetKdmNo() == tKdmNo)
		    {
                return ptRt;
		    }
	    }
	}
    return NULL;
}
TVideoOutRelation* CVcData::GetVideoOutRt(s32 nMatrix,s32 nIndex)
{
	TVideoOutRTList* ptEncoderList = g_cVcData.GetVideoOutRTListPtr(nMatrix);
	if (ptEncoderList == NULL)
	{
		return NULL;
	}
	TVideoOutRelation* ptEncoder = ptEncoderList->PosFirst();
	s32 nEncoderIndex = 0;
	while (ptEncoder)
	{
		if (nIndex == nEncoderIndex)
		{
			return ptEncoder;
		}
		ptEncoder = ptEncoderList->PosNext(ptEncoder);
		nEncoderIndex++;
	}
	return NULL;
}
//lqp update
void CVcData::AddVideoOutRT(const u32 dwMatrixID, const TVideoOutRelation& tVideoOutRT)
{
    s32 nListIndex = GetEncdrContainerindex(dwMatrixID);
    if (nListIndex == -1)
    {
        G200Log(LOG_MAIN_MOD, LOGLVL_EXCEPTION,"添加编码器时找不到列表，可能由于矩阵配置过多");
        return;
    }
	TVideoOutRelation *ptTmp = m_atVideoOutRTList[nListIndex].AddItem();
	CHECK_ADD_ITEM_RET(ptTmp, "Can't malloc memory for add VideoOutRelation\n");
	
	memcpy(ptTmp, &tVideoOutRT, sizeof(TVideoOutRelation));
}

void CVcData::DelVideoOutRT(const u32 dwMatrixID, const TVideoOutRelation& tVideoOutRT)
{
    s32 nListIndex = GetEncdrListIndex(dwMatrixID);
    if (nListIndex == -1)
    {
        return;
    }
	TVideoOutRelation tDelData;
	memcpy(&tDelData, &tVideoOutRT, sizeof(TVideoOutRelation));
	m_atVideoOutRTList[nListIndex].DelItem(&tDelData);
}

void CVcData::AddVideoInRT(const TVideoInRelation& tVideoInRt)
{
	TVideoInRelation *ptTemp  = m_tVideoInRTList.AddItem();
	CHECK_ADD_ITEM_RET(ptTemp, "Can't malloc memory for add VideoInRelation\n");
	
	memset(ptTemp, 0, sizeof(TVideoInRelation));
	memcpy(ptTemp, &tVideoInRt, sizeof(TVideoInRelation));
}

void CVcData::DelVideoInRT(const TVideoInRelation& tVideoInRt)
{
	TVideoInRelation tRelation;
	memcpy(&tRelation, &tVideoInRt, sizeof(TVideoInRelation));
	m_tVideoInRTList.DelItem(&tRelation);
}


TPuiDataList* CVcData::GetPuiDataListPtr()
{
	return &m_tPuiDataList;
}

TPuiData* CVcData::AddPuiDev(const TBasePui& tBasePui)
{
	TPuiData *ptTmp = m_tPuiDataList.AddItem();
	CHECK_ADD_ITEM_RETNULL(ptTmp, "Can't malloc memory for add PuiData\n");

	ptTmp->SetBasePui(tBasePui);
	ptTmp->SetUsed(TRUE);
	ptTmp->SetConnected(FALSE);
	ptTmp->SetSure(TRUE);
	ptTmp->SetDel(FALSE);

	return ptTmp;
}

void CVcData::DelPuiDev(const TPuiID& tPuiID)
{
	TPuiData *ptPuiData = m_tPuiDataList.PosFirst();
	for (; NULL != ptPuiData; ptPuiData = m_tPuiDataList.PosNext(ptPuiData))
	{
		if (ptPuiData->GetBasePui().GetKdmNo() == tPuiID.GetKdmNo())
		{
			m_tPuiDataList.DelItem(ptPuiData);
			return;
		}
	}

	return;
}

TPuiData* CVcData::GetPuiData(const TPuiID& tPuiId)
{
	TPuiData *ptPuiData = m_tPuiDataList.PosFirst();
	for (; NULL != ptPuiData; ptPuiData = m_tPuiDataList.PosNext(ptPuiData))
	{
		if (ptPuiData->GetBasePui().GetKdmNo() == tPuiId.GetKdmNo())
		{
			return ptPuiData;
		}
	}
	
	return NULL;
}

TPuiData* CVcData::GetPuiData(const u16 wInstId)
{
    TPuiData *pTmp = m_tPuiDataList.PosFirst();
	
    for(; pTmp != NULL; pTmp = m_tPuiDataList.PosNext(pTmp))
    {
        if(pTmp->GetInstId() == wInstId)
        {
            return pTmp;
        }
    }
	
    return NULL;
}

TSwitchDataList* CVcData::GetSwitchDataListPtr()
{
	return &m_tSwitchDataList;
}

void CVcData::AddSwitchData(const TSwitchData& tSwitchData)
{
	TSwitchData *ptTmp = m_tSwitchDataList.AddItem();
	CHECK_ADD_ITEM_RET(ptTmp, "Cannot malloc memory for add switchdata!\n");

	memcpy(ptTmp, &tSwitchData, sizeof(TSwitchData));
}

void CVcData::DelSwitchDataByCamNo(const TKDMNO& tSrcNo,u32 dwIp,u32 dwPort)
{
	TSwitchData *ptSwitchData = m_tSwitchDataList.PosFirst();
	for(; NULL != ptSwitchData; ptSwitchData = m_tSwitchDataList.PosNext(ptSwitchData))
	{
		if (ptSwitchData->GetCamNO() == tSrcNo && ptSwitchData->GetVideoIp() == dwIp 
			&& ptSwitchData->GetVideoIpPort() == dwPort)
		{
			OspPrintf(TRUE, FALSE, "\n----------删除交换数据-----------\n");
			OspPrintf(TRUE, FALSE, "摄像头编号     ：%s\n", tSrcNo.GetNOString());
			OspPrintf(TRUE, FALSE, "视频输入端口号 ：%d\n", ptSwitchData->GetVideoInPort());
			OspPrintf(TRUE, FALSE, "编码器编号     ：%s\n", ptSwitchData->GetEncNO().GetNOString());
			OspPrintf(TRUE, FALSE, "编码通道       ：%d\n", ptSwitchData->GetEncChnId());
			OspPrintf(TRUE, FALSE, "视频输入通道   ：%d\n", ptSwitchData->GetVidChnId());
			OspPrintf(TRUE, FALSE, "矩阵输出通道号 ：%d\n", ptSwitchData->GetVideoOutPort());
			s8 achIp[20] = {0};
			IPtoString(dwIp,achIp,sizeof(achIp));
			OspPrintf(TRUE, FALSE, "视频接收ip地址     ：%s\n",achIp);
			OspPrintf(TRUE, FALSE, "视频接收端口地址   ：%d\n",dwPort);
			OspPrintf(TRUE, FALSE, "---------------------------------\n");

			m_tSwitchDataList.DelItem(ptSwitchData);
			return;
		}
	}
}
TSwitchData* CVcData::GetSwitchData(const TKDMNO& tSrcNo,u32 dwIp,u32 dwPort)
{
	TSwitchData *ptSwitchData = m_tSwitchDataList.PosFirst();
	for(; NULL != ptSwitchData; ptSwitchData = m_tSwitchDataList.PosNext(ptSwitchData))
	{
		if (ptSwitchData->GetCamNO() == tSrcNo && ptSwitchData->GetVideoIp() == dwIp 
			&& ptSwitchData->GetVideoIpPort() == dwPort)
		{			
			return ptSwitchData;
		}
		else if (ptSwitchData->GetCamNO() == tSrcNo && dwIp == 0 && dwPort == 0)
		{
			return ptSwitchData;
		}
	}

	return NULL;
}

void CVcData::AddOnlineEncNum()
{
	m_dwOnlineEncNum++;
}

u32 CVcData::GetOnlineEncNum()
{
	return m_dwOnlineEncNum;
}

void CVcData::SetOnlineEncNum(u32 dwOnlineEncNum)
{
	m_dwOnlineEncNum = dwOnlineEncNum;
}

void CVcData::DelOnlineEncNum()
{
	if (m_dwOnlineEncNum >0)
	{
		m_dwOnlineEncNum--;
	}
	else
	{
		m_dwOnlineEncNum = 0;
	}
}

BOOL32 CVcData::ChooseEnableEnc(const u32 dwMatrixID, const u16 wInPort, OUT TVideoOutRelation& tMatrixCtrlEnc, 
								OUT TVideoOutRelation& tRcvStreamEnc, OUT u8& byEncChnId, OUT u8& byVidChnId, OUT u16& wOutPort)
{
	TVideoOutRTList* ptOutRtList = GetVideoOutRTListPtr(dwMatrixID);
	if (NULL == ptOutRtList || ptOutRtList->GetItemCount() == 0)
	{
		OspPrintf(TRUE, FALSE, "[G200/VcSched][ChooseEnableEnc] : 没有可以调度的解码器！\n");
		return FALSE;
	}
	
	BOOL32 bFindConnMatrixEnc = FALSE;
	BOOL32 bFindIdelEnc = FALSE;
	
	TVideoOutRelation* ptOutRt = ptOutRtList->PosFirst();
	TVideoOutRelation* ptRcvStreamEnc = NULL;
	for(; NULL != ptOutRt; ptOutRt = ptOutRtList->PosNext(ptOutRt))
	{
		if (!ptOutRt->GetEncData().IsOnLine())
		{
			continue;
		}
		
		if (!bFindIdelEnc && ptOutRt->IsCanUse(&byEncChnId, &byVidChnId, &wOutPort))
		{
			ptOutRt->GetEncVideoInputPortInfo(byVidChnId).SetCurMatrixInputPort(wInPort);
			bFindIdelEnc = TRUE;

			//保存当前编码器的指针，后续没有找到串口时，重置状态
			ptRcvStreamEnc = ptOutRt;
			memcpy(&tRcvStreamEnc, ptOutRt, sizeof(TVideoOutRelation));
			OspPrintf(TRUE, FALSE, "[G200/VcSched][ChooseEnableEnc] : 选择编码器[%s]的编码通道[%d]、视频输入通道[%d]绑定的矩阵输出端口号[%d]发送码流\n",
				ptOutRt->GetEncData().GetDevName(), byEncChnId, byVidChnId, wOutPort);
			if (bFindConnMatrixEnc)
			{
				break;
			}
		}
		
		if (!bFindConnMatrixEnc && ptOutRt->IsConnMatrix())
		{
			memcpy(&tMatrixCtrlEnc, ptOutRt, sizeof(TVideoOutRelation));
			bFindConnMatrixEnc = TRUE;
			
			OspPrintf(TRUE, FALSE, "[G200/VcSched][ChooseEnableEnc] : 选中了连接矩阵的编码器[%s]执行矩阵控制命令！\n", tMatrixCtrlEnc.GetEncData().GetDevName());
			if (bFindIdelEnc)
			{
				break;
			}
		}
	}
	
	if (!bFindIdelEnc || !bFindConnMatrixEnc)
	{
		if (bFindIdelEnc && !bFindConnMatrixEnc)
		{
			ptRcvStreamEnc->ResetEncStatus(byEncChnId,byVidChnId);
		}
		OspPrintf(TRUE, FALSE, "[G200/VcSched][ProcSendStream] : 编码器繁忙[%d] 或者 没有任何编码器连接了矩阵[%d]！\n",
			!bFindIdelEnc, !bFindConnMatrixEnc);
		return FALSE;
	}

	return TRUE;
}

void CVcData::SetConnMplat(BOOL32 status)
{
	m_bConnMPlat = status;
}

BOOL32 CVcData::GetConnMplat()
{
	return m_bConnMPlat;
}
