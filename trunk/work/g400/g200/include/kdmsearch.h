#ifndef _KDMSEARCH_H_
#define _KDMSEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "kdvtype.h"

typedef struct
{
    u32 m_dwTargetIP;
    u32 m_dwNewIP;
    u32 m_dwNewMask;
    u32 m_dwNewGW;
} TSetNewIpInfo;

// �ص������ṹ
typedef void (*FEvSetDevIp)(void* pUserHandle, TSetNewIpInfo tNewIpInfo);

/************************************************************************/
/* �������ܣ���������ģ��                                               */
/* ����˵����pfunSetIp ���� �޸�IP��ַ�ص�ָ��                          */
/* ��������ֵ��0 ���� �ɹ�����0 ���� ʧ��                               */
/* ����˵��������ģ������һ�������߳�                                   */
/************************************************************************/
int KdmSearchStartup(void* pUserHandle, FEvSetDevIp pfunSetIp, s8* pchConfigPath);

/************************************************************************/
/* �������ܣ�ֹͣ����ģ��                                               */
/* ����˵������                                                         */
/* ��������ֵ����                                                       */
/* ����˵����                                                           */
/************************************************************************/
void KdmSearchCleanup(void);

#ifdef __cplusplus
}
#endif

#endif // ifndef _KDMSEARCH_H_

/**************************** FILE END *******************************/

