#ifndef _G200_COMMON_H
#define _G200_COMMON_H

#include "osp.h"
#include "monmsg.h"
#include "cmstypes.h"
#include "kdmver.h"
#include "g200comm.h"
#include "httpcommon.h"


#define G200_VERSION KDM_VER(G200, 0101)
#define G200_CONFVERSION "KDM2800_G200 V1R1B1"

#ifndef OUT
#define OUT
#endif

/************************************************************************/
/* �ڲ�APP�Ŷ���                                                        */
/************************************************************************/
#define AID_G200_BEGIN            200
#define AID_G200_VCSCHED      AID_G200_BEGIN+1
#define AID_G200_SDK          AID_G200_BEGIN+2
#define AID_G200_CFG          AID_G200_BEGIN+3
#define AID_G200_CLIENTCFG    AID_G200_BEGIN+4


/************************************************************************/
/* �ڲ��궨��                                                           */
/************************************************************************/
#define G200_MAX_CAMERA_NUM               32               //ƽ̨���֧�ֵ�����ͷ��

#define G200_MAX_PERMATRIX_VIDEOIN_NUM    1024
#define G200_MAX_PERMATRIX_VIDEOOUT_NUM   64

#define G200_MAX_PUI_SSN_NUM		      4

#define G200_MAX_ENC_NUM                  5120


const u32 G200_TIMESPACE_PUI_REG_TIMER = 10 * 1000;          //�豸ע�ᳬʱ
const u32 G200_CONN_MPLAT_INTV = 10 * 1000;                  //�����ϼ�ƽ̨��ʱ���
const u32 G200_CONN_REGSRV_INTV = 15 * 1000;
const u32 G200_REG_REGSRV_INTV = 30 * 1000;


/************************************************************************/
/* G200��־��ӡ���ඨ��                                                 */
/************************************************************************/
#define LOG_MOD_NUM      7

#define LOG_SCHED_MOD    0
#define LOG_PUI_MOD      1
#define LOG_CFG_MOD      2
#define LOG_DEVREG_MOD   3
#define LOG_SDK_MOD      4
#define LOG_MAIN_MOD     5
#define LOG_CLIENTCFG_MOD 6

//��������
//����ͷ�Ŀ�������
enum TG200CamCmd{
	G200_CAM_CMD_MOVEUP=1,			//(u8)1		//�����ƶ�
	G200_CAM_CMD_MOVEDOWN,	    	//(u8)2		//�����ƶ�
	G200_CAM_CMD_MOVELEFT,			//(u8)3		//�����ƶ�
	G200_CAM_CMD_MOVERIGHT,			//(u8)4		//�����ƶ�
	G200_CAM_CMD_MOVEHOME,			//(u8)5		//�ع�
	G200_CAM_CMD_MOVESTOP,			//(u8)6		//ֹͣ�ƶ�
	G200_CAM_CMD_ZOOMTELE,		    //(u8)7		//��������ͷ
	G200_CAM_CMD_ZOOMWIDE,			//(u8)8		//��Զ����ͷ
	G200_CAM_CMD_ZOOMSTOP,			//(u8)9		//��Ұ����ֹͣ
	G200_CAM_CMD_FOCUSFAR,		    //(u8)10	//�������Զ
	G200_CAM_CMD_FOCUSNEAR,			//(u8)11	//���������
	G200_CAM_CMD_FOCUSAUTO,			//(u8)12	//�Զ�����
	G200_CAM_CMD_FOCUSSTOP,			//(u8)13	//����ֹͣ
	G200_CAM_CMD_PRESETSET,	    	//(u8)14	//����ͷԤ��
	G200_CAM_CMD_PRESETCALL,			//(u8)15	//������ͷԤ��
	G200_CAM_CMD_CAMERASET,			//(u8)16	//��ʼ������ͷ
	G200_CAM_CMD_BRIGHTUP,			//(u8)17	//�������
	G200_CAM_CMD_BRIGHTDOWN,			//(u8)18	//�������
	G200_CAM_CMD_BRIGHTSTOP,			//(u8)19	//ֹͣ����
	G200_CAM_CMD_NEW,				//(u8)20	//������
	G200_CAM_CMD_APPEND,				//(u8)21	//��������
	G200_CAM_CMD_GOTOPOINT,			//(u8)22	//���Ķ�λ
	G200_CAM_CMD_ZOOMPART,			//(u8)23	//�ֲ��Ŵ�
		
	G200_CAM_CMD_PANSPEED,			//(u8)24	//������ˮƽ�ƶ��ٶ�����������ڲ�ʹ�ã�
	G200_CAM_CMD_TILTSPEED,			//(u8)25	//�����˴�ֱ�ƶ��ٶ�����������ڲ�ʹ�ã�
	G200_CAM_CMD_SETBRIGHT,			//(u8)26	//sony���ȵ�����������ڲ�ʹ�ã�
	G200_CAM_CMD_MATRIX_TRANSIO,		//(u8)27
	G200_CAM_CMD_MATRIX_SWTICH=27,	//(u8)27	//�л���Ƶ
	G200_CAM_CMD_MATRIX_VIDMIX4,		//(u8)28	//����ϳ�2��2
	G200_CAM_CMD_MATRIX_VIDMIX9,		//(u8)29	//����ϳ�3��3
	G200_CAM_CMD_MATRIX_VIDMIX16,	//(u8)30	//����ϳ�4��4
	G200_CAM_CMD_MATRIX_VIDMIXPP,	//(u8)31	//����ϳɻ��л�
		
	G200_CAM_CMD_ZOOMWHOLE,			//(u8)32    //�ֲ���С 
		
	////�˵�//////////////////////////////////////////////////////////////////////	
	G200_CAM_CMD_MENU_UP,			//(u8)33	//�˵���
	G200_CAM_CMD_MENU_DOWN,			//(u8)34	//�˵���
	G200_CAM_CMD_MENU_LEFT,			//(u8)35	//�˵���
	G200_CAM_CMD_MENU_RIGHT,			//(u8)36	//�˵���
		
	G200_CAM_CMD_MENU_SHOW,			//(u8)37	//�˵���ʾ
	G200_CAM_CMD_MENU_HIDE,			//(u8)38	//�˵�����
		
	G200_CAM_CMD_MENU_ENTER,			//(u8)39	//�˵�ȷ��
	G200_CAM_CMD_MENU_EXIT,		    //(u8)40	//�˵�ȡ��
		
	G200_CAM_CMD_PRESETDEL,			//(u8)41	//ɾ������ͷԤ��
	////�����л�//////////////////////////////////////////////////////////////////////
	G200_CAM_CMD_MATRIX_SWTICHASC,	//(u8)42	//�����л�
	G200_CAM_CMD_MATRIX_SWTICHDESC,	//(u8)43	//�����л�
	G200_CAM_CMD_MATRIX_IMAGEHOLD,	//(U8)44	//ͼ�񱣳�
		
	////Ѳ��//////////////////////////////////////////////////////////////////////	
	G200_CAM_CMD_STARTAUTOSCAN=128,  //(u8)128	//��ʼ�Զ�Ѳ��
	G200_CAM_CMD_STOPAUTOSCAN,       //(u8)129	//ֹͣ�Զ�Ѳ��	
	G200_CAM_CMD_STARTPRESETSCAN,	//(u8)130	//��ʼѲ��
	G200_CAM_CMD_STOPPRESETSCAN,     //(u8)131	//ֹͣѲ��
};

/************************************************************************/
/* �ڲ���Ϣ����                                                         */
/************************************************************************/
#define  EV_G200_EVENT_BEGIN   EV_G200_MSG_BGN

enum TPrintMsg
{
	EV_G200_PRINT_MATRIX_INFO_REQ = EV_G200_EVENT_BEGIN,  //��ӡ������Ϣ
	EV_G200_PRINT_VIDEOOUT_RT_REQ,                        //��ӡ�������Ϣ
    EV_G200_PRINT_CAMSTATUS_REQ,                             //��ӡ����ͷ״̬

	EV_G200_PRINT_SWITCHINFO_REQ,                         //��ӡ������Ϣ
	EV_G200_PRINT_LICENSEINFO_REQ,                        //��ӡlicense��Ϣ
	EV_G200_PRINT_G200STATUS_REQ,                         //��ӡg200״̬��Ϣ
	
	END_PRINT_EVENT,
};

enum TInterMsg
{
	EV_G200_SET_INST_ALIAS = EV_G200_EVENT_BEGIN + 150,   //�豸ʵ����������Ϣ����

	EV_G200_VCSCHED_POWERON,                 //�������ĵ���ģ�飬��Ϣ����
	EV_G200_CFGMGR_POWERON,
    EV_G200_SDKSSN_POWERON,                  //����sdkssnģ��,��Ϣ����

	EV_G200_CONN_MPLAT_TIMER,                //�����ϼ�ƽ̨��ʱ��

	EV_G200_PUI_REG_TIMEOUT,                 //puiע�ᳬʱ��ʱ��

	EV_G200_PUI_DISCONNECT_NTY,              //pui����֪ͨ�� ��Ϣ����
	EV_G200_FORCE_PUI_DISCONNECT_CMD,        //ǿ��pui���������Ϣ����

	EV_G200_CFGMGR_GET_BINDRT_CMD,           //֪ͨ���ù���ģ�鷢�Ͱ󶨹�ϵ��Ϣ����Ϣ��Ϊstring(�󶨹�ϵ�ļ�����·��)
	EV_G200_CFGMGR_GET_MATRIX_NTY,           //���ù���ģ���ϱ�������Ϣ֪ͨ��Ϣ����Ϣ��ΪTTotalNum(��Ϣ��������) + N * TMatrixData
	EV_G200_CFGMGR_GET_BINDINRT_NTY,         //���ù���ģ���ϱ�����󶨹�ϵ��Ϣ����Ϣ��ΪTTotalNum(��Ϣ��������) + N * TVideoInItem
	EV_G200_CFGMGR_GET_BINDOUTRT_NTY,        //���ù���ģ���ϱ�����󶨹�ϵ��Ϣ����Ϣ��ΪTTotalNum(��Ϣ��������) + N * TVideoOutItem

	EV_G200_SDK_START_SEND_REQ,              //��������������Ϣ��ΪTPuChn + TTransportAddr(Video) + u8(Mode) + u8(SwitchType) + TTransportAddr(Audio)
	EV_G200_SDK_START_SEND_ACK,              //���������ɹ�����Ϣ��ΪTTransportAddr(rtcp Video) + TTransportAddr(rtcp Audio)
	EV_G200_SDK_START_SEND_NACK,             //��������ʧ�ܣ���Ϣ��ΪTPuChn + TTransportAddr(Video) + u8(Mode) + u8(SwitchType) + TTransportAddr(Audio)

	EV_G200_SDK_STOP_SEND_REQ,               //����ֹͣ���������� ��Ϣ��ΪTPuChn + u8(Mode)
	EV_G200_SDK_STOP_SEND_ACK,               //ֹͣ���������ɹ��� ��Ϣ��ΪTPuChn + u8(Mode)
	EV_G200_SDK_STOP_SEND_NACK,              //ֹͣ��������ʧ�ܣ� ��Ϣ��ΪTPuChn + u8(Mode)

	EV_G200_DISCONNECT_WITH_MPLAT_NTY,       //���ϼ�ƽ̨����֪ͨ����Ϣ����
    EV_G200_CONNECT_WITH_MPLAT_NTY,          //���ϼ�ƽ̨����֪ͨ����Ϣ����

	EV_G200_CONN_REGSRV_TIMER,
	EV_G200_REG_REGSRV_TIMER,

	EV_G200_CFGMGR_ADD_BINDINRT_NTY,         //���Ӿ�������󶨹�ϵ��Ϣ֪ͨ����Ϣ��ΪTTotalNum(��Ϣ��������) + N * TVideoInItem

	EV_G200_SDK_STOP_RCV_DEV_STREAM_NTY,     //ֹͣ����ĳ���豸����������Ϣ��TPuChn
    EV_G200_YJCTRL_CMD,                      //�ƾ�����
};
/************************************************************************/
/* �ڲ��ṹ����                                                         */
/************************************************************************/



//G200��ӡ����
void G200Log(u8 byLogModule, u8 byLogLevel, const char* szFormat, ...);

//IP��ַת��
BOOL32 IPtoString( u32 dwIP, char achIPStr[], u32 dwLen);

BOOL32 UnifyDir( s8 *pszNewDir, u32 dwDirLen);

#endif