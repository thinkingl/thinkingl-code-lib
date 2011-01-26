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
/* 内部APP号定义                                                        */
/************************************************************************/
#define AID_G200_BEGIN            200
#define AID_G200_VCSCHED      AID_G200_BEGIN+1
#define AID_G200_SDK          AID_G200_BEGIN+2
#define AID_G200_CFG          AID_G200_BEGIN+3
#define AID_G200_CLIENTCFG    AID_G200_BEGIN+4


/************************************************************************/
/* 内部宏定义                                                           */
/************************************************************************/
#define G200_MAX_CAMERA_NUM               32               //平台最大支持的摄像头数

#define G200_MAX_PERMATRIX_VIDEOIN_NUM    1024
#define G200_MAX_PERMATRIX_VIDEOOUT_NUM   64

#define G200_MAX_PUI_SSN_NUM		      4

#define G200_MAX_ENC_NUM                  5120


const u32 G200_TIMESPACE_PUI_REG_TIMER = 10 * 1000;          //设备注册超时
const u32 G200_CONN_MPLAT_INTV = 10 * 1000;                  //连接上级平台超时间隔
const u32 G200_CONN_REGSRV_INTV = 15 * 1000;
const u32 G200_REG_REGSRV_INTV = 30 * 1000;


/************************************************************************/
/* G200日志打印分类定义                                                 */
/************************************************************************/
#define LOG_MOD_NUM      7

#define LOG_SCHED_MOD    0
#define LOG_PUI_MOD      1
#define LOG_CFG_MOD      2
#define LOG_DEVREG_MOD   3
#define LOG_SDK_MOD      4
#define LOG_MAIN_MOD     5
#define LOG_CLIENTCFG_MOD 6

//控制命令
//摄像头的控制命令
enum TG200CamCmd{
	G200_CAM_CMD_MOVEUP=1,			//(u8)1		//向上移动
	G200_CAM_CMD_MOVEDOWN,	    	//(u8)2		//向下移动
	G200_CAM_CMD_MOVELEFT,			//(u8)3		//向左移动
	G200_CAM_CMD_MOVERIGHT,			//(u8)4		//向右移动
	G200_CAM_CMD_MOVEHOME,			//(u8)5		//回归
	G200_CAM_CMD_MOVESTOP,			//(u8)6		//停止移动
	G200_CAM_CMD_ZOOMTELE,		    //(u8)7		//拉近摄像头
	G200_CAM_CMD_ZOOMWIDE,			//(u8)8		//拉远摄像头
	G200_CAM_CMD_ZOOMSTOP,			//(u8)9		//视野调节停止
	G200_CAM_CMD_FOCUSFAR,		    //(u8)10	//将焦距调远
	G200_CAM_CMD_FOCUSNEAR,			//(u8)11	//将焦距调近
	G200_CAM_CMD_FOCUSAUTO,			//(u8)12	//自动调焦
	G200_CAM_CMD_FOCUSSTOP,			//(u8)13	//调焦停止
	G200_CAM_CMD_PRESETSET,	    	//(u8)14	//摄象头预存
	G200_CAM_CMD_PRESETCALL,			//(u8)15	//调摄象头预存
	G200_CAM_CMD_CAMERASET,			//(u8)16	//初始化摄像头
	G200_CAM_CMD_BRIGHTUP,			//(u8)17	//画面调亮
	G200_CAM_CMD_BRIGHTDOWN,			//(u8)18	//画面调暗
	G200_CAM_CMD_BRIGHTSTOP,			//(u8)19	//停止调亮
	G200_CAM_CMD_NEW,				//(u8)20	//新命令
	G200_CAM_CMD_APPEND,				//(u8)21	//附加命令
	G200_CAM_CMD_GOTOPOINT,			//(u8)22	//中心定位
	G200_CAM_CMD_ZOOMPART,			//(u8)23	//局部放大
		
	G200_CAM_CMD_PANSPEED,			//(u8)24	//艾立克水平移动速度设置命令，（内部使用）
	G200_CAM_CMD_TILTSPEED,			//(u8)25	//艾立克垂直移动速度设置命令，（内部使用）
	G200_CAM_CMD_SETBRIGHT,			//(u8)26	//sony亮度调节启动命令（内部使用）
	G200_CAM_CMD_MATRIX_TRANSIO,		//(u8)27
	G200_CAM_CMD_MATRIX_SWTICH=27,	//(u8)27	//切换视频
	G200_CAM_CMD_MATRIX_VIDMIX4,		//(u8)28	//画面合成2×2
	G200_CAM_CMD_MATRIX_VIDMIX9,		//(u8)29	//画面合成3×3
	G200_CAM_CMD_MATRIX_VIDMIX16,	//(u8)30	//画面合成4×4
	G200_CAM_CMD_MATRIX_VIDMIXPP,	//(u8)31	//画面合成画中画
		
	G200_CAM_CMD_ZOOMWHOLE,			//(u8)32    //局部缩小 
		
	////菜单//////////////////////////////////////////////////////////////////////	
	G200_CAM_CMD_MENU_UP,			//(u8)33	//菜单上
	G200_CAM_CMD_MENU_DOWN,			//(u8)34	//菜单下
	G200_CAM_CMD_MENU_LEFT,			//(u8)35	//菜单左
	G200_CAM_CMD_MENU_RIGHT,			//(u8)36	//菜单右
		
	G200_CAM_CMD_MENU_SHOW,			//(u8)37	//菜单显示
	G200_CAM_CMD_MENU_HIDE,			//(u8)38	//菜单隐藏
		
	G200_CAM_CMD_MENU_ENTER,			//(u8)39	//菜单确定
	G200_CAM_CMD_MENU_EXIT,		    //(u8)40	//菜单取消
		
	G200_CAM_CMD_PRESETDEL,			//(u8)41	//删除摄象头预存
	////矩阵切换//////////////////////////////////////////////////////////////////////
	G200_CAM_CMD_MATRIX_SWTICHASC,	//(u8)42	//正向切换
	G200_CAM_CMD_MATRIX_SWTICHDESC,	//(u8)43	//反向切换
	G200_CAM_CMD_MATRIX_IMAGEHOLD,	//(U8)44	//图像保持
		
	////巡航//////////////////////////////////////////////////////////////////////	
	G200_CAM_CMD_STARTAUTOSCAN=128,  //(u8)128	//开始自动巡航
	G200_CAM_CMD_STOPAUTOSCAN,       //(u8)129	//停止自动巡航	
	G200_CAM_CMD_STARTPRESETSCAN,	//(u8)130	//开始巡检
	G200_CAM_CMD_STOPPRESETSCAN,     //(u8)131	//停止巡检
};

/************************************************************************/
/* 内部消息定义                                                         */
/************************************************************************/
#define  EV_G200_EVENT_BEGIN   EV_G200_MSG_BGN

enum TPrintMsg
{
	EV_G200_PRINT_MATRIX_INFO_REQ = EV_G200_EVENT_BEGIN,  //打印矩阵信息
	EV_G200_PRINT_VIDEOOUT_RT_REQ,                        //打印输出绑定信息
    EV_G200_PRINT_CAMSTATUS_REQ,                             //打印摄像头状态

	EV_G200_PRINT_SWITCHINFO_REQ,                         //打印交换信息
	EV_G200_PRINT_LICENSEINFO_REQ,                        //打印license信息
	EV_G200_PRINT_G200STATUS_REQ,                         //打印g200状态信息
	
	END_PRINT_EVENT,
};

enum TInterMsg
{
	EV_G200_SET_INST_ALIAS = EV_G200_EVENT_BEGIN + 150,   //设备实例别名，消息体无

	EV_G200_VCSCHED_POWERON,                 //启动中心调度模块，消息体无
	EV_G200_CFGMGR_POWERON,
    EV_G200_SDKSSN_POWERON,                  //启动sdkssn模块,消息体无

	EV_G200_CONN_MPLAT_TIMER,                //连接上级平台定时器

	EV_G200_PUI_REG_TIMEOUT,                 //pui注册超时定时器

	EV_G200_PUI_DISCONNECT_NTY,              //pui断链通知， 消息体无
	EV_G200_FORCE_PUI_DISCONNECT_CMD,        //强制pui断链命令，消息体无

	EV_G200_CFGMGR_GET_BINDRT_CMD,           //通知配置管理模块发送绑定关系信息，消息体为string(绑定关系文件所在路径)
	EV_G200_CFGMGR_GET_MATRIX_NTY,           //配置管理模块上报矩阵信息通知消息，消息体为TTotalNum(消息内容数量) + N * TMatrixData
	EV_G200_CFGMGR_GET_BINDINRT_NTY,         //配置管理模块上报输入绑定关系消息，消息体为TTotalNum(消息内容数量) + N * TVideoInItem
	EV_G200_CFGMGR_GET_BINDOUTRT_NTY,        //配置管理模块上报输出绑定关系消息，消息体为TTotalNum(消息内容数量) + N * TVideoOutItem

	EV_G200_SDK_START_SEND_REQ,              //请求发送码流，消息体为TPuChn + TTransportAddr(Video) + u8(Mode) + u8(SwitchType) + TTransportAddr(Audio)
	EV_G200_SDK_START_SEND_ACK,              //发送码流成功，消息体为TTransportAddr(rtcp Video) + TTransportAddr(rtcp Audio)
	EV_G200_SDK_START_SEND_NACK,             //发送码流失败，消息体为TPuChn + TTransportAddr(Video) + u8(Mode) + u8(SwitchType) + TTransportAddr(Audio)

	EV_G200_SDK_STOP_SEND_REQ,               //请求停止发送码流， 消息体为TPuChn + u8(Mode)
	EV_G200_SDK_STOP_SEND_ACK,               //停止发送码流成功， 消息体为TPuChn + u8(Mode)
	EV_G200_SDK_STOP_SEND_NACK,              //停止码流发送失败， 消息体为TPuChn + u8(Mode)

	EV_G200_DISCONNECT_WITH_MPLAT_NTY,       //与上级平台断链通知，消息体无
    EV_G200_CONNECT_WITH_MPLAT_NTY,          //与上级平台连接通知，消息体无

	EV_G200_CONN_REGSRV_TIMER,
	EV_G200_REG_REGSRV_TIMER,

	EV_G200_CFGMGR_ADD_BINDINRT_NTY,         //增加矩阵输入绑定关系消息通知，消息体为TTotalNum(消息内容数量) + N * TVideoInItem

	EV_G200_SDK_STOP_RCV_DEV_STREAM_NTY,     //停止接收某个设备的码流，消息体TPuChn
    EV_G200_YJCTRL_CMD,                      //云镜控制
};
/************************************************************************/
/* 内部结构定义                                                         */
/************************************************************************/



//G200打印函数
void G200Log(u8 byLogModule, u8 byLogLevel, const char* szFormat, ...);

//IP地址转换
BOOL32 IPtoString( u32 dwIP, char achIPStr[], u32 dwLen);

BOOL32 UnifyDir( s8 *pszNewDir, u32 dwDirLen);

#endif