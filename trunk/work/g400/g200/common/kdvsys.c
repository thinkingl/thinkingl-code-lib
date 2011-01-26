/*****************************************************************************
   ģ����      : KDV system
   �ļ���      : kdvsys.c
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDV�����������궨��
   ����        : ����
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2002/01/30  0.9         ����        ����
   2003/08/25  1.0		   jianghy     ȥ����ӡ��Ϣ
   2005/05/22  3.6         libo        linux��ʵ��
******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "kdvsys.h"

#ifdef _LINUX_
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

/*====================================================================
    ������      ��OpenProfile
    ����        ���������ļ�
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����tKeyHandle *lpKeyHandle, Profile����ָ��   
                  const s8* lpszProfileName, Profile�ļ���
                  BYTE  byOpenMode, �򿪷�ʽ
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
/*DllExport BOOL32 OpenProfile( tKeyHandle *lpKeyHandle,
                  const s8* lpszProfileName,
                  BYTE  byOpenMode
                )
{
    char    achMode[5];

    switch( byOpenMode )
    {
    case PROFILE_READONLY:
        strcpy( achMode, "rt" );
        break;
    case PROFILE_READWRITE:
    case PROFILE_WRITEONLY:
        strcpy( achMode, "rt+" );
        break;
    default:
        return( FALSE );
    }

    *lpKeyHandle = ( tKeyHandle )fopen( lpszProfileName, achMode );
    if( ( FILE * )(*lpKeyHandle) == NULL )
    {
#ifdef _DEBUG
        printf( "Open file %s failed!\n", lpszProfileName );
#endif
        return( FALSE );
    }
    else
    {
#ifdef _DEBUG
        printf( "Open file %s succeeded! KeyHandle = %d\n", 
            lpszProfileName, *lpKeyHandle );
#endif
        return( TRUE );
    }
}*/
   
/*====================================================================
    ������      ��CloseProfile
    ����        ���ر������ļ�
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, Profile����   
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
/*DllExport BOOL32 CloseProfile ( tKeyHandle lpKeyHandle )
{
    int iReturn = fclose( ( FILE * )lpKeyHandle );

    if( iReturn == 0 )
    {
#ifdef _DEBUG
        printf( "Close file %d succeeded!\n", lpKeyHandle );
#endif
        return( TRUE );
    }
    else
    {
#ifdef _DEBUG
        printf( "Close file %d failed!\n", lpKeyHandle );
#endif
        return( FALSE );
    }
} */   

/*====================================================================
    ������      ��WriteKeyValueString
    ����        ���ӵ�ǰ�α괦��ʼ����ָ���ؼ���������û����ö�Ӧ����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, Profile����   
                  s32 sdwOffsetCurLine, Ҫд����п�ʼ��ƫ����
                  s32 sdwOffsetNextLine, ��һ�п�ʼ��ƫ����
                  const s8* lpszKeyName, Ҫд��Ĺؼ�����
                  const s8* lpszKeyValue, Ҫд��Ĺؼ���ֵ�����ΪNULL��
                    ɾ������
    ����ֵ˵��  ����ǰ�½�β��ƫ������ʧ�ܷ���-1
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/31    1.0         Li, Yi        ����
====================================================================*/
s32 WriteKeyValueString( FILE *stream, 
                s32 sdwOffsetCurLine, s32 sdwOffsetNextLine, 
                const s8* lpszKeyName, const s8* lpszKeyValue )
{
    u32  dwLen;
    s8*   lpszBuf = NULL ;

    ASSERT( lpszKeyName != NULL );

    /* copy the remaining content in buffer */
    fseek( stream, 0, SEEK_END );
    dwLen = ftell( stream ) - sdwOffsetNextLine;
    if( dwLen != 0 )
    {
        lpszBuf = (s8*)malloc( dwLen );
        ASSERT( lpszBuf != NULL );
        fseek( stream, sdwOffsetNextLine, SEEK_SET );
        dwLen = fread( lpszBuf, sizeof( char ), dwLen, stream );
    }
    
    /* write the new value or delete it */
    fseek( stream, sdwOffsetCurLine, SEEK_SET );
    if( lpszKeyValue != NULL )
    {
        fputs( (const s8*)lpszKeyName, stream );
        fputs( (const s8*)" = ", stream );
        fputs( (const s8*)lpszKeyValue, stream );
        fputs( (const s8*)STR_RETURN, stream );
    }

    /* write the remaining content in the buffer back */
    if( dwLen != 0 )
    {
        fwrite( lpszBuf, sizeof( char ), dwLen, stream );
    }

    if( lpszBuf )
    {
        free( lpszBuf );
    }
    return( ftell( stream ) );
}

/*====================================================================
    ������      ��GetRegKeyInt
    ����        �����α��Ƶ�ָ���Ķ�����һ�п�ʼ��
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, Profile����   
                  const s8* lpszSectionName, Profile�еĶ���
                  BOOL32 bCreate, ���û�иö����Ƿ񴴽�
    ����ֵ˵��  ������ƫ����Offset�����ʧ�ܷ���-1
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
s32 MoveToSection( FILE *stream, const s8* lpszSectionName, 
                   BOOL32 bCreate )
{
    char    achRow[MAX_LINE_LEN + 1];
    char    achSectionCopy[MAX_SECTIONNAME_LEN + 1];
    u32  dwLen;
    s8*   lpszReturn;

    ASSERT( strlen( lpszSectionName ) <= MAX_SECTIONNAME_LEN );

    /* move from the beginning */
    if ( fseek( stream, 0, SEEK_SET ) != 0 )
    {
        return( -1 );
    }

    /* copy the section name and change it */
    strncpy( achSectionCopy, lpszSectionName, MAX_SECTIONNAME_LEN );
    achSectionCopy[MAX_SECTIONNAME_LEN] = '\0';
    Trim( achSectionCopy );
    StrUpper( achSectionCopy );

    do
    {
        /* if error or arrive at the end of file */
        if( fgets( achRow, MAX_LINE_LEN, stream ) == NULL )
        {
            if( bCreate && feof( stream ) )    /* create the section */
            {
                fputs( (const s8*)STR_RETURN, stream );
                fputs( (const s8*)"[", stream );
                fputs( (const s8*)lpszSectionName, stream );
                fputs( (const s8*)"]", stream );
                fputs( (const s8*)STR_RETURN, stream );
                fseek( stream, 0, SEEK_END );
                return( ftell( stream ) );
            }
            else
            {
                return( -1 );
            }
        }

        /* eliminate the return key */
        if( ( lpszReturn = strstr( achRow, "\r\n" ) ) != NULL )
        {
            lpszReturn[0] = '\0';
        }
        else if( ( lpszReturn = strstr( achRow, "\n" ) ) != NULL )
        {
            lpszReturn[0] = '\0';
        }
        Trim( achRow );
        dwLen = strlen( achRow );
        
        /* must be a section */
        if( dwLen <= 2 || achRow[0] != '[' || achRow[dwLen - 1] != ']' )
        {
            continue;
        }
        
        /* verify the section name */
        strncpy( achRow, achRow + 1, dwLen - 2 );
        achRow[dwLen - 2] = '\0';
        Trim( achRow );
        StrUpper( achRow );
        if( strcmp( achRow, achSectionCopy ) == 0 )    /* identical */
        {
            return( ftell( stream ) );
        }
    } while( TRUE );
}

/*====================================================================
    ������      ��ReadWriteKeyValueString
    ����        ���ӵ�ǰ�α괦��ʼ����ָ���ؼ���������û����ö�Ӧ����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, Profile����   
                  const s8* lpszKeyName, ���ڵĹؼ�����
                  s8* lpszKeyValue, ���ػ����õĹؼ��ֶ�Ӧֵ�ַ���ָ�룬
                    ��С��Ԥ�ȷ��䣬���Ϊд��ʽ�Ҹ�ָ��ΪNULL����ɾ����
                    ��
                  s32 sdwOffsetCurSection, ��ǰҪ��д��Section����ʼ
                    ƫ�������Ӹ�Section��ʼ
                  u32 dwBufSize, ���ػ����õĹؼ��ֶ�Ӧֵ�ַ���Buffer��
                    ��С���粻�����Զ��ضϣ����һ���ֽ�����'\0'
                  BOOL32 bWrite, �Ƿ�д�����д�Ļ����Զ�����
    ����ֵ˵��  ����ǰ�½�β��ƫ������ʧ�ܷ���-1��READ������ֵ������
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
s32 ReadWriteKeyValueString( FILE *stream, const s8* lpszKeyName, 
                        s8* lpszKeyValue, s32 sdwOffsetCurSection, 
                        u32 dwBufSize, BOOL32 bWrite )
{
    char    achRow[MAX_LINE_LEN + 1];
    char    achRowKeyName[MAX_KEYNAME_LEN + 1];
    char    achKeyNameCopy[MAX_KEYNAME_LEN + 1];
    s8*   lpszEqualPos;
    s8*   lpszReturn;
    u32  dwLen;
    s32  sdwOffsetCurLine;
    s32  sdwOffsetNextLine;

    ASSERT( lpszKeyName != NULL && strlen( lpszKeyName ) <= MAX_KEYNAME_LEN );
    //ASSERT( !bWrite || ( strlen( lpszKeyValue ) <= MAX_VALUE_LEN ) );

    /* copy the Key name and change it */
    strncpy( achKeyNameCopy, lpszKeyName, MAX_KEYNAME_LEN );
    achKeyNameCopy[MAX_KEYNAME_LEN] = '\0';
    Trim( achKeyNameCopy );
    StrUpper( achKeyNameCopy );

    fseek( stream, sdwOffsetCurSection, SEEK_SET );
    do
    {
        /* record the offset in every line */
        sdwOffsetCurLine = ftell( stream );

        if( fgets( achRow, MAX_LINE_LEN, stream ) == NULL )
        {                                           /* the end */
            if( bWrite && lpszKeyValue != NULL && feof( stream ) )       
                                                    /* create the Key */
            {
                sdwOffsetNextLine = sdwOffsetCurLine;
                return( WriteKeyValueString( stream, sdwOffsetCurLine, 
                    sdwOffsetNextLine, lpszKeyName, lpszKeyValue ) );
            }
            else
            {
                return( -1 );
            }
        }

        /* eliminate the return key */
        if( ( lpszReturn = strstr( achRow, "\r\n" ) ) != NULL )
        {
            lpszReturn[0] = '\0';
        }
        else if( ( lpszReturn = strstr( achRow, "\n" ) ) != NULL )
        {
            lpszReturn[0] = '\0';
        }
        Trim( achRow );
        dwLen = strlen( achRow );
        
        if( dwLen == 0 )    continue;

        /* if arrive at another section */
        if( achRow[0] == '[' )
        {
            if( bWrite && lpszKeyValue != NULL )
            {
                sdwOffsetNextLine = sdwOffsetCurLine = sdwOffsetCurSection;
                return( WriteKeyValueString( stream, sdwOffsetCurLine, 
                    sdwOffsetNextLine, lpszKeyName, lpszKeyValue ) );
            }
            else
            {
                return( -1 );
            }
        }

        /* if arrive at a comment or cannot find a '=', continue */
        if( strstr( achRow, PROFILE_COMMENT1 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT2 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT3 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT4 ) == achRow ||
            ( lpszEqualPos = strchr( achRow, '=' ) ) == NULL )
        {
            continue;
        }

        /* verify the key name */
        memcpy( achRowKeyName, achRow, lpszEqualPos - achRow );
        achRowKeyName[lpszEqualPos - achRow] = '\0';
        Trim( achRowKeyName );
        StrUpper( achRowKeyName );
        if( strcmp( achRowKeyName, achKeyNameCopy ) != 0 )  /* not identical */
        {
            continue;
        }

        /* return or set the key value string */
        if( bWrite )
        {
            sdwOffsetNextLine = ftell( stream );
            return( WriteKeyValueString( stream, sdwOffsetCurLine, 
                sdwOffsetNextLine, lpszKeyName, lpszKeyValue ) );
        }
        else
        {
            Trim( lpszEqualPos + 1 );
            strncpy( lpszKeyValue, lpszEqualPos + 1, dwBufSize - 1 );
            lpszKeyValue[dwBufSize - 1] = '\0';
            return( ftell( stream ) );
        }

    } while( TRUE );
}

/*====================================================================
    ������      ��GetRegKeyInt
    ����        ���������ļ��ж�ȡһ������ֵ
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����const s8* lpszProfileName, �ļ�����������·����
                  const s8* lpszSectionName, Profile�еĶ���   
                  const s8* lpszKeyName, ���ڵĹؼ�����    
                  const s32  sdwDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  s32  *sdwReturnValue, ����ֵ                         
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 GetRegKeyInt( const s8* lpszProfileName, 
                   const s8* lpszSectionName, 
                   const s8* lpszKeyName, 
                   const s32  sdwDefault,
                   s32  *sdwReturnValue 
                 )
{
    char    achKeyValue[MAX_VALUE_LEN + 1];
    BOOL32    bResult;

    *sdwReturnValue = sdwDefault;

    bResult = GetRegKeyString( lpszProfileName, lpszSectionName, lpszKeyName, 
        NULL, achKeyValue, sizeof( achKeyValue ) );

    if( bResult != FALSE )
    {
        *sdwReturnValue = atoi( achKeyValue );
    }
    
    return( bResult );
}


/*====================================================================
    ������      ��GetRegKeyInt2
    ����        ���������ļ��ж�ȡһ������ֵ
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, �ļ����
                  const s8* lpszSectionName, Profile�еĶ���   
                  const s8* lpszKeyName, ���ڵĹؼ�����    
                  const s32  sdwDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  s32  *sdwReturnValue, ����ֵ                         
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 GetRegKeyInt2( FILE *stream, 
                   const s8* lpszSectionName, 
                   const s8* lpszKeyName, 
                   const s32  sdwDefault,
                   s32  *sdwReturnValue 
                 )
{
    char    achKeyValue[MAX_VALUE_LEN + 1];
    BOOL32    bResult;

    *sdwReturnValue = sdwDefault;

    if( stream == NULL )
    {
        return FALSE;
    }
    
    bResult = GetRegKeyString2(stream, lpszSectionName, lpszKeyName, 
        NULL, achKeyValue, sizeof( achKeyValue ) );

    if( bResult != FALSE )
    {
        *sdwReturnValue = atoi( achKeyValue );
    }
    
    return( bResult );
}
    
/*====================================================================
    ������      ��GetRegKeyStringTable
    ����        ���������ļ��ж�ȡһ�ű��������������
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����const s8* lpszProfileName, �ļ�����������·����
                  const s8* lpszSectionName, Profile�еĶ���
                  const s8* lpszDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  s8*  lpszReturnValue, �����ִ�
                  s8* *lpszEntryArray, �����ַ�������ָ��
                  s32 *sdwEntryNum, �ַ���������Ŀ���ɹ��󷵻��ַ�����
                        ��ʵ����Ŀ���紫�����Ŀ���������ļ�¼���贫��
                  u32 dwBufSize, �����ִ�BUFFER�ĳ��ȣ��粻����ضϣ���
                        ��һ���ֽ�����'\0'
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/02/06    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 GetRegKeyStringTable( const s8* lpszProfileName, 
                   const s8* lpszSectionName, 
                   const s8* lpszDefault, 
                   s8* *lpszEntryArray, 
                   u32*dwEntryNum,
                   u32 dwBufSize 
                 )
{
    char    achKeyName[MAX_KEYNAME_LEN + 1];
    BOOL32    bResult = TRUE;
    u32  dwFileEntryNum;     /* number of entries from profile */
    u32  dwReadEntryNum;     /* real number of entries read from profile */
    u32  dwLoop;
    FILE    *stream;

    /* copy the default string */    
    if( lpszDefault != NULL )
    {
        for( dwLoop = 0; dwLoop < *dwEntryNum; dwLoop++ )
        {
            strncpy( lpszEntryArray[dwLoop], lpszDefault, dwBufSize - 1 );
            lpszEntryArray[dwLoop][dwBufSize - 1] = '\0';
        }
    }

    bResult = GetRegKeyInt( lpszProfileName, lpszSectionName, (const s8*)STR_ENTRY_NUM, 
        1, (s32 *)&dwFileEntryNum );        /* get the number of table entries from profile */
    if( bResult == FALSE )  return( FALSE );

    stream = fopen( lpszProfileName, "rb" );
    if( stream == NULL )
    {
 /*     printf( "Open file %s failed!\n", lpszProfileName );*/
        return( FALSE );
    }

    dwReadEntryNum = 0;
    for( dwLoop = 0; dwLoop < min( dwFileEntryNum, *dwEntryNum ); dwLoop++ )
    {
        sprintf( achKeyName, "%s%lu", STR_ENTRY, dwLoop );
        bResult = GetRegKeyString2( stream, lpszSectionName, achKeyName, 
                    NULL, lpszEntryArray[dwReadEntryNum], dwBufSize );
        if( bResult == FALSE )  continue;
        dwReadEntryNum++;
    }

    *dwEntryNum = dwReadEntryNum;

    fclose( stream );
    return( TRUE );
}
    
/*====================================================================
    ������      ��GetRegKeyString
    ����        ���������ļ��ж�ȡһ�ַ���ֵ
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����const s8* lpszProfileName, �ļ�����������·����  
                  const s8* lpszSectionName, Profile�еĶ���
                  const s8* lpszKeyName, ���ڵĹؼ�����
                  const s8* lpszDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  s8*  lpszReturnValue, �����ִ�
                  u32 dwBufSize, �����ִ�BUFFER�ĳ��ȣ��粻����ضϣ���
                        ��һ���ֽ�����'\0'
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 GetRegKeyString( const s8* lpszProfileName,
                      const s8* lpszSectionName,
                      const s8* lpszKeyName, 
                      const s8* lpszDefault, 
                      s8*  lpszReturnValue, 
                      u32 dwBufSize 
                    )
{
    FILE    *stream;
    BOOL32    bResult;

    stream = fopen( lpszProfileName, "rb" );
    if( stream == NULL )
    {
        /*printf( "Open file %s failed!\n", lpszProfileName );*/
		if (lpszDefault != NULL)
		{
			strncpy(lpszReturnValue,lpszDefault,dwBufSize -1);
			lpszReturnValue[dwBufSize - 1] = '\0';
		}
        return( FALSE );
    }

    bResult = GetRegKeyString2( stream, lpszSectionName, lpszKeyName, lpszDefault, 
                    lpszReturnValue, dwBufSize );
    
    fclose( stream );

    return( bResult );
}
    
/*====================================================================
    ������      ��GetRegKeyString2
    ����        �������ļ�����������ļ��ж�ȡһ�ַ���ֵ�����ļ�����
                    "rb"��ʽ��
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, �ļ����
                  const s8* lpszSectionName, Profile�еĶ���
                  const s8* lpszKeyName, ���ڵĹؼ�����
                  const s8* lpszDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  s8*  lpszReturnValue, �����ִ�
                  u32 dwBufSize, �����ִ�BUFFER�ĳ��ȣ��粻����ضϣ���
                        ��һ���ֽ�����'\0'
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 GetRegKeyString2( FILE *stream,
                      const s8* lpszSectionName,
                      const s8* lpszKeyName, 
                      const s8* lpszDefault, 
                      s8*  lpszReturnValue, 
                      u32 dwBufSize 
                    )
{
    s32  sdwOffset;

    ASSERT( lpszSectionName != NULL && lpszKeyName != NULL 
        && lpszReturnValue != NULL );

    ASSERT( stream != NULL);
    
    /* copy the default string */    
    if( lpszDefault != NULL )
    {
        strncpy( lpszReturnValue, lpszDefault, dwBufSize - 1 );
        lpszReturnValue[dwBufSize - 1] = '\0';
    }

    /* find the section first */
    if( ( sdwOffset = MoveToSection( stream, lpszSectionName, FALSE ) ) == -1 )
    {
/*#ifdef _DEBUG
        printf( "Searching section [%s] in file %lu failed!\n", 
            lpszSectionName, ( u32)stream );
#endif*/
        return( FALSE );
    }

    /* continue to find the right key */
    if( ReadWriteKeyValueString( stream, lpszKeyName, lpszReturnValue, 
                sdwOffset, dwBufSize, FALSE ) == -1 )
    {
/*#ifdef _DEBUG
        printf( "Searching for Key %s in file %lu failed!\n", 
            lpszKeyName, ( u32)stream );
#endif */
        return( FALSE );
    }
    
    return( TRUE );
}
    
/*====================================================================
    ������      ��SetRegKeyInt
    ����        ���������ļ���д��һ�µ������͵�ֵ��������������Զ�����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����const s8* lpszProfileName, �ļ�����������·����
                  const s8* lpszSectionName, Profile�еĶ���
                  const s8* lpszKeyName, ���ڵĹؼ�����
                  const s32 sdwValue, �µ�����ֵ
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 SetRegKeyInt( const s8* lpszProfileName,
                   const s8* lpszSectionName, 
                   const s8* lpszKeyName, 
                   const s32 sdwValue   
                 )
{
    char    achKeyValue[MAX_VALUE_LEN + 1];
    BOOL32    bResult;

    sprintf( (s8*)achKeyValue, "%ld", sdwValue );

    bResult = SetRegKeyString( lpszProfileName, lpszSectionName, 
                lpszKeyName, achKeyValue );
    
    return( bResult );
}    

/*====================================================================
    ������      ��SetRegKeyString
    ����        ���������ļ���д��һ�µ��ַ����͵�ֵ��������������Զ�����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����const s8* lpszProfileName, �ļ�����������·����   
                  const s8* lpszSectionName, Profile�еĶ���
                  const s8* lpszKeyName, ���ڵĹؼ�����
                  const s8* lpszValue, �µ��ִ�ֵ, NULL���ʾɾ������
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 SetRegKeyString( const s8* lpszProfileName, 
                      const s8* lpszSectionName, 
                      const s8* lpszKeyName, 
                      const s8* lpszValue
                    )
{
    s32  sdwOffset;
    FILE    *stream;
    s8*    lpszBuf = NULL;

    ASSERT( lpszSectionName != NULL && lpszKeyName != NULL );

    stream = fopen( lpszProfileName, "rb+" );
    if( stream == NULL )
    {
        /*printf( "Open file %s failed!\n", lpszProfileName );*/
        return( FALSE );
    }

    /* find the section first, if not exist, create it */
    if( ( sdwOffset = MoveToSection( stream, lpszSectionName, TRUE ) ) == -1 )
    {
/*#ifdef _DEBUG
        printf( "Creating section [%s] in file %s failed!\n", 
            lpszSectionName, lpszProfileName );
#endif*/
        fclose( stream );
        return( FALSE );
    }

    /* continue to find the right key and write the value */
    if( ( sdwOffset = ReadWriteKeyValueString( stream, lpszKeyName, (s8*)lpszValue, 
            sdwOffset, 0, TRUE ) ) == -1 )
    {
/*#ifdef _DEBUG
        printf( "Searching for or Writing Key %s in file %s failed!\n", 
            lpszKeyName, lpszProfileName );
#endif*/
        fclose( stream );
        return( FALSE );
    }
    
    /* record the whole new file */
    lpszBuf = (s8*)malloc( sdwOffset );
    ASSERT( lpszBuf );
    fseek( stream, 0, SEEK_SET );
    sdwOffset = fread( lpszBuf, sizeof( char ), sdwOffset, stream );
    fclose( stream );

    /* rewrite the file */
    stream = fopen( lpszProfileName, "wb" );
    /* ASSERT( stream != NULL ); */
    if( stream == NULL )
    {
        free( lpszBuf );
        /*printf( "Open file %s failed!\n", lpszProfileName );*/
        return( FALSE );
    }
    fwrite( lpszBuf, sizeof( char ), sdwOffset, stream );
    fclose( stream );

    free( lpszBuf );
    return( TRUE );
}


        
/*====================================================================
    ������      ��DeleteRegKey
    ����        ���������ļ���ɾ��һ��Key
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����const s8* lpszProfileName, �ļ�����������·���� 
                  const s8* lpszSectionName, Profile�еĶ���
                  const s8* lpszKeyName, ���ڵĹؼ�����
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/02/01    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 DeleteRegKey( const s8* lpszProfileName, 
                      const s8* lpszSectionName, 
                      const s8* lpszKeyName 
                    )
{
    BOOL32    bResult;

    bResult = SetRegKeyString( lpszProfileName, lpszSectionName, 
                lpszKeyName, NULL );
    
    return( bResult );
}
    
#ifdef _VXWORKS_

extern STATUS	routeAdd( char *destination, char *gateway );
extern STATUS	mRouteAdd( char *, char *, long, int, int );

/*====================================================================
    ������      ��ReadRouteTable
    ����        ������RouteTable��Ϣ������·��
    �㷨ʵ��    ��
    ����ȫ�ֱ�������
    �������˵����const s8* lpszProfileName, �ļ�����������·����
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/06/26    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 ReadRouteTable( const s8* lpszProfileName )
{
    char    achEntry[MAX_VALUE_LEN + 1];
    char    chSeps[] = " \t";       /* space or tab as seperators */
    char    *pchToken;
    u32  dwLoop;
    BOOL32    bResult = TRUE;
    u32  dwEntryNum;
	char	achTemp[64];
	char	achDestIpAddr[32];
	char	achNextHopIpAddr[32];
	u32	dwMask;
    char    *pchStopStr;

    /* get the number of entry */
    bResult = GetRegKeyInt( lpszProfileName, SECTION_ipRouteTable, STR_ENTRY_NUM, 
                0, ( s32 * )&dwEntryNum );

    /* analyze entry strings */
    for( dwLoop = 0; dwLoop < dwEntryNum; dwLoop++ )
    {
        sprintf( achTemp, "%s%lu", STR_ENTRY, dwLoop );
		bResult = GetRegKeyString( lpszProfileName, SECTION_ipRouteTable, achTemp, 
			"cannot find the entry", achEntry, sizeof( achEntry ) ) ;

        if( !bResult )
			continue;

		/* ipRouteDest */
        pchToken = strtok( achEntry, chSeps );
        if( pchToken == NULL )
        {
            /*printf( "Wrong profile while reading %s\n!", FIELD_ipRouteDest );*/
            bResult = FALSE;
        }
        else
        {
            strncpy( achDestIpAddr, pchToken, sizeof( achDestIpAddr ) );
			achDestIpAddr[sizeof( achDestIpAddr ) - 1] = '\0';
        }
        /* ipRouteNextHop */
        pchToken = strtok( NULL, chSeps );
        if( pchToken == NULL )
        {
            /*printf( "Wrong profile while reading %s\n!", FIELD_ipRouteNextHop );*/
            bResult = FALSE;
        }
        else
        {
            strncpy( achNextHopIpAddr, pchToken, sizeof( achNextHopIpAddr ) );
			achNextHopIpAddr[sizeof( achNextHopIpAddr ) - 1] = '\0';
        }
        /* ipRouteMask */
        pchToken = strtok( NULL, chSeps );
        if( pchToken == NULL || ( memcmp( pchToken, "0x", 2 ) != 0 && memcmp( pchToken, "0X", 2 ) != 0 ) )
        {
            /*printf( "Wrong profile while reading %s\n!", FIELD_ipRouteMask );*/
            bResult = FALSE;
        }
        else
        {
            dwMask = strtoul( pchToken + 2, &pchStopStr, 16 );
        }

		/* add into route table */
#ifndef VXWORKS_SIMU
		if( dwMask == 0xffffffff )
		{
			routeAdd( achDestIpAddr, achNextHopIpAddr );
		}
		else
		{
			mRouteAdd( achDestIpAddr, achNextHopIpAddr, dwMask, 0, 0 );
		}
#endif
	}
    
    return( bResult );
}
#endif

/*====================================================================
    ������      ��Trim
    ����        ��ȥ���ַ�����ǰ���������ո��Լ�TAB��
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����s8* lpszSrc, �����Լ������ַ���
    ����ֵ˵��  ��void
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport void Trim( s8* lpszSrc )
{
    s32    sdwRightIndex;
    s32    sdwLeftIndex;

    sdwLeftIndex = 0;                           /* process left side first */
    while( ( lpszSrc[sdwLeftIndex] == ' ' || lpszSrc[sdwLeftIndex] == 0x09 ) 
        && lpszSrc[sdwLeftIndex] != '\0' )
    {
        sdwLeftIndex++;
    }
    sdwRightIndex = strlen( lpszSrc ) - 1;      /* then right side */
    while( ( lpszSrc[sdwRightIndex] == ' ' || lpszSrc[sdwRightIndex] == 0x09 ) 
        && sdwRightIndex > 0 )
    {
        sdwRightIndex--;
    }

    if( sdwRightIndex >= sdwLeftIndex )
    {
        strncpy( lpszSrc, lpszSrc + sdwLeftIndex, sdwRightIndex - sdwLeftIndex + 1 );
        lpszSrc[ sdwRightIndex - sdwLeftIndex + 1] = '\0';
    }
    else
    {
        lpszSrc[0] = '\0';
    }
}

/*====================================================================
    ������      ��StrUpper
    ����        ���ַ���ת���ɴ�д
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����s8* lpszSrc, �����Լ������ַ���
    ����ֵ˵��  ��void
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport void StrUpper( s8* lpszSrc )
{
    u16   wLoop;
    char    chDifference = 'A' - 'a';

    for( wLoop = 0; wLoop < strlen( lpszSrc ); wLoop++ )
    {
        if ( lpszSrc[wLoop] >= 'a' && lpszSrc[wLoop] <= 'z' )
        {
            lpszSrc[wLoop] += chDifference;
        }
    }
}

/*====================================================================
    ������      ��StrLower
    ����        ���ַ���ת����Сд
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����s8* lpszSrc, �����Լ������ַ���
    ����ֵ˵��  ��void
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport void StrLower( s8* lpszSrc )
{
    u16   wLoop;
    char    chDifference = 'A' - 'a';

    for( wLoop = 0; wLoop < strlen( lpszSrc ); wLoop++ )
    {
        if ( lpszSrc[wLoop] >= 'A' && lpszSrc[wLoop] <= 'Z' )
        {
            lpszSrc[wLoop] -= chDifference;
        }
    }
}

/*====================================================================
    ������      ��TableMemoryFree
    ����        ���ͷŵ���̬���루malloc���ı�ṹ�ڴ�
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����void **ppMem, ָ������ָ��
                  u32 dwEntryNum, ָ�������С
    ����ֵ˵��  ��void
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
/*DllExport void TableMemoryFree( void **ppMem, u32 dwEntryNum )
{
    u32   dwLoop;

    if( ppMem == NULL )
    {
        return;
    }

    for( dwLoop = 0; dwLoop < dwEntryNum; dwLoop++ )
    {
        if( ppMem[dwLoop] != NULL )
        {
            free( ppMem[dwLoop] );
        }
    }

    free( ppMem );
}*/

/*====================================================================
    ������      ��IsValidTime
    ����        ����֤�Ƿ�Ϸ�����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����u16 wYear, ��
                  u8 byMonth, ��
                  u8 byDay, ��
                  u8 byHour, ʱ
                  u8 byMin, ��
                  u8 bySec, ��
    ����ֵ˵��  ���Ϸ�����TRUE�����𷵻�FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/03/12    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL32 IsValidTime( u16 wYear, u8 byMonth, u8 byDay, 
                           u8 byHour, u8 byMin, u8 bySec )
{
    u8   byMaxDay;
    
    if( byMonth > 12 || byMonth < 1 || byHour >= 24 || byMin >= 60 || bySec >= 60 )
        return( FALSE );

    switch( byMonth )
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if( byDay < 1 || byDay > 31 )
            return( FALSE );
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if( byDay < 1 || byDay > 30 )
            return( FALSE );
        break;
    case 2:
        if( ( wYear % 100 != 0 && wYear % 4 == 0 ) || wYear % 400 == 0 )
            byMaxDay = 29;
        else
            byMaxDay = 28;

        if( byDay < 1 || byDay > byMaxDay )
            return( FALSE );
        break;
    default:
        return( FALSE );
        break;
    }

    return( TRUE );
}
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
DllExport BOOL32 DeleteSec(const s8* pszFileName,const s8* pszSecName)
{
    s8 achRow[MAX_LINE_LEN + 1];
    s8 achSecCopy[MAX_SECTIONNAME_LEN + 1];

    s8* pszReturn;
    s32 nLen;
    s32 nSecStartPos;
    s32 nNextOffSetLine;
    BOOL32 bFind = FALSE;

    FILE* pStream;
    s32 nRemainingLen;
    s8* pRemainingBuf;
    s32 nCurPos;
    s8* pFileBuf;

    strncpy(achSecCopy,pszSecName,MAX_SECTIONNAME_LEN);
    Trim((s8*)achSecCopy);
    StrUpper((s8*)achSecCopy);
    
    achSecCopy[MAX_SECTIONNAME_LEN] = '\0';

    pStream = fopen(pszFileName,"rb+");
    if (pStream == NULL)
    {
        return FALSE;
    }
    
    do 
    {
        nNextOffSetLine = ftell(pStream);
        if (NULL == fgets(achRow,MAX_LINE_LEN,pStream))
        {   
            fseek(pStream,0,SEEK_END);
            nNextOffSetLine = ftell(pStream);
            break;
        }
        if ((pszReturn = strstr(achRow,"\r\n")) != NULL)
        {
            pszReturn[0] = '\0';
        }
        else if ((pszReturn = strstr(achRow,"\n")) != NULL)
        {
            pszReturn[0] = '\0';
        }

        Trim(achRow);
        nLen = strlen(achRow);
        if (nLen < 2 && achRow[0] != '[' || achRow[nLen-1] != ']')
        {
           continue;
        }
        
        //���ָ�������Ѿ��ҵ�����ô��λ�þ������εĽ�ֹλ��
        if (bFind)
        {
            break;
        }

        strncpy(achRow,achRow + 1,nLen -2);
        achRow[nLen - 2] = '\0';
        StrUpper(achRow);
        if (0 == strcmp(achRow,achSecCopy))
        {
            bFind = TRUE;//�ҵ����ε���ʼλ��
            nSecStartPos = nNextOffSetLine;
        }
        
    } while (TRUE); 

    if (!bFind)
    {
        return FALSE;
    }
    fseek(pStream,0,SEEK_END);
    nRemainingLen = ftell(pStream) - nNextOffSetLine;
    pRemainingBuf = NULL;//�����ļ�ʣ������

    pRemainingBuf = (s8*)malloc(nRemainingLen);

    if (pRemainingBuf==NULL)
    {
        return FALSE;
    }
    fseek(pStream,nNextOffSetLine,SEEK_SET);
    fread(pRemainingBuf,sizeof(s8),nRemainingLen,pStream);//��ȡ�¸�����֮�����������
    
    //�ڵ�ǰ���ε�λ��д�¸�����֮�������
    fseek(pStream,nSecStartPos,SEEK_SET);
    fwrite(pRemainingBuf,sizeof(s8),nRemainingLen,pStream);
    free(pRemainingBuf);
    
    nCurPos = ftell(pStream);

    //��ȡд��֮��ǰλ��֮ǰ���ļ������ݣ����¸���ԭ�����ļ�
    pFileBuf = NULL;
    pFileBuf = (s8*)malloc(nCurPos);
    if (pFileBuf == NULL)
    { 
        return FALSE;
    }
    fseek(pStream,0,SEEK_SET);
    fread(pFileBuf,sizeof(s8),nCurPos,pStream);  
    fclose(pStream);

    pStream = fopen(pszFileName,"wb");
    if (pStream == NULL)
    {
        free(pFileBuf);
        return FALSE;
    }
    fwrite(pFileBuf,sizeof(s8),nCurPos,pStream);
    fclose(pStream);
    free(pFileBuf);

    return TRUE;
}

FILE *g_pStream = NULL;
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
								  s8* pchValueBuf,s32 nValueMax)
{
    s8 achRow[MAX_LINE_LEN + 1];
    s8* pszReturn;
    s32 nLen;
    if (pszFileName != NULL && pszSecName != NULL)
    {
        if (g_pStream == NULL)
        {
            g_pStream = fopen(pszFileName,"rb");
            if (g_pStream == NULL)
            {
                return FALSE;
            }
        }
    
        if (MoveToSection(g_pStream,pszSecName,FALSE) == -1)
        {
            fclose(g_pStream);
            g_pStream = NULL;
            return FALSE;
        }
    }
  
    do 
    {
        if (fgets(achRow,MAX_LINE_LEN,g_pStream) == NULL)
        {
            if (g_pStream != NULL)
            {
                fclose(g_pStream);
                g_pStream = NULL;
            }
            return FALSE;
        }
        if ((pszReturn = strstr(achRow,"\r\n")) != NULL)
        {
            pszReturn[0] = '\0';
        }
        else if ((pszReturn = strstr(achRow,"\n")) != NULL)
        {
            pszReturn[0] = '\0';
        }
        Trim(achRow);
        nLen = strlen(achRow);
        if (nLen > 2 && achRow[0] == '[' && achRow[nLen - 1] == ']')
        {
            //���������һ�����Σ�˵�������ε������Ѿ���ȡ���
            if (g_pStream != NULL)
            {
                fclose(g_pStream);//�ر��ļ�
                g_pStream = NULL;
            }
            return FALSE;
        }
        else if(nLen == 0 || strstr( achRow, PROFILE_COMMENT1 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT2 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT3 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT4 ) == achRow )
        {
            //�������к�ע����
            continue;
        }
        else if ( nLen > 0)
        {
            //��ȡ�������е�һ������
            if ((pszReturn = strstr(achRow,"=")) != NULL)
            {
				strncpy(pchValueBuf,pszReturn+1,nValueMax - 1);
				pchValueBuf[nValueMax - 1] = '\0';
                Trim(pchValueBuf);

				pszReturn[0] = '\0';
				strncpy(pchKeyBuf,achRow,nKeyMax-1);
				pchKeyBuf[nKeyMax - 1] = '\0';
                Trim(pchKeyBuf);
                break;
            }
        }
    } while (TRUE);

    return TRUE;
}