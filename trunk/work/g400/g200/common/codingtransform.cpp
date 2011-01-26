#include "codingtransform.h"

s32 CodingTransformer::UTF_8ToGB2312(s8* pOut, s32 iBufSize, s8 *pText, s32 nLen)
{
#ifdef WIN32
     WCHAR* pWtemp = new WCHAR[nLen]; 
     int iWcharLenth = ::MultiByteToWideChar(CP_UTF8,0,pText,-1,pWtemp,nLen);
     if (0 == iWcharLenth)
     {
          DWORD dwLastErr = GetLastError();
          printf("alexaroma:×ªÂë´íÎó£¬´íÎóºÅ:%d\n", dwLastErr);
          delete pWtemp;
          return 0;
     }
     int iMultByteLenth = ::WideCharToMultiByte(CP_GB2312, NULL, pWtemp, 
                                             iWcharLenth, pOut, iBufSize, NULL, NULL);
     if (0 == iMultByteLenth)
     {
          DWORD dwLastErr = GetLastError();
          printf("alexaroma:×ªÂë´íÎó£¬´íÎóºÅ:%d\n", dwLastErr);
          delete pWtemp;
          return 0;
     }
     delete pWtemp;
     return iMultByteLenth;
#else
     iconv_t cd;
     int rc;
     char   **pin   =   &pText;
     char   **pout   =   &pOut;
     int outlen = iBufSize;
     cd = iconv_open("GB2312","UTF-8");
     if(cd==0)
     {
        return -1;
     }
     memset(pOut,0,outlen); 
     if(iconv(cd,pin,(size_t*)&nLen,pout,(size_t*)&outlen) != -1)
     {
          iconv_close(cd);
          return outlen; 
     }
     iconv_close(cd);
     return -1;
#endif//WIN32
    return 0;
}


int  CodingTransformer::GB2312ToUTF_8(s8* pOut, s32 iBufSize,s8 *pText, s32 nLen)
{
#ifdef WIN32
     WCHAR* pWtemp = new WCHAR[nLen];
     int iWcharLenth = ::MultiByteToWideChar(CP_GB2312, MB_PRECOMPOSED, 
                                          pText, -1, pWtemp, nLen);
     if (0 == iWcharLenth) 
     {
          DWORD dwLastErr = GetLastError(); 
          printf("alexaroma:×ªÂë´íÎó£¬´íÎóºÅ:%d\n", dwLastErr); 
          delete pWtemp;  return 0;
     }
     int iMultByteLenth = ::WideCharToMultiByte(CP_UTF8, 0, pWtemp, 
                                             iWcharLenth, pOut, iBufSize, NULL, NULL);
     if (0 == iMultByteLenth) 
     { 
          DWORD dwLastErr = GetLastError(); 
          printf("alexaroma:×ªÂë´íÎó£¬´íÎóºÅ:%d\n", dwLastErr); 
          delete pWtemp;
          return 0; 
     } 
     delete pWtemp; 
     return iMultByteLenth;
#else//WIN32
     iconv_t cd;
     int   rc;
     char   **pin   =   &pText; 
     char   **pout   =   &pOut; 
     int outlen = iBufSize;
     cd = iconv_open("UTF-8","GB2312");
     if(cd == 0)
     {
        return -1;
     }  
     memset(pOut,0,outlen); 
     if(iconv(cd,pin,(size_t *)&nLen,pout,(size_t*)&outlen) != -1) 
     {
          iconv_close(cd); 
          return outlen; 
     } 
     iconv_close(cd);
     return -1;
#endif//WIN32
    return 0;
}
