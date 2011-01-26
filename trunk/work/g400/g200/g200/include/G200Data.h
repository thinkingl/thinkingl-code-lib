#ifndef _G200_DATA_H
#define _G200_DATA_H

#include "Common.h"
#include "kdmlist.h"
#include "readlicense.h"
#include "usbkeyconst.h"
#include "cnetmanager.h"
struct TPuiData
{
protected:
    TBasePui  m_tBasePui;                       //�豸
    u16       m_wInstId;                        //ʵ����
	u8		  m_byIpNum;						//IP����
	TIpAddr   m_atDevIp[IP_NUM];				//IP�б�
    u8        m_byDevStat;                      //�豸״̬
	                                            //  0:����1:ʹ��2:����3:ͬ��
	
    s32		  m_nRegTime;                       //ע��ʱ��
	u32       m_dwMaxPuNum;					    //ǰ��������
	u32       m_dwCurPuNum;					    //����ǰ����
	u32		  m_dwRegionID;                     //�����
public:
    TPuiData(void);
	
    void PrintData(u8 byOffsetSize = 0) const;
	
    void SetConnected(BOOL32 bConnected);//����Pui����״̬
    void SetInstId(u16 wInstId);         //����Puiʵ����
    void SetBasePui(const TBasePui& tBasePui);//����BasePui
    const TBasePui& GetBasePui(void) const;   //��ȡBasePui
    TBasePui& GetBasePui(void);               //��ȡBasePui
    BOOL32 IsConnected(void)const;            //��ȡPui����״̬
    u16 GetInstId(void) const;                //��ȡPuiʵ����
    void SetUsed(BOOL32 bUsed);               //�����Ƿ�����
    BOOL32 IsUsed(void)const;                 //��ȡ�Ƿ�����
    void SetDel(BOOL32 bDel);                 //�����Ƿ�����
    BOOL32 IsDel(void)const;                  //��ȡ�Ƿ�����
	void SetSure(BOOL32 bSure);               //�����Ƿ�ͬ��
	BOOL32 IsSure(void)const;                 //��ȡ�Ƿ�ͬ��
	
    void SetRegTime();
    s32 GetRegTime() const;
	
	void SetCurPuNum(u32 dwNum);    //���õ�ǰ����PU��Ŀ
	u32  GetCurPuNum() const;       //��ȡ��ǰ����PU��Ŀ
	void AddCurPuNum();             //����һ����ǰ����PU��Ŀ
	void DelCurPuNum();             //����һ����ǰ����PU��Ŀ
	
	void SetMaxPuNum(u32 dwNum);    //�����������PU��Ŀ
	u32  GetMaxPuNum() const;       //��ȡ�������PU��Ŀ

	void SetIpNum(u8 byNum);
	u8 GetIpNum() const;
	void SetIpAddr(u8 byIndex, const TIpAddr& tIpAddr);//����������ip��ַ
	const TIpAddr& GetIpAddr(u8 byIndex) const;
};
typedef CKdmListEx<TPuiData,1> TPuiDataList;

struct TVideoSrcInfo
{
protected:
	u8 m_byCamType;         //����ͷ����;
    u16 m_wCamId;           //����ͷ��ַ��;
    u8 m_byTransType;       //�������䷽ʽ;
    u8 m_byCamCtrlWay;      //��Ƶ����Դ����ͷ���Ʒ�ʽ��0:����1:����
    u8 m_byCamParseWay;     //��Ƶ����Դ����ͷЭ�������ʽ;
    u8 m_byCamAutoRun;      //����ͷ�Զ�Ѳ������;
	s8 szVideoSrcAlias[MAXLEN_ALIAS+1];
public:
	TVideoSrcInfo();

	u8 GetCamType();
	void SetCamType(const u8 byCamType);
	
	u16 GetCamId();
	void SetCamId(const u16 wCamId);
	
	u8 GetTransType();
	void SetTransType(const u8 byTransType);
	
	u8 GetCamCtrlWay();
	void SetCamCtrlWay(const u8 byCamCtrlWay);
	
	u8 GetCamParseWay();
	void SetCamParseWay(const u8 byCamParseWay);
	
	u8 GetCamAutoRun();
	void SetCamAutoRun(const u8 byCamAutoRun);

	LPCSTR GetVideoSrcAlias() const;
    void SetVideoSrcAlias(LPCSTR lpszSrcVideoChnAlias);
};

struct TMatrixData
{
public:
	TMatrixData();
	
public:
	u32 GetMatrixID();
	void SetMatrixID(const u32 dwMatrixID);
	
	u8 GetMatrixType();
	void SetMatrixType(const u8 byMatrixType);
	
    void SetMatrixName(const s8* pszMatrixName);
    const s8* GetMatrixName();

    void SetMatrixInportNum(s32 nPortNum);
    s32 GetMatrixInportNum();

    void SetMatrixOutportNum(s32 nPortNum);
    s32 GetMatrixOutportNum();

	BOOL32 IsNeedNtyCamStatus();
	void SetNeedNtyCamStatus(BOOL32 bNeed);
	
	u16 GetOnlineEncNum();
	void SetOnlineEncNum(const u16 wNum);
	void AddOnlineEncNum();
	void SubOnlineEncNum();
	
protected:
    s8 m_achMatrixName[MAXLEN_NAME + 1];
    s32 m_nMatrixInportNum;
    s32 m_nMatrixOutPortNum;
	u32 m_dwMatrixID;
	u8 m_byMatrixType;
	u16 m_wOnlineEncNum;
	BOOL32 m_bNeedNtyCamStatus;
};

typedef CKdmListEx<TMatrixData, 1> TMatrixDataList;

struct TCameraData
{
protected:
    TPuStatus  m_tPuStatus;                 //PU״̬
    TVideoSrcInfo m_tVideoSrcInfo;          //��ƵԴ��Ϣ
    u8  m_byDevStat;                        //�豸״̬
                					        //1:����
                                            //2:ͬ��
    s8 m_szDevName[MAXLEN_NAME+1];         //�豸����;
public:
    TCameraData(void);
    void PrintData(u8 byOffsetSize = 0);

    void SetPuStatus(const TPuStatus& tPuStatus); //����PU״̬
    void SetPuOnline(BOOL32 bIsOnline);

    TPuStatus* GetPuStatusPtr();
    TPuStatus& GetPuStatus(void); //��ȡPU״̬
    
	TVideoSrcInfo& GetVideoSrcInfo();
	void SetVideoSrcInfo(const TVideoSrcInfo& tVideoSrcInfo);

    BOOL32 IsUsed() const;          //�Ƿ�����
    void SetUsed(BOOL32 bIsUsed);   //�����Ƿ�����

    void SetDel(BOOL32 bDel);       //�����Ƿ�����
    BOOL32 IsDel(void)const;        //��ȡ�Ƿ�����

    BOOL32 IsStop(s32 dwTime) const;  //�Ƿ�ͣ��

    BOOL32 IsOnLine(void) const;      //�Ƿ�����

	void SetSure(BOOL32 bSure); //�����Ƿ�ͬ��
	BOOL32 IsSure(void) const;   //��ȡ�Ƿ�ͬ��
    LPCSTR GetDevName();
    void SetDevName(LPCSTR lpszName);
};

struct TEncData
{
protected:
    TPuStatus       m_tPuStatus;                        //PU״̬
    u16             m_wCamNum;                          //��ص���
    TBasePuVideoChnInfo m_tBasePuVideoChnInfo;          //��ƵԴ����
    u32             m_dwIpAddr;                         //IP��ַ(��·��)
    u16             m_wStartPort;                       //Դ������ʼ�˿�
    u16             m_wInstId;                          //ʵ����
    u8              m_byDevStat;                        //�豸״̬
                                                            //1:ȡ��Cap
                                                            //2:ȡ��״̬
                					                        //3:����
                                                            //4:ͬ��

    u8              m_bySwitchType;                     //ת������
    TTransportAddr  m_tTransportAddr;                   //�鲥��ַ
	s8              m_szManuFac[MAXLEN_DEVICE_MANUFACTURER+1]; //�豸������

    u32             m_dwPuiAddr;                        //��Ӧ��ƽ̨��ַ(Pui��)(������)
    s8 m_szDevName[MAXLEN_NAME+1];         //�豸����;
public:
    TEncData(void);
    void PrintData(u8 byOffsetSize = 0);
    void SetPuStatus(const TPuStatus& tPuStatus); //����PU״̬
    void SetPuOnline(const BOOL32 bIsOnline);

    TPuStatus* GetPuStatusPtr();
    TPuStatus& GetPuStatus(void); //��ȡPU״̬
    void SetBasePuVideoChnInfo(const TBasePuVideoChnInfo& tBasePuVideoChnInfo);//������ƵԴ����
    const TBasePuVideoChnInfo& GetBasePuVideoChnInfo(void) const; //��ȡ��ƵԴ����
	void ParsePuVideoChnInfo();//ˢ��PuStatus������ͨ������ƵԴ����
    void SetBasePu(const TBasePu& tBasePu);
    BOOL32 IsUsed() const;          //�Ƿ�����
    void SetUsed(const BOOL32 bIsUsed);   //�����Ƿ�����

    u32 GetIpAddr() const;          //��ȡIP��ַ��������
    void SetIpAddr(const u32 dwIpAddr);   //����IP��ַ��������

    u16 GetInstId(void) const;          //��ȡPuiʵ����
    void SetInstId(const u16 wInstId);  //����Puiʵ����

    void SetDel(const BOOL32 bDel);       //�����Ƿ�����
    BOOL32 IsDel(void)const;        //��ȡ�Ƿ�����
    void    SetSwitchType(u8 bySwitchType); //����ת������
    u8      GetSwitchType(void)const;       //��ȡת������
    void    SetTransportAddr(const TTransportAddr& tTransportAddr);//�����鲥��ַ
    const TTransportAddr&  GetTransportAddr(void)const;//��ȡ�鲥��ַ

    void SetStopTime(const TKDMTime& tKdmTime); //����ͣ��ʱ��
    TKDMTime GetStopTime() const;               //��ȡͣ��ʱ��
    BOOL32 IsStop(const s32 dwTime) const;            //�Ƿ�ͣ��
    u16 GetStartPort()const;//��ȡԴ������ʼ�˿�
    void SetStartPort(u16 wStartPort);//����Դ������ʼ�˿�
    u8 GetDirectTransNum()const;//��ȡֱ��·��
    void SetDirectTransNum(const u8 byDirectTransNum);//����ֱ��·��

    u8 GetNowDirectTransCnt(const u8 byEncChnId)const;//��ȡ��ǰֱ��·��
    void SetNowDirectTransCnt(const u8 byEncChnId, const u8 byNowDirectTransCnt);//���õ�ǰֱ��·��
    BOOL32 IsNeedAutoLook(void)const;//�Ƿ���Ҫ�Զ�ѡ��
    void SetNeedAutoLook(const BOOL32 bNeedAutoLook);//�����Զ�ѡ��
    BOOL32 IsOnLine(void) const;              //�Ƿ�����

    BOOL32 IsHasGetCap() const;
    void HasGetCap(const BOOL32 bIsHasGetCap);

    BOOL32 IsHasGetStatus() const;
    void HasGetStatus(const BOOL32 bIsHasGetStatus);

	void SetSure(const BOOL32 bSure); //�����Ƿ�ͬ��
	BOOL32 IsSure(void)const; //��ȡ�Ƿ�ͬ��

    BOOL32 IsCanUse() const;              //ƽ̨�Ƿ��ȡ������Ϣ

    void SetCamNum(const u16 wCamNum);    //���ü�ص���
    u16 GetCamNum() const;          //��ȡ��ص���

 	void SetManuFac(LPCSTR szManuFac);  //���ó�������
	LPCSTR GetManuFac() const;          //��ȡ��������

    u32 GetPuiIpAddr() const;          //��ȡPu��½��Pui��IP��ַ��������
    void SetPuiIpAddr(const u32 dwIpAddr);   //����Pu��½��Pui��IP��ַ��������

    const s8* GetDevName();
    void SetDevName(const s8* lpszName);
};


//������Ƶ����˿�������ͷ�İ󶨹�ϵ
struct TVideoInRelation
{
public:
	TVideoInRelation();
	
	u32 GetMatrixID();
	void SetMatrixID(const u32 dwMatrixID);

	u8 GetMatrixType();
	void SetMatrixType(const u8 byMatrixType);
	
	u16 GetVideoInPort();
	void SetVideoInPort(const u16 wVideoInPort);
	
	TCameraData& GetCameraData();
	void SetCameraData(const TCameraData& tCamData);

    void SetRegisterStatus(BOOL32 bRegiste);
	BOOL32 GetRegisterStatus();

    void SetCamAddr(s32 nCamAddr);
    s32 GetCamAddr();

    void SetCamType(s32 nType);
    s32 GetCamType();

protected:
	u32 m_dwMatrixID;
	u8  m_byMatrixType;      //��������
	u16 m_wVideoInPort;      //��������˿ں�

    BOOL32 m_bRegiste;
    s32 m_nCamAddr;
    s32 m_nCamType;
	TCameraData m_tCamData;       //���������˿����ӵ�����ͷ��Ϣ
};
typedef CKdmListEx<TVideoInRelation, 1> TVideoInRTList;

struct TEncVideoInputPortInfo
{
public:
	TEncVideoInputPortInfo();
	
	BOOL32 IsConnVideoOutPort();
	void SetConnVideoOutPort(BOOL32 bConn);

	BOOL32 IsUsed();
	void SetUsed(BOOL32 bUsed);

	u16 GetCurMatrixInputPort();
	void SetCurMatrixInputPort(const u16 wCurInputPort);

	u16 GetVideoOutPort();
	void SetVideoOutPort(const u16 wPort);

protected:
	BOOL32 m_bIsConnVideoOutPort;  //�ñ���������ƵԴ����˿��Ƿ������˾������Ƶ����˿�
	BOOL32 m_bIsUsed;              //�ñ���������ƵԴ����˿��Ƿ�����ʹ��
	u16 m_wCurMatrixInPutPort;     //��ǰ���ڷ���ľ�����Ƶ����Դ�˿�
	u16 m_wVideoOutPort;           //�ñ���������ƵԴ����˿����ӵľ�����Ƶ����˿ں�
};

//������Ƶ����˿���������Ķ�Ӧ��ϵ
struct TVideoOutRelation
{
public:
	TVideoOutRelation();

	u32 GetMatrixID();
	void SetMatrixID(const u32 dwMatrixID);

	BOOL32 IsConnMatrix();
	void SetConnMatrix(BOOL32 bConnMatrix);
	
	TEncVideoInputPortInfo& GetEncVideoInputPortInfo(const u8 byVidChnId);
	void SetEncVideoInputPortInfo(const u8 byVidChnId, const TEncVideoInputPortInfo& tInfo);
	
	TEncData& GetEncData();
	void SetEncData(const TEncData& tEncData);

	//��ȡ���������õı���ͨ������Ƶ����ͨ������Ӧ�ľ�������˿ں�
	BOOL32 IsCanUse(OUT u8 *pbyEncChnId, OUT u8 *pbyVidChnId, OUT u16 *pwValidOutPort);

	//�ָ��������ı���ͨ������Ƶ����ͨ����ʹ��״̬
	void ResetEncStatus(const u8 byEncChnId, const u8 byVidChnId);
	
    void SetVideoSrcPortNum(s32 nPortNum);
    s32 GetVideoSrcPortNum();

protected:
	u32    m_dwMatrixID;
    s32 m_nVideoSrcPortNum;
	BOOL32 m_bIsConnMatrix;   //�Ƿ������˾���
	TEncData m_tEncDevData;   //��������Ϣ
	TEncVideoInputPortInfo m_atEncVideoInputPortInfo[MAX_PU_CHNNUM];      //����������ƵԴ����˿���Ϣ
};

typedef CKdmListEx<TVideoOutRelation, 1> TVideoOutRTList;

struct TSwitchData
{
public:
	TSwitchData();

public:
	void SetMatrixID(const u32 dwMatrixID);
	u32 GetMatrixID();

	void SetEncNO(const TKDMNO& tKdmNo);
	TKDMNO& GetEncNO();

	void SetCamNO(const TKDMNO& tKdmNo);
	TKDMNO& GetCamNO();

	void SetEncChnId(const u8& byEncChnId);
	u8 GetEncChnId();

	void SetVidChnId(const u8 byVidChnId);
	u8 GetVidChnId();

	void SetVideoOutPort(const u16 wOutPort);
	u16 GetVideoOutPort();

	void SetVideoInPort(const u16 wInPort);
	u16 GetVideoInPort();

	void SetVideoIp(u32 ip);
	u32  GetVideoIp();

	void SetVideoIpPort(u16 port);
	u16  GetVideoIpPort();
	
	void SetAudioIp(u32 dwIp);
	u32  GetAudioIp();

	void SetAudioIpPort(u16 wIpPort);
	u16  GetAudioIpPort();
protected:
	u32 m_dwMatrixID;    //������
	TKDMNO m_tEncNO;     //���������
	TKDMNO m_tCamNO;     //����ͷ���
	u8     m_byEncChnId; //����ͨ����
	u8     m_byVidChnId; //��������Ƶ����ͨ���ţ���0��ʼ��
	u16    m_wVideoOutPort; //��������˿ںţ���1��ʼ��
	u16    m_wVideoInPort;  //��������˿ںţ���1��ʼ��

	u32    m_dwIp;          //����������ip��ַ
	u16    m_wPort;        //���������Ķ˿�

	u32    m_dwAudioIp;     //������Ƶ��ip��ַ
	u16    m_wAudioPort;    //������Ƶ�Ķ˿�
};
typedef CKdmListEx<TSwitchData, 1> TSwitchDataList;

class CLicenseHandler
{
public:
	enum{TYPE_NONE,TYPE_FILE,TYPE_USB};
	CLicenseHandler(const s8* pszDevice = "eth0"):m_pszDeviceName(pszDevice)
	{
		m_bValid = FALSE;
		m_nMaxChannel = 0;
		m_nMaxEncoder = 0;
		m_abyMacAddr[0] = 0;
		m_achApproval[0] = 0;
		m_achMaxDate[0] = 0;
		m_byType = TYPE_NONE;
	}
	void InitLicense(const s8* pszFilePath)
	{	
#ifdef _LINUX_
		s8 achHostFile[MAXLEN_FILENAME + 1];
		s8 achLicenseKeyFile[MAXLEN_FILENAME + 1];
		sprintf(achHostFile,"%s%s",pszFilePath,"host.txt");
		sprintf(achLicenseKeyFile,"%s%s",pszFilePath,"kedalicense.key");
		s32 nLicenceLen = 64;
		s8 achTmp[64] = {0};
		
		do 
		{
			if (!m_cNetManager.GetMacAddr(m_pszDeviceName,m_abyMacAddr,sizeof(m_abyMacAddr)))
			{
				m_bValid = FALSE;
				break;
			}
			
			FILE* pFile = fopen(achHostFile,"w");
			fwrite(m_abyMacAddr,sizeof(u8),strlen(m_abyMacAddr),pFile);
			fclose(pFile);
			
			u16 wRet = 0;		
			wRet = ReadLicenseFile(achLicenseKeyFile);
			if (0 != wRet)
			{
				OspPrintf(TRUE , FALSE , "Fail ReadLicenseFile\n");
				break;
			}
			
			//��ȡ�ļ��еĻ�����
			wRet = GetValue("KEY_LICENSE_MACHINENO", achTmp, nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , "Fail get KEY_LICENSE_MACHINENO\n");
				break;
			}
			printf("machineno:%s\n", achTmp);
			if (memcmp(achTmp, m_abyMacAddr, 64) != 0)
			{
				OspPrintf(TRUE, FALSE , "Fail cmp MACHINENO\n");
				break;
			}
			//license��Ч
			m_bValid = TRUE;
			m_byType = TYPE_FILE;

			nLicenceLen = 64;
			wRet = GetValue("KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM", achTmp, nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from file:KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM not found!\n");
				break;
			}
			m_nMaxChannel = atoi(achTmp);
			
			nLicenceLen = 64;
			wRet = GetValue("KEY_LICENSE_MAX_OTHER_ENCODER_NUM",achTmp,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from file:KEY_LICENSE_MAX_OTHER_ENCODER_NUM not found!\n");
				break;
			}
			m_nMaxEncoder = atoi(achTmp);
			
			nLicenceLen = sizeof(m_achMaxDate);
			wRet = GetValue("KEY_LICENSE_MAX_DATE",m_achMaxDate,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from file:KEY_LICENSE_MAX_DATE not found!\n");
				break;
			}

			nLicenceLen = sizeof(m_achApproval);
			wRet = GetValue("KEY_LICENSE_KEY",m_achApproval,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from file:KEY_LICENSE_KEY not found!\n");
				break;
			}
			
		} while (TRUE);
		
		//��ȡ�ļ����ɹ�,���ȡusb�е�license
		if (!m_bValid)
		{
			u16 wRet = ReadLicenseFile("USBKEY");//����0Ϊ�ɹ�
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey license\n");
				return;		
			}
			m_bValid = TRUE;
			m_byType = TYPE_USB;

			nLicenceLen = 64;
			wRet = GetValue("KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM", achTmp, nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from usbkey:KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM not found!\n");
				return;
			}
			nLicenceLen = 64;
			wRet = GetValue("KEY_LICENSE_MAX_OTHER_ENCODER_NUM",achTmp,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from usbkey:KEY_LICENSE_MAX_OTHER_ENCODER_NUM not found!\n");
				return;
			}
			m_nMaxEncoder = atoi(achTmp);
			
			nLicenceLen = sizeof(m_achMaxDate);
			wRet = GetValue("KEY_LICENSE_MAX_DATE",m_achMaxDate,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from usbkey:KEY_LICENSE_MAX_DATE not found!\n");
				return;
			}

			nLicenceLen = sizeof(m_achApproval);
			wRet = GetValue("KEY_LICENSE_KEY",m_achApproval,nLicenceLen);
			if (0 != wRet)
			{
				OspPrintf(TRUE, FALSE , " failed to read usbkey from usbkey:KEY_LICENSE_KEY not found!\n");
				return;
			}
		}
#else
		m_bValid = TRUE;
		m_nMaxChannel = 32;
		m_nMaxEncoder = 32;
#endif
	}	

	s32 GetMaxChannel() const
	{
		return m_nMaxChannel;
	}

	const s8* GetMacInfo() const
	{
		return m_abyMacAddr;
	}

	const s8* GetApprovalNo() const
	{
		return m_achApproval;
	}

	const s8* GetMaxDate() const
	{
		return m_achMaxDate;
	}

	BOOL32 IsLicenseValid()
	{
		return m_bValid;
	}

	u8 GetLicenseType()
	{
		return m_byType;
	}

	s32 GetMaxEncoderNum()
	{
		return m_nMaxEncoder;
	}
private:
	CNetManager m_cNetManager;//��ȡmac��ַ
	BOOL32 m_bValid;//license�Ƿ���Ч
	s32 m_nMaxChannel;//license֧�ֵ����ͨ����
	s32 m_nMaxEncoder;//����ע�������������
	s8 m_abyMacAddr[128 + 1];//��Ӧ������MAC,����֮��
	s8 m_achApproval[64 + 1];//���֤���
	s8 m_achMaxDate[128 + 1];//���֤��Ч��
	u8 m_byType;//license����
	const s8* m_pszDeviceName;
};

class CVcData
{
public:
	CVcData();
	~CVcData();

	TMatrixDataList* GetMatrixDataListPtr();
	void AddMatrixData(const TMatrixData& tMatrixData);
	void DelMatrixData(const u32 dwMatrixID);
	TMatrixData* GetMatrixData(const u32 dwMatrixID);

	TVideoInRTList *GetVideoInRTListPtr();
	void AddVideoInRT(const TVideoInRelation& tVideoInRt);
	void DelVideoInRT(const TVideoInRelation& tVideoInRt);
	TVideoInRelation* GetVideoInRT(const u32 dwMatrixID);
	TVideoInRelation* GetVideoInRT(const TKDMNO& tVidiconID);

	TVideoOutRTList *GetVideoOutRTListPtr(const u32 dwMatrixID);
	void AddVideoOutRT(const u32 dwMatrixID, const TVideoOutRelation& tVideoOutRT);
	void DelVideoOutRT(const u32 dwMatrixID, const TVideoOutRelation& tVideoOutRT);
	TVideoOutRelation* GetVideoOutRt(const TKDMNO& tKdmNo);
	TVideoOutRelation* GetVideoOutRt(s32 nMatrix,s32 nIndex);
	TPuiDataList *GetPuiDataListPtr();
	TPuiData* AddPuiDev(const TBasePui& tBasePui);
	void DelPuiDev(const TPuiID& tPuiID);
	TPuiData* GetPuiData(const TPuiID& tPuiId);
	TPuiData* GetPuiData(const u16 wInstId);

	TSwitchDataList* GetSwitchDataListPtr();
	void AddSwitchData(const TSwitchData& tSwitchData);
	void DelSwitchDataByCamNo(const TKDMNO& tSrcNo,u32 dwIp = 0,u32 dwPort = 0);
	TSwitchData* GetSwitchData(const TKDMNO& tSrcNo,u32 dwIp = 0,u32 dwPort = 0);

	void AddOnlineEncNum();
	void SetOnlineEncNum(u32 dwOnlineEncNum);
	void DelOnlineEncNum();
	u32 GetOnlineEncNum();

	BOOL32 ChooseEnableEnc(const u32 dwMatrixID, const u16 wInPort, OUT TVideoOutRelation& tMatrixCtrlEnc, OUT TVideoOutRelation& tRcvStreamEnc, OUT u8& byEncChnId, OUT u8& byVidChnId, OUT u16& wOutPort);
	
	void SetConnMplat(BOOL32 status);
	BOOL32 GetConnMplat();
private:
	u32 GetMatrixID(const TKDMNO& tKdmNo);
    s32 GetEncdrListIndex(u32 dwMatrixIndex);
    s32 GetEncdrContainerindex(u32 dwMatrixIndex);
	
public:
	TG200CfgBase m_tG200Cfg;
	CLicenseHandler m_cLicenseHandler;
public:
	SEMHANDLE       m_hMainSem;
protected:
	TMatrixDataList m_tMatrixDataList;  //������Ϣ�б�
	TVideoInRTList  m_tVideoInRTList;   //��Ƶ����󶨹�ϵ�б�
	TVideoOutRTList m_atVideoOutRTList[G200_MAX_MATRIX_NUM];   //ÿ�������Ŷ�Ӧһ����Ƶ�������
	TPuiDataList    m_tPuiDataList;     //pui�豸�б�
	TSwitchDataList m_tSwitchDataList;  //���������б�

	u32 m_dwOnlineEncNum;  //���߱�������Ŀ
	BOOL32 m_bConnMPlat;   //���ϼ�ƽ̨������״̬
};

extern CVcData g_cVcData;

#endif