/*****************************************************************************
   模块名      : KDV system
   文件名      : kdvsys.h
   相关文件    : 
   文件实现功能: KDV公共函数及宏定义
   作者        : 李屹
   版本        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2002/01/30  0.9         李屹        创建
   2004/03/03  3.0         jianghy	   修改
******************************************************************************/
#ifndef _KDV_SYS_H_
#define _KDV_SYS_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include "kdvtype.h"

#ifdef WIN32
    #define DllExport __declspec(dllexport)
#else     /* VxWorks*/
    #define DllExport
#endif

/* 定义回车 */
#if defined( WIN32 )
    #define     STR_RETURN          (const s8*)"\r\n"
#elif defined( VXWORKS_SIMU )
    #define     STR_RETURN          (const s8*)"\r\n"
#else            /* VxWorks */
    #define     STR_RETURN          (const s8*)"\n"
#endif

/* SNMP拷贝FTP文件分割符 */
#define CHAR_FTPFILE_SEPARATOR		(char)'|'	/* 不同文件之间分隔符 */
#define CHAR_FTPFILETYPE_SEPARATOR	(char)':'	/* 文件名和类型之间分隔符 */
#define CHAR_EXE_TYPE				(char)'E'	/* 二进制文件类型 */
#define CHAR_CFG_TYPE				(char)'C'	/* 配置文件类型 */
#define CHAR_RES_TYPE				(char)'R'   /* 资源文件类型 */
#define CHAR_WEB_TYPE				(char)'W'	/* web文件类型 */
#define CHAR_PIC_TYPE				(char)'P'	/* 图片文件类型 */

/* 定义Profile文件中表格数据相关字符串常量 */
#define STR_ENTRY_NUM			(const s8*)"EntryNum"
#define STR_ENTRY				(const s8*)"Entry"
#define SECTION_ipRouteTable	(const s8*)"ipRouteTable"
#define FIELD_ipRouteDest		(const s8*)"ipRouteDest"
#define FIELD_ipRouteNextHop	(const s8*)"ipRouteNextHop"
#define FIELD_ipRouteMask		(const s8*)"ipRouteMask"

/* OID length */
#define OID_IP_ADDR_LEN     4 
#define OID_ID_LEN          1 

/* maximum size of physical address */
#define MAXSIZE_PHYADDR     16

typedef u8   MAC_TYPE[6];

/* 路径定义 */
#if defined( WIN32 )    /* WIN32 */
#define PATH_FTP            (const s8*)"ftp"
#define PATH_CONFIG         (const s8*)"conf"
#define PATH_BIN			(const s8*)"."
#define PATH_RES			(const s8*)"res"
#define PATH_WEB			(const s8*)"webfiles/doc"
#define PATH_PIC			(const s8*)"webfiles/doc/images"
#define PATH_XML			(const s8*)"webfiles" //add hzc
#define PATH_DOC			(const s8*)"doc"      //add hzc
#elif defined( VXWORKS_SIMU )
#define PATH_FTP            (const s8*)"ftp"
#define PATH_CONFIG         (const s8*)"conf"
#define PATH_BIN			(const s8*)"."
#define PATH_RES			(const s8*)"res"
#else                   /* vxWorks */
#define PATH_FTP            (const s8*)"/ftp"
#define PATH_CONFIG         (const s8*)"/conf"
#define PATH_BIN			(const s8*)""
#define PATH_RES			(const s8*)"res"
#define PATH_WEB			(const s8*)"/webfiles/doc"
#define PATH_PIC			(const s8*)"/webfiles/doc/images"
#define PATH_XML			(const s8*)"/webfiles"//add hzc
#define PATH_DOC			(const s8*)"doc"    //add hzc

#endif


/* semTake超时 */
#define WAIT_SEM_TIMEOUT    2000    /* ticks */

/* SNMP PORT macro */
#define  SNMP_PORT          161         /* port for snmp */
#define  TRAP_PORT          162         /* port for snmp traps */

/* SNMP TRAP自定义specificTrap */
#define SPECIFICTRAP_COLD_RESTART       1L
#define SPECIFICTRAP_POWERON			1L   //add by hzc
#define SPECIFICTRAP_POWEROFF			2L
#define SPECIFICTRAP_ALARM_GENERATE     3L
#define SPECIFICTRAP_ALARM_RESTORE      4L
#define SPECIFICTRAP_LED_STATE			5L
#define SPECIFICTRAP_CONF_STATE			6L
#define SPECIFICTRAP_MT_STATE			7L
#define SPECIFICTRAP_AUTODETECT                 8L   //自动探测 //add by hzc

/* PrintMsg() type取值 */
#define MESSAGE_INFO            (u8  )0x1
#define MESSAGE_WARNING         (u8  )0x2
#define MESSAGE_ERROR           (u8  )0x3

#define MESSAGE_BUFFER_LEN      (u16)1024   /* 消息缓冲的最大长度 */

#ifndef MAX_PATH
    /* 定义最大路径长度 */
    #define MAX_PATH            (u16)256
#endif

#ifndef MAX_LINE_LEN
    /* 定义一行的最大长度 */
    #define MAX_LINE_LEN        (u16)1024
#endif

#define MAX_SECTIONNAME_LEN     (u16)50
#define MAX_KEYNAME_LEN         (u16)50
#define MAX_VALUE_LEN           (u16)255

/* OpenProfile() byOpenMode 取值 */
#define PROFILE_READONLY        (u8  )0x1       /*只读*/
#define PROFILE_WRITE           (u8  )0x2       /*读写*/
#define PROFILE_READWRITE       PROFILE_WRITE   /*读写*/
#define PROFILE_WRITEONLY       (u8  )0x4       /*只写*/

#define PROFILE_MAX_OPEN        (u16)20        /* 打开文件的最大数目 */

typedef u32                   tKeyHandle;

#define INVALID_KEYHANDLE       (u16)0xFFFF    /* 无效的文件句柄 */

/* Profile文件注释符定义 */
#define PROFILE_COMMENT1        (const s8*)";"
#define PROFILE_COMMENT2        (const s8*)"//"
#define PROFILE_COMMENT3        (const s8*)"--"
#define PROFILE_COMMENT4        (const s8*)"#"

/* struct definition */
typedef struct
{
    u16    dwAddrLen;                      /* Length of address */
    u8   phyAddress[MAXSIZE_PHYADDR];    /* physical address value */
} PHYADDR_STRUCT;

typedef struct
{
    u8   byDtiNo;
    u8   byE1No;
} MCU_E1_STRUCT;

typedef struct
{
    u8   byDtiNo;
    u8   byE1No;
    u8   byTsNo;
} MCU_TS_STRUCT;

typedef struct
{
    u32   dwTsNumber;                 /* TS number */
    MCU_TS_STRUCT   *ptMcuTs;           /* MCU TS array */
} MCU_TS_ARRAY_STRUCT;

typedef struct
{
    u8   byE1No;
    u8   byTsNo;
} MT_TS_STRUCT;

/* -----------------------  系统公用函数  ----------------------------- */
#define LOWORD16(l)     ((u16)(l))
#define HIWORD16(l)     ((u16)(((u32)(l) >> 16) & 0xFFFF))
#define MAKEDWORD(l,h)  ((u32)(((u16)(l)) | ((u32)((u16)(h))) << 16))

#ifndef _WINDEF_  /* 为了能够有Windows下使用 */
    #define LOBYTE(w)       ((u8)(w))
    #define HIBYTE(w)       ((u8)(((u16)(w) >> 8) & 0xFF))
    #define MAKEWORD(l,h)  ((u16)(((u8)(l)) | ((u16)((u8)(h))) << 8))
#endif

#ifdef _DEBUG 
	#undef ASSERT
	#define ASSERT(exp)    \
        { \
	      if ( !( exp ) ) \
		  { \
            printf((s8*)"Assert File %s, Line %d (%s)\n", \
	                 __FILE__, __LINE__, (s8*)#exp ); \
		  } \
        }
#else
    #undef ASSERT
    #define ASSERT( exp )    {}
#endif

/* 读取配置文件类函数 */
DllExport BOOL32 GetRegKeyInt( const s8* lpszProfileName,    /* 文件名（含绝对路径）*/
                   const s8* lpszSectionName,      /* Profile中的段名   */
                   const s8* lpszKeyName,          /* 段内的关键字名    */
                   const s32 sdwDefault,     /* 失败时返回的默认值*/
                   s32  *sdwReturnValue      /* 返回值            */
                 );
    /* 从配置文件中读取一整数型值 */

DllExport BOOL32 GetRegKeyStringTable( const s8* lpszProfileName,    /* 文件名（含绝对路径）*/
                   const s8* lpszSectionName,      /* Profile中的段名   */        
                   const s8* lpszDefault,          /* 失败时返回的默认值*/
                   s8* *lpszEntryArray,       /* 返回字符串数组指针 */
                   u32 *dwEntryNum,           /* 字符串数组数目，成功后返回字符串数组实际数
                                                目，如传入的数目不够则多余的记录不予传回 */
                   u32 dwBufSize              /* 返回字串的长度，如不够则截断，最
                                                后一个字节填上'\0'    */
                 );
    /* 从配置文件中读取一张表里的若干行数据
 */

DllExport BOOL32 GetRegKeyString( const s8* lpszProfileName, /* 文件名（含绝对路径）*/
                      const s8* lpszSectionName,   /* Profile中的段名   */
                      const s8* lpszKeyName,       /* 段内的关键字名    */
                      const s8* lpszDefault,       /* 失败时返回的默认值*/
                      s8* lpszReturnValue,    /* 返回字串          */
                      u32 dwBufSize           /* 返回字串的长度，如不够则截断，最
                                                后一个字节填上'\0'    */
                    );
    /* 从配置文件中读取一字符串型值 */
    
DllExport BOOL32 GetRegKeyString2( FILE *stream,  /* 文件句柄 */
                      const s8* lpszSectionName,   /* Profile中的段名   */
                      const s8* lpszKeyName,       /* 段内的关键字名    */
                      const s8* lpszDefault,       /* 失败时返回的默认值*/
                      s8* lpszReturnValue,    /* 返回字串          */
                      u32 dwBufSize           /* 返回字串的长度，如不够则截断，最
                                                后一个字节填上'\0'    */
                    );
    /* 根据文件句柄从配置文件中读取一字符串值，该文件需以"rb"方式打开 */
    
DllExport BOOL32 SetRegKeyInt( const s8* lpszProfileName,    /* 文件名（含绝对路径）*/
                   const s8* lpszSectionName,      /* Profile中的段名   */
                   const s8* lpszKeyName,          /* 段内的关键字名    */
                   const s32  sdwValue       /* 新的整数值        */
                 );
    /* 往配置文件中写入一新的整数型的值，如果不存在则自动创建 */

DllExport BOOL32 SetRegKeyString( const s8* lpszProfileName, /* 文件名（含绝对路径）*/
                      const s8* lpszSectionName,   /* Profile中的段名   */
                      const s8* lpszKeyName,       /* 段内的关键字名    */
                      const s8* lpszValue          /* 新的字串值, NULL则表示删除改行 */
                    );
    /* 往配置文件中写入一新的字符串型的值，如果不存在则自动创建 */

DllExport BOOL32 DeleteRegKey( const s8* lpszProfileName,    /* 文件名（含绝对路径）*/
                      const s8* lpszSectionName,   /* Profile中的段名   */
                      const s8* lpszKeyName        /* 段内的关键字名    */
                    );
    /* 从配置文件中删除一行Key */

#ifndef WIN32
DllExport BOOL32 ReadRouteTable( const char * lpszProfileName );
#endif
	/* 从配置文件中读取路由表并设置 */

/* 字符串处理类函数 */
DllExport void Trim( s8* lpszSrc );        /* 去除字符串的前导及后续空格以及TAB键 */
DllExport void StrUpper( s8* lpszSrc );    /* 字符串转换成大写 */
DllExport void StrLower( s8* lpszSrc );    /* 字符串转换成小写 */

/* 内存处理函数*/
/*====================================================================
    功能        ：释放掉动态申请（malloc）的表结构内存
    输入参数说明：void **ppMem, 指针数组指针
                  u32 dwEntryNum, 指针数组大小
    返回值说明  ：void
====================================================================*/
/* DllExport void TableMemoryFree( void **ppMem, u32 dwEntryNum ); */

/*====================================================================
    功能        ：验证是否合法日期
    输入参数说明：u16 wYear, 年
                  u8 byMonth, 月
                  u8 byDay, 日
                  u8 byHour, 时
                  u8 byMin, 分
                  u8 bySec, 秒
    返回值说明  ：合法返回TRUE，负责返回FALSE
====================================================================*/
DllExport BOOL32 IsValidTime( u16 wYear, u8 byMonth, u8 byDay, 
                           u8 byHour, u8 byMin, u8 bySec );
/*====================================================================
    函数名      ：DeleteSec
    功能        ：删除一个区段包括区段下的所有数据
    算法实现    ：定位到指定区段的起始位置，然后查找下一个区段的起始位置
                  删除两位置之间的数据
    引用全局变量：无
    输入参数说明：const s8* pszFileNam, 文件名
                  const s8* pszSecName,区段名称
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    2010/03/01    1.0         liqingping        创建
====================================================================*/
DllExport BOOL32 DeleteSec(const s8* pszFileName,const s8* pszSecName);
/*====================================================================
    函数名      ：GetNextLineValue
    功能        ：获取一个区段的下一行的字符串
    算法实现    ：
    引用全局变量：g_pStream
    输入参数说明：const s8* pszFileNam, 文件名
                  const s8* pszSecName,区段名称
				  s8* pchKeyBuf,保存键名的缓冲区
				  s32 nKeyMax,缓冲区大小
                  s8* pchValueBuf,保存键值的缓冲区
				  s32 nKeyMax,缓冲区实际的大小
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    2010/03/01    1.0      liqingping        创建
====================================================================*/
DllExport BOOL32 GetNextLineString(const s8* pszFileName, const s8* pszSecName,s8* pchKeyBuf,s32 nKeyMax,
								  s8* pchValueBuf,s32 nValueMax);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _KDV_SYS_H_ */

/* end of file sys.h */

