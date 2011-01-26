/*****************************************************************************
   ģ����      : KDV system
   �ļ���      : kdvsys.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDV�����������궨��
   ����        : ����
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2002/01/30  0.9         ����        ����
   2004/03/03  3.0         jianghy	   �޸�
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

/* ����س� */
#if defined( WIN32 )
    #define     STR_RETURN          (const s8*)"\r\n"
#elif defined( VXWORKS_SIMU )
    #define     STR_RETURN          (const s8*)"\r\n"
#else            /* VxWorks */
    #define     STR_RETURN          (const s8*)"\n"
#endif

/* SNMP����FTP�ļ��ָ�� */
#define CHAR_FTPFILE_SEPARATOR		(char)'|'	/* ��ͬ�ļ�֮��ָ��� */
#define CHAR_FTPFILETYPE_SEPARATOR	(char)':'	/* �ļ���������֮��ָ��� */
#define CHAR_EXE_TYPE				(char)'E'	/* �������ļ����� */
#define CHAR_CFG_TYPE				(char)'C'	/* �����ļ����� */
#define CHAR_RES_TYPE				(char)'R'   /* ��Դ�ļ����� */
#define CHAR_WEB_TYPE				(char)'W'	/* web�ļ����� */
#define CHAR_PIC_TYPE				(char)'P'	/* ͼƬ�ļ����� */

/* ����Profile�ļ��б����������ַ������� */
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

/* ·������ */
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


/* semTake��ʱ */
#define WAIT_SEM_TIMEOUT    2000    /* ticks */

/* SNMP PORT macro */
#define  SNMP_PORT          161         /* port for snmp */
#define  TRAP_PORT          162         /* port for snmp traps */

/* SNMP TRAP�Զ���specificTrap */
#define SPECIFICTRAP_COLD_RESTART       1L
#define SPECIFICTRAP_POWERON			1L   //add by hzc
#define SPECIFICTRAP_POWEROFF			2L
#define SPECIFICTRAP_ALARM_GENERATE     3L
#define SPECIFICTRAP_ALARM_RESTORE      4L
#define SPECIFICTRAP_LED_STATE			5L
#define SPECIFICTRAP_CONF_STATE			6L
#define SPECIFICTRAP_MT_STATE			7L
#define SPECIFICTRAP_AUTODETECT                 8L   //�Զ�̽�� //add by hzc

/* PrintMsg() typeȡֵ */
#define MESSAGE_INFO            (u8  )0x1
#define MESSAGE_WARNING         (u8  )0x2
#define MESSAGE_ERROR           (u8  )0x3

#define MESSAGE_BUFFER_LEN      (u16)1024   /* ��Ϣ�������󳤶� */

#ifndef MAX_PATH
    /* �������·������ */
    #define MAX_PATH            (u16)256
#endif

#ifndef MAX_LINE_LEN
    /* ����һ�е���󳤶� */
    #define MAX_LINE_LEN        (u16)1024
#endif

#define MAX_SECTIONNAME_LEN     (u16)50
#define MAX_KEYNAME_LEN         (u16)50
#define MAX_VALUE_LEN           (u16)255

/* OpenProfile() byOpenMode ȡֵ */
#define PROFILE_READONLY        (u8  )0x1       /*ֻ��*/
#define PROFILE_WRITE           (u8  )0x2       /*��д*/
#define PROFILE_READWRITE       PROFILE_WRITE   /*��д*/
#define PROFILE_WRITEONLY       (u8  )0x4       /*ֻд*/

#define PROFILE_MAX_OPEN        (u16)20        /* ���ļ��������Ŀ */

typedef u32                   tKeyHandle;

#define INVALID_KEYHANDLE       (u16)0xFFFF    /* ��Ч���ļ���� */

/* Profile�ļ�ע�ͷ����� */
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

/* -----------------------  ϵͳ���ú���  ----------------------------- */
#define LOWORD16(l)     ((u16)(l))
#define HIWORD16(l)     ((u16)(((u32)(l) >> 16) & 0xFFFF))
#define MAKEDWORD(l,h)  ((u32)(((u16)(l)) | ((u32)((u16)(h))) << 16))

#ifndef _WINDEF_  /* Ϊ���ܹ���Windows��ʹ�� */
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

/* ��ȡ�����ļ��ຯ�� */
DllExport BOOL32 GetRegKeyInt( const s8* lpszProfileName,    /* �ļ�����������·����*/
                   const s8* lpszSectionName,      /* Profile�еĶ���   */
                   const s8* lpszKeyName,          /* ���ڵĹؼ�����    */
                   const s32 sdwDefault,     /* ʧ��ʱ���ص�Ĭ��ֵ*/
                   s32  *sdwReturnValue      /* ����ֵ            */
                 );
    /* �������ļ��ж�ȡһ������ֵ */

DllExport BOOL32 GetRegKeyStringTable( const s8* lpszProfileName,    /* �ļ�����������·����*/
                   const s8* lpszSectionName,      /* Profile�еĶ���   */        
                   const s8* lpszDefault,          /* ʧ��ʱ���ص�Ĭ��ֵ*/
                   s8* *lpszEntryArray,       /* �����ַ�������ָ�� */
                   u32 *dwEntryNum,           /* �ַ���������Ŀ���ɹ��󷵻��ַ�������ʵ����
                                                Ŀ���紫�����Ŀ���������ļ�¼���贫�� */
                   u32 dwBufSize              /* �����ִ��ĳ��ȣ��粻����ضϣ���
                                                ��һ���ֽ�����'\0'    */
                 );
    /* �������ļ��ж�ȡһ�ű��������������
 */

DllExport BOOL32 GetRegKeyString( const s8* lpszProfileName, /* �ļ�����������·����*/
                      const s8* lpszSectionName,   /* Profile�еĶ���   */
                      const s8* lpszKeyName,       /* ���ڵĹؼ�����    */
                      const s8* lpszDefault,       /* ʧ��ʱ���ص�Ĭ��ֵ*/
                      s8* lpszReturnValue,    /* �����ִ�          */
                      u32 dwBufSize           /* �����ִ��ĳ��ȣ��粻����ضϣ���
                                                ��һ���ֽ�����'\0'    */
                    );
    /* �������ļ��ж�ȡһ�ַ�����ֵ */
    
DllExport BOOL32 GetRegKeyString2( FILE *stream,  /* �ļ���� */
                      const s8* lpszSectionName,   /* Profile�еĶ���   */
                      const s8* lpszKeyName,       /* ���ڵĹؼ�����    */
                      const s8* lpszDefault,       /* ʧ��ʱ���ص�Ĭ��ֵ*/
                      s8* lpszReturnValue,    /* �����ִ�          */
                      u32 dwBufSize           /* �����ִ��ĳ��ȣ��粻����ضϣ���
                                                ��һ���ֽ�����'\0'    */
                    );
    /* �����ļ�����������ļ��ж�ȡһ�ַ���ֵ�����ļ�����"rb"��ʽ�� */
    
DllExport BOOL32 SetRegKeyInt( const s8* lpszProfileName,    /* �ļ�����������·����*/
                   const s8* lpszSectionName,      /* Profile�еĶ���   */
                   const s8* lpszKeyName,          /* ���ڵĹؼ�����    */
                   const s32  sdwValue       /* �µ�����ֵ        */
                 );
    /* �������ļ���д��һ�µ������͵�ֵ��������������Զ����� */

DllExport BOOL32 SetRegKeyString( const s8* lpszProfileName, /* �ļ�����������·����*/
                      const s8* lpszSectionName,   /* Profile�еĶ���   */
                      const s8* lpszKeyName,       /* ���ڵĹؼ�����    */
                      const s8* lpszValue          /* �µ��ִ�ֵ, NULL���ʾɾ������ */
                    );
    /* �������ļ���д��һ�µ��ַ����͵�ֵ��������������Զ����� */

DllExport BOOL32 DeleteRegKey( const s8* lpszProfileName,    /* �ļ�����������·����*/
                      const s8* lpszSectionName,   /* Profile�еĶ���   */
                      const s8* lpszKeyName        /* ���ڵĹؼ�����    */
                    );
    /* �������ļ���ɾ��һ��Key */

#ifndef WIN32
DllExport BOOL32 ReadRouteTable( const char * lpszProfileName );
#endif
	/* �������ļ��ж�ȡ·�ɱ����� */

/* �ַ��������ຯ�� */
DllExport void Trim( s8* lpszSrc );        /* ȥ���ַ�����ǰ���������ո��Լ�TAB�� */
DllExport void StrUpper( s8* lpszSrc );    /* �ַ���ת���ɴ�д */
DllExport void StrLower( s8* lpszSrc );    /* �ַ���ת����Сд */

/* �ڴ洦����*/
/*====================================================================
    ����        ���ͷŵ���̬���루malloc���ı�ṹ�ڴ�
    �������˵����void **ppMem, ָ������ָ��
                  u32 dwEntryNum, ָ�������С
    ����ֵ˵��  ��void
====================================================================*/
/* DllExport void TableMemoryFree( void **ppMem, u32 dwEntryNum ); */

/*====================================================================
    ����        ����֤�Ƿ�Ϸ�����
    �������˵����u16 wYear, ��
                  u8 byMonth, ��
                  u8 byDay, ��
                  u8 byHour, ʱ
                  u8 byMin, ��
                  u8 bySec, ��
    ����ֵ˵��  ���Ϸ�����TRUE�����𷵻�FALSE
====================================================================*/
DllExport BOOL32 IsValidTime( u16 wYear, u8 byMonth, u8 byDay, 
                           u8 byHour, u8 byMin, u8 bySec );
/*====================================================================
    ������      ��DeleteSec
    ����        ��ɾ��һ�����ΰ��������µ���������
    �㷨ʵ��    ����λ��ָ�����ε���ʼλ�ã�Ȼ�������һ�����ε���ʼλ��
                  ɾ����λ��֮�������
    ����ȫ�ֱ�������
    �������˵����const s8* pszFileNam, �ļ���
                  const s8* pszSecName,��������
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    2010/03/01    1.0         liqingping        ����
====================================================================*/
DllExport BOOL32 DeleteSec(const s8* pszFileName,const s8* pszSecName);
/*====================================================================
    ������      ��GetNextLineValue
    ����        ����ȡһ�����ε���һ�е��ַ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����g_pStream
    �������˵����const s8* pszFileNam, �ļ���
                  const s8* pszSecName,��������
				  s8* pchKeyBuf,��������Ļ�����
				  s32 nKeyMax,��������С
                  s8* pchValueBuf,�����ֵ�Ļ�����
				  s32 nKeyMax,������ʵ�ʵĴ�С
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    2010/03/01    1.0      liqingping        ����
====================================================================*/
DllExport BOOL32 GetNextLineString(const s8* pszFileName, const s8* pszSecName,s8* pchKeyBuf,s32 nKeyMax,
								  s8* pchValueBuf,s32 nValueMax);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _KDV_SYS_H_ */

/* end of file sys.h */

