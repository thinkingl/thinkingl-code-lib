/*****************************************************************************
    ģ���� ��PMS
    �ļ��� ��
    ����ļ� ��
    �ļ�ʵ�ֹ��ܣ�
    ����        ������
    �汾        ��V1.0  Copyright(C) 2007-2008 KDC, All rights reserved.
------------------------------------------------------------------------------
    �޸ļ�¼:
    ��  ��		�汾		�޸���		�߶���      �޸�����
    11/30/07    1.0         ����                     ����
******************************************************************************/

#ifndef _CNETMANAGER_H
#define _CNETMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdvtype.h"
#include "kdvencrypt.h"
#include "osp.h"
#include "kdmconst.h"

#ifdef _LINUX_


#include <errno.h>
#include <net/if.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <arpa/inet.h>

#include <sys/socket.h>  // socket
#include <sys/times.h>  // time
#include <sys/select.h> 
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#endif


#define ETHTOOL_GSET   	        0x00000001 /* Get settings. */
#define SIOCETHTOOL             0x8946

/* The forced speed, 10Mb, 100Mb, gigabit, 2.5Gb, 10GbE. */
#define NIC_SPEED_10		    (u16)10

#define NIC_SPEED_100		    (u16)100

#define NIC_SPEED_1000		    (u16)1000

#define NIC_SPEED_2500	    	(u16)2500

#define NIC_SPEED_10000 		(u16)10000

#define CAN_NOT_GET_DEV_SETTING 0

//����socketʧ��
#define CREATE_SOCKET_ERROR     0

//���mac��ַ����
#define MAX_MAC_LEN             (u8)64

#define MAX_IPADDRESS_LEN       (s32)(32+1)

#define  BUFSIZE                (u32)8192 

#ifndef IF_NAMESIZE
#define  IF_NAMESIZE            (u32)16
#endif

//NetManager����
struct NetConfigure
{
public:
	NetConfigure()
	{
		memset(this,0,sizeof(NetConfigure));
	}
    const s8 *GetNicIp()const
    {
        return m_achLocalIp;
    }
    const s8 *GetNicNetMask()const
    {
        return m_achLocalNetMask;
    }
    const s8 *GetNicGateWay()const
    {
        return m_achLocalGateWay;
    }
    void SetNicIp(const s8 *achIp)
    {
        if (NULL == achIp)
        {
            return;
        }
        strncpy(m_achLocalIp, achIp, sizeof(m_achLocalIp));
        m_achLocalIp[sizeof(m_achLocalIp) - 1] = 0;
    }

    void SetNicNetMask(const s8 *achNetMask)
    {
        if (NULL == achNetMask)
        {
            return;
        }
        strncpy(m_achLocalNetMask, achNetMask, sizeof(m_achLocalNetMask));
        m_achLocalNetMask[sizeof(m_achLocalNetMask) - 1] = 0;
    }

    void SetNicGateWay(const s8 *achGateWay)
    {
        if (NULL == achGateWay)
        {
            return;
        }
        strncpy(m_achLocalGateWay, achGateWay, sizeof(m_achLocalGateWay));
        m_achLocalGateWay[sizeof(m_achLocalGateWay) - 1] = 0;
    }

private:
    s8  m_achLocalIp     [MAX_IPADDRESS_LEN];
    s8  m_achLocalNetMask[MAX_IPADDRESS_LEN];
    s8  m_achLocalGateWay[MAX_IPADDRESS_LEN];
};

struct route_info{ 
    in_addr dstAddr; 
    in_addr srcAddr; 
    in_addr gateWay; 
    s8 ifName[IF_NAMESIZE]; 
}; 

//�豸�������
class CNetManager
{
public:
    CNetManager()
    {
        m_wSpeed = 0;
        m_byNicNum = 0;
    }
	
    ~CNetManager()
    {
		
    }
    
public:

    /*========================================================================
    �� �� ����GetNicNum
    ��    �ܣ���ȡ�豸������Ŀ
    ��    ����
    �� �� ֵ�����ص����������������ص�ַ
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2007/12/04  2800    ������  ����
    ========================================================================*/
    u8 GetNicNum()
    {
#ifdef _LINUX_
        
        s32 nSocket = 0;
        ifconf tIfconf;
        memset(&tIfconf, 0, sizeof(ifconf));
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
            return CREATE_SOCKET_ERROR;
        } 
        //  getparms nic num
        if (!ioctl(nSocket, SIOCGIFCONF, (s8 *)&tIfconf))
        {
            //�ų����ص�ַ
            m_byNicNum = tIfconf.ifc_len / sizeof(ifreq) - 1;
            
            if (nSocket != 0)
            {
                close(nSocket);
            }
            return m_byNicNum;
        }
        else
        {
			OspPrintf(TRUE, FALSE,"Cannot get device settings");
            //�ͷ�socket
            if (nSocket != 0)
            {
                close(nSocket);
            }
            return CAN_NOT_GET_DEV_SETTING;
        }
        
        //�ͷ�socket
        if (nSocket != 0)
        {
            close(nSocket);
        }
#endif 
        return CREATE_SOCKET_ERROR;
    };

    /*========================================================================
    �� �� ����GetSpeed
    ��    �ܣ���ȡָ�����ڴ���
    ��    ���������豸��"eth0"/"eth1" ..../"eth16" ��֧�ֵ�16
    �� �� ֵ��NIC_SPEED_10/NIC_SPEED_100/NIC_SPEED_1000/NIC_SPEED_2500
              /NIC_SPEED_10000/NIC_SPEED_ERROR
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2007/12/04  2800    ������  ����
    ========================================================================*/
	u16 GetSpeed(const s8 *achDevName)
    {
#ifdef _LINUX_
        s32  nError;
        ethtool_cmd tEthoolCmd;
        ifreq ifr;
        s32 nSocket = 0;
        
        /* Setup our control structures. */
        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, achDevName);
		OspPrintf(TRUE, FALSE,"Nic DevName -- %s\n", ifr.ifr_name);
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
			OspPrintf(TRUE, FALSE,"Creat Socket Failed!! DevName -- %s\n", achDevName);
            return CREATE_SOCKET_ERROR;
        }
        
        tEthoolCmd.cmd = ETHTOOL_GSET;
        ifr.ifr_data = (s8 *)&tEthoolCmd;
        nError = ioctl(nSocket, SIOCETHTOOL, &ifr);
        
        if (0 == nError) 
        {
			OspPrintf(TRUE, FALSE,"DevName-- %s:  GetSpeed-- %d\n", achDevName, tEthoolCmd.speed);
            m_wSpeed = tEthoolCmd.speed;
            //�ͷ�socket
            if (nSocket != 0)
            {
                close(nSocket);
            }
			if (65535 == m_wSpeed)
			{
				m_wSpeed = 0;
			}
            return m_wSpeed;
            
        }
        else if (nError != EOPNOTSUPP) 
        {
			OspPrintf(TRUE, FALSE,"Cannot get device settings");
            
            //�ͷ�socket
            if (nSocket != 0)
            {
                close(nSocket);
            }
            return CAN_NOT_GET_DEV_SETTING;
        }
        
        //�ͷ�socket
        if (nSocket != 0)
        {
            close(nSocket);
        }
#endif 
        return CREATE_SOCKET_ERROR;
    };

    //�޸�ָ������IP

    //�޸�ָ������NetMask

    //�޸�ָ������GateWay

    /*========================================================================
    �� �� ����GetNicInfo
    ��    �ܣ���ȡ�豸���ڲ���
    ��    ���������豸��"eth0"/"eth1" ..../"eth16" ��֧�ֵ�16
    �� �� ֵ��NetConfigure
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2007/12/04  2800    ������  ����
    ========================================================================*/
	NetConfigure GetNicInfo(const s8 *achDevName)
    {
        memset(&m_tNetConfigure, 0, sizeof(NetConfigure));
#ifdef _LINUX_
        
        ifreq  chIfreq;
        memset(&chIfreq, 0, sizeof(ifreq));
        sockaddr_in *pAddr = NULL;
        sockaddr_in *pMask = NULL;
		sockaddr_in *pGateway = NULL;
        s32 nSocket = 0;
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
            return m_tNetConfigure;
        } 
        strcpy(chIfreq.ifr_name, achDevName);
        
        if (!(ioctl(nSocket, SIOCGIFADDR, (s8 *)&chIfreq)))
        {
            pAddr = (sockaddr_in *) (&chIfreq.ifr_addr);
            m_tNetConfigure.SetNicIp(inet_ntoa(pAddr->sin_addr));
        }
        if (!(ioctl(nSocket, SIOCGIFNETMASK, (s8 *)&chIfreq)))
        {
            pMask = (sockaddr_in *)(&chIfreq.ifr_netmask);
            m_tNetConfigure.SetNicNetMask(inet_ntoa(pMask->sin_addr));
        }    
//         if (!(ioctl(nSocket, SIOCGIFBRDADDR, (s8 *)&chIfreq)))
//         {
//             pBrdAddr = (sockaddr_in *)(&chIfreq.ifr_broadaddr);
//             m_tNetConfigure.SetNicGateWay(inet_ntoa(pBrdAddr->sin_addr));
//         }
        s8  m_achLocalGateWay[MAX_IPADDRESS_LEN];
        if (GetGateway(m_achLocalGateWay, achDevName))
        {
            m_tNetConfigure.SetNicGateWay(m_achLocalGateWay);
        }
        
        if (nSocket != 0)
        {
            close(nSocket);
        }
#endif    
        
        return m_tNetConfigure;
    };

    /*========================================================================
    �� �� ����NicIsConnet
    ��    �ܣ������Ƿ�����
    ��    ���������豸��"eth0"/"eth1" ..../"eth16" ��֧�ֵ�16
    �� �� ֵ��TRUEΪ����״̬ FALSEΪ�Ͽ�״̬
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2007/12/04  2800    ������  ����
    ========================================================================*/
	BOOL32 NicIsConnet(const s8 *achDevName)
    {
#ifdef _LINUX_
        ifreq ifr;
        ethtool_value tIsConnect;
        
        memset(&ifr, 0, sizeof(ifr));
        tIsConnect.cmd = ETHTOOL_GLINK;
        
        s32 nSocket = 0;
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
			OspPrintf(TRUE, FALSE,"Create Socket Failed\n");
            return FALSE;
        } 
        strncpy(ifr.ifr_name, achDevName, sizeof(ifr.ifr_name)-1);
        ifr.ifr_data = (char *) &tIsConnect;
        
        if (ioctl(nSocket, SIOCETHTOOL, &ifr) == -1)
        {

            //OspPrintf(TRUE, FALSE,"ETHTOOL_GLINK failed: %s\n", strerror(errno));
            close(nSocket);

            return FALSE;
        }
        
        close(nSocket);
        return (tIsConnect.data ? FALSE : TRUE);
#endif
        return FALSE;
    };

    /*========================================================================
    �� �� ����GetMacAddr
    ��    �ܣ���ȡMAC��ַ
    ��    ����
    �� �� ֵ��TRUEΪ��ȡ�ɹ� FALSEΪ��ȡʧ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2007/12/04  2800    ������  ����
    ========================================================================*/
	BOOL32 GetMacAddr(const s8 *achDevName, s8 *pchTextContent, s32 nBufLen)
    {
        u8 achTextInContent[MAX_MAC_LEN];
        u8 achTextOutContent[MAX_MAC_LEN];
        memset(achTextInContent, 0, sizeof(achTextInContent));
        memset(achTextOutContent, 0, sizeof(achTextInContent));
		memset(pchTextContent, 0, nBufLen);
        
#ifdef _LINUX_
        ifreq ifr;
        u8 wII = 0;
        s32 nSocket = 0;
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
            return FALSE;
        } 
        
        strncpy(ifr.ifr_name, achDevName, sizeof(ifr.ifr_name)-1);
        
        if (!(ioctl (nSocket, SIOCGIFHWADDR, (s8 *)&ifr)))
        {
            //������MAC��ַȡ�������achTextInContent��
            while ( wII < 6 )
            {
                achTextInContent[wII] = ifr.ifr_hwaddr.sa_data[wII];
                wII++;
            }
        }
        else
        {
            close(nSocket);
            return FALSE;
        }
        
        //���ݼ���
        int nAesRet;
        nAesRet = KdvAES(AUTH_DEFAULT_AES_KEY , strlen(AUTH_DEFAULT_AES_KEY),
            AUTH_DEFAULT_AES_MODE , DIR_ENCRYPT, AUTH_DEFAULT_AES_INITKEY,
            (u8 *)achTextInContent, MAX_MAC_LEN, (u8 *)achTextOutContent);
        
        if (nAesRet < 0)
        {
            close(nSocket);
            return FALSE;
        }
        
		s32 nMaxLenCpy=nBufLen/2 <64 ? nBufLen/2 : 64;
        for (wII=0; wII<nMaxLenCpy; wII++)
        {      
            sprintf(pchTextContent + wII*2, "%02x", achTextOutContent[wII]);
        }
        pchTextContent[nMaxLenCpy - 1]= 0;

        close(nSocket);
#endif
        
        return TRUE;
    };
    /*========================================================================
    �� �� ����GetMacAddr
    ��    �ܣ���ȡMAC��ַ
    ��    ����
    �� �� ֵ��TRUEΪ��ȡ�ɹ� FALSEΪ��ȡʧ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2007/12/04  2800    ������  ����
    ========================================================================*/
	BOOL32 GetRawMacAddr(const s8 *achDevName, s8 *pchTextContent)
	{
#ifdef _LINUX_
        ifreq ifr;
        u8 wII = 0;
        s32 nSocket = 0;
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
            return FALSE;
        } 
        
        strncpy(ifr.ifr_name, achDevName, sizeof(ifr.ifr_name)-1);
        
        if (!(ioctl (nSocket, SIOCGIFHWADDR, (s8 *)&ifr)))
        {
			sprintf(pchTextContent,"%02x:%02x:%02x:%02x:%02x:%02x",
				(u8)ifr.ifr_hwaddr.sa_data[0],(u8)ifr.ifr_hwaddr.sa_data[1],(u8)ifr.ifr_hwaddr.sa_data[2],
				(u8)ifr.ifr_hwaddr.sa_data[3],(u8)ifr.ifr_hwaddr.sa_data[4],(u8)ifr.ifr_hwaddr.sa_data[5]);
        }
        else
        {
            close(nSocket);
            return FALSE;
        }
#endif
        
        return TRUE;
	}
    /*========================================================================
    �� �� ����SetNicInfo
    ��    �ܣ�������������
    ��    ������Ҫ���õ��������
    �� �� ֵ��TRUEΪ���óɹ� FALSEΪ����ʧ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2007/12/04  2800    ������  ����
    ========================================================================*/
    BOOL32 SetNicInfo(const s8 *achDevname, const s8 *achIp, const s8 *achMask, const s8 *achGateWay)
    {
        s8 achCfgFilePath[50] = {0};
        s8 achCfgFilePathNew[50] = {0};
        FILE* pCfgFile;
        FILE* pCfgFileNew;
        s8 achLine[512] = {0};
        s8 achTemp[512] = {0};
        BOOL bFoundIp = FALSE;
        BOOL bFoundMask = FALSE;
        BOOL bFoundGateWay = FALSE;
        
        sprintf(achCfgFilePath, "/etc/sysconfig/network-scripts/ifcfg-%s", achDevname);
        sprintf(achCfgFilePathNew, "%s.new", achCfgFilePath);

        pCfgFile = fopen(achCfgFilePath, "r" );
        if (NULL == pCfgFile)
        {
            return FALSE;
        }
        
        pCfgFileNew = fopen(achCfgFilePathNew, "w");
        if (NULL == pCfgFileNew)
        {
            fclose(pCfgFile);
            return FALSE;
        }
        
        while (0 == feof(pCfgFile))
        {
            if (NULL == fgets(achLine, 512, pCfgFile))
            {
                if (ferror(pCfgFile) != 0)
                {// error
                    fclose(pCfgFile);
                    fclose(pCfgFileNew);
                    return FALSE;
                }
                else
                {
                    continue;
                }
            }
            
            //������IPADDR
            if ('#'  != achLine[0] && NULL != strstr(achLine, "IPADDR") && NULL != achIp)  
            {
                sprintf(achTemp, "IPADDR=%s\n", achIp);
                if (-1 == fputs(achTemp, pCfgFileNew))
                {
                    fclose(pCfgFile);
                    fclose(pCfgFileNew);
                    return FALSE;
                }
                bFoundIp = TRUE;
                continue;
            }
            //������NETMASK
            if ('#'  != achLine[0] && NULL != strstr(achLine, "NETMASK") && NULL != achMask)  
            {
                sprintf(achTemp, "NETMASK=%s\n", achMask);
                if (-1 == fputs(achTemp, pCfgFileNew))
                {
                    fclose(pCfgFile);
                    fclose(pCfgFileNew);
                    return FALSE;
                }
                bFoundMask = TRUE;
                continue;
            }
            //������GATEWAY
            if ('#'  != achLine[0] && NULL != strstr(achLine, "GATEWAY") && NULL != achGateWay)  
            {
                sprintf(achTemp, "GATEWAY=%s\n", achGateWay);
                if (-1 == fputs(achTemp, pCfgFileNew))
                {
                    fclose(pCfgFile);
                    fclose(pCfgFileNew);
                    return FALSE;
                }
                bFoundGateWay = TRUE;
                continue;
            }

            //������ԭ��д��
            if(-1 == fputs(achLine, pCfgFileNew))
            {
                fclose(pCfgFile);
                fclose(pCfgFileNew);
                return FALSE;
            }
            continue;
        }

        //������IPADDR �����һ��
        if(!bFoundIp && NULL != achMask)
        {
            sprintf(achTemp, "IPADDR=%s\n", achIp);
            if (-1 == fputs(achTemp, pCfgFileNew))
            {
                fclose(pCfgFile);
                fclose(pCfgFileNew);
                return FALSE;
            }	
        }
        //������NETMASK �����һ��
        if(!bFoundMask && NULL != achIp)
        {
            sprintf(achTemp, "NETMASK=%s\n", achMask);
            if (-1 == fputs(achTemp, pCfgFileNew))
            {
                fclose(pCfgFile);
                fclose(pCfgFileNew);
                return FALSE;
            }
        }
        //������GATEWAY �����һ��
        if(!bFoundGateWay && NULL != achGateWay && 0 != achGateWay[0])
        {
            sprintf(achTemp, "GATEWAY=%s\n", achGateWay);
            if (-1 == fputs(achTemp, pCfgFileNew))
            {
                fclose(pCfgFile);
                fclose(pCfgFileNew);
                return FALSE;
            }	
        }
        
        fclose(pCfgFile);
        fclose(pCfgFileNew);
        
        //�滻�ļ�
        char szSysCmd[512] = {0};
        sprintf(szSysCmd, "rm -f %s; mv -f %s %s", achCfgFilePath, achCfgFilePathNew, achCfgFilePath);
        OspSystem( szSysCmd);
        
        return TRUE;
    };
    

    s32 ReadNlSock(s32 sockFd, s8 *bufPtr, s32 seqNum, s32 pId)
    {
        s32 msgLen = 0; 
 #ifdef _LINUX_
        struct nlmsghdr *nlHdr; 
        s32 readLen = 0;
        
        do
        { 
            if((readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0)
            { 
                perror("SOCK READ: "); 
                return -1; 
            } 
            
            nlHdr = (struct nlmsghdr *)bufPtr; 
            
            if((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR)) 
            { 
                perror("Error in recieved packet"); 
                return -1; 
            } 
            
            if(nlHdr->nlmsg_type == NLMSG_DONE) 
            { 
                break; 
            } 
            else
            { 
                bufPtr += readLen; 
                msgLen += readLen; 
            } 
            
            if((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) 
            { 
                break; 
            } 
        } while((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId)); 
#endif
        return msgLen; 
    } 
#ifdef _LINUX_    
    /* For parsing the route info returned */ 
    BOOL32 ParseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo,s8 *gateway, s8 *devname) 
    { 
        struct rtmsg *rtMsg; 
        struct rtattr *rtAttr; 
        s32 rtLen; 
        s8 *tempBuf = NULL; 
        
        tempBuf = (s8 *)malloc(100); 
        rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr); 
        
		/* If the route is not for AF_INET or does not belong to main routing table 
		then return. */ 
        if((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN)) 
            return FALSE; 
        
		/* get the rtattr field */ 
        rtAttr = (struct rtattr *)RTM_RTA(rtMsg); 
        rtLen = RTM_PAYLOAD(nlHdr); 
        for(;RTA_OK(rtAttr,rtLen);rtAttr = RTA_NEXT(rtAttr,rtLen))
        { 
            switch(rtAttr->rta_type) 
            { 
            case RTA_OIF:
                if_indextoname(*(s8 *)RTA_DATA(rtAttr), rtInfo->ifName);
				OspPrintf(TRUE, FALSE, "-------rtInfo->ifName---%s\n", rtInfo->ifName);
                break; 
                
            case RTA_GATEWAY: 
                rtInfo->gateWay = *(in_addr *)RTA_DATA(rtAttr); 
                break; 
            } 
        } 
		// ADDED BY BOB - ALSO COMMENTED prs32Route 
        
        if (NULL != strstr((s8 *)inet_ntoa(rtInfo->dstAddr), "0.0.0.0")) 
        {
            sprintf(gateway, (s8 *)inet_ntoa(rtInfo->gateWay));
            sprintf(devname, rtInfo->ifName);
			OspPrintf(TRUE, FALSE, "-----gateway--%s\n ---- devname--%s\n", gateway, devname);
            
            free(tempBuf); 
            return TRUE;
        }
        free(tempBuf); 
        return FALSE; 
    } 
#endif
	    /*========================================================================
    �� �� ����GetGateway
    ��    �ܣ���ȡ��������
    ��    ����
    �� �� ֵ��TRUEΪָ��������ȡ���سɹ� FALSEΪʧ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2007/12/04  2800    ������  ����
    ========================================================================*/
    BOOL32 GetGateway(s8 *gateway, const s8* devname) 
    {
#ifdef _LINUX_ 
        struct nlmsghdr *nlMsg; 
        struct rtmsg *rtMsg; 
        struct route_info *rtInfo; 
        s8 msgBuf[BUFSIZE]; 
        
        s32 sock, len, msgSeq = 0; 
        s8 buff[1024]; 
        
        
        /* Create Socket */ 
        if((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0) 
            perror("Socket Creation: "); 
        
		/* Initialize the buffer */ 
        memset(msgBuf, 0, BUFSIZE); 
        
		/* pos32 the header and the msg structure pos32ers s32o the buffer */ 
        nlMsg = (struct nlmsghdr *)msgBuf; 
        rtMsg = (struct rtmsg *)NLMSG_DATA(nlMsg); 
        
        /* Fill in the nlmsg header*/ 
		nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message. 
		nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table . 
		
		nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump. 
		nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet. 
		nlMsg->nlmsg_pid = getpid(); // PID of process sending the request. 
        
        /* Send the request */ 
        if(send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0)
        { 
            close(sock);
            return FALSE; 
        } 
        
		/* Read the response */ 
        if((len = ReadNlSock(sock, msgBuf, msgSeq, getpid())) < 0) 
        { 
            close(sock);
            return FALSE; 
        } 
		/* Parse and prs32 the response */ 
        rtInfo = (struct route_info *)malloc(sizeof(struct route_info)); 
        
		/* THIS IS THE NETTSTAT -RL code I commented out the prs32ing here and in parse routes */ 
		//fprs32f(stdout, "Destination\tGateway\ts32erface\tSource\n"); 
        s8 achdevname[IF_NAMESIZE] = {0};
        for(;NLMSG_OK(nlMsg,len);nlMsg = NLMSG_NEXT(nlMsg,len))
        { 
            memset(rtInfo, 0, sizeof(struct route_info));
            
            ParseRoutes(nlMsg, rtInfo, gateway, achdevname);
            
        }  
        if (0 == strcmp(achdevname, devname))
        {
            free(rtInfo); 
            close(sock);
            return TRUE;
        }
        
        free(rtInfo); 
        close(sock);
#endif
        return FALSE; 
    } 

private:
    //���ڸ���
    u8 m_byNicNum;

    //�����豸��

    //������������
	NetConfigure m_tNetConfigure;

    //���ڴ���
    u16 m_wSpeed;

};

#endif
