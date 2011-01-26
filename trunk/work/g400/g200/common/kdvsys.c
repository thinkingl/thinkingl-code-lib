/*****************************************************************************
   模块名      : KDV system
   文件名      : kdvsys.c
   相关文件    : 
   文件实现功能: KDV公共函数及宏定义
   作者        : 李屹
   版本        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2002/01/30  0.9         李屹        创建
   2003/08/25  1.0		   jianghy     去掉打印信息
   2005/05/22  3.6         libo        linux版实现
******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "kdvsys.h"

#ifdef _LINUX_
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

/*====================================================================
    函数名      ：OpenProfile
    功能        ：打开配置文件
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：tKeyHandle *lpKeyHandle, Profile名柄指针   
                  const s8* lpszProfileName, Profile文件名
                  BYTE  byOpenMode, 打开方式
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：CloseProfile
    功能        ：关闭配置文件
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：FILE *stream, Profile名柄   
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：WriteKeyValueString
    功能        ：从当前游标处开始查找指定关键字名并获得或设置对应内容
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：FILE *stream, Profile名柄   
                  s32 sdwOffsetCurLine, 要写入的行开始的偏移量
                  s32 sdwOffsetNextLine, 下一行开始的偏移量
                  const s8* lpszKeyName, 要写入的关键字名
                  const s8* lpszKeyValue, 要写入的关键字值，如果为NULL则
                    删除该行
    返回值说明  ：当前新结尾的偏移量，失败返回-1
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/31    1.0         Li, Yi        创建
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
    函数名      ：GetRegKeyInt
    功能        ：将游标移到指定的段名下一行开始处
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：FILE *stream, Profile名柄   
                  const s8* lpszSectionName, Profile中的段名
                  BOOL32 bCreate, 如果没有该段名是否创建
    返回值说明  ：返回偏移量Offset，如果失败返回-1
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：ReadWriteKeyValueString
    功能        ：从当前游标处开始查找指定关键字名并获得或设置对应内容
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：FILE *stream, Profile名柄   
                  const s8* lpszKeyName, 段内的关键字名
                  s8* lpszKeyValue, 返回或设置的关键字对应值字符串指针，
                    大小需预先分配，如果为写方式且该指针为NULL，则删除该
                    行
                  s32 sdwOffsetCurSection, 当前要读写的Section的起始
                    偏移量，从该Section后开始
                  u32 dwBufSize, 返回或设置的关键字对应值字符串Buffer的
                    大小，如不够则自动截断，最后一个字节填上'\0'
                  BOOL32 bWrite, 是否写，如果写的话则自动创建
    返回值说明  ：当前新结尾的偏移量，失败返回-1，READ操作该值无意义
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：GetRegKeyInt
    功能        ：从配置文件中读取一整数型值
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：const s8* lpszProfileName, 文件名（含绝对路径）
                  const s8* lpszSectionName, Profile中的段名   
                  const s8* lpszKeyName, 段内的关键字名    
                  const s32  sdwDefault, 失败时返回的默认值
                  s32  *sdwReturnValue, 返回值                         
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：GetRegKeyInt2
    功能        ：从配置文件中读取一整数型值
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：FILE *stream, 文件句柄
                  const s8* lpszSectionName, Profile中的段名   
                  const s8* lpszKeyName, 段内的关键字名    
                  const s32  sdwDefault, 失败时返回的默认值
                  s32  *sdwReturnValue, 返回值                         
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：GetRegKeyStringTable
    功能        ：从配置文件中读取一张表里的若干行数据
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：const s8* lpszProfileName, 文件名（含绝对路径）
                  const s8* lpszSectionName, Profile中的段名
                  const s8* lpszDefault, 失败时返回的默认值
                  s8*  lpszReturnValue, 返回字串
                  s8* *lpszEntryArray, 返回字符串数组指针
                  s32 *sdwEntryNum, 字符串数组数目，成功后返回字符串数
                        组实际数目，如传入的数目不够则多余的记录不予传回
                  u32 dwBufSize, 返回字串BUFFER的长度，如不够则截断，最
                        后一个字节填上'\0'
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/02/06    1.0         Li, Yi        创建
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
    函数名      ：GetRegKeyString
    功能        ：从配置文件中读取一字符串值
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：const s8* lpszProfileName, 文件名（含绝对路径）  
                  const s8* lpszSectionName, Profile中的段名
                  const s8* lpszKeyName, 段内的关键字名
                  const s8* lpszDefault, 失败时返回的默认值
                  s8*  lpszReturnValue, 返回字串
                  u32 dwBufSize, 返回字串BUFFER的长度，如不够则截断，最
                        后一个字节填上'\0'
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：GetRegKeyString2
    功能        ：根据文件句柄从配置文件中读取一字符串值，该文件需以
                    "rb"方式打开
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：FILE *stream, 文件句柄
                  const s8* lpszSectionName, Profile中的段名
                  const s8* lpszKeyName, 段内的关键字名
                  const s8* lpszDefault, 失败时返回的默认值
                  s8*  lpszReturnValue, 返回字串
                  u32 dwBufSize, 返回字串BUFFER的长度，如不够则截断，最
                        后一个字节填上'\0'
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：SetRegKeyInt
    功能        ：往配置文件中写入一新的整数型的值，如果不存在则自动创建
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：const s8* lpszProfileName, 文件名（含绝对路径）
                  const s8* lpszSectionName, Profile中的段名
                  const s8* lpszKeyName, 段内的关键字名
                  const s32 sdwValue, 新的整数值
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：SetRegKeyString
    功能        ：往配置文件中写入一新的字符串型的值，如果不存在则自动创建
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：const s8* lpszProfileName, 文件名（含绝对路径）   
                  const s8* lpszSectionName, Profile中的段名
                  const s8* lpszKeyName, 段内的关键字名
                  const s8* lpszValue, 新的字串值, NULL则表示删除改行
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：DeleteRegKey
    功能        ：从配置文件中删除一行Key
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：const s8* lpszProfileName, 文件名（含绝对路径） 
                  const s8* lpszSectionName, Profile中的段名
                  const s8* lpszKeyName, 段内的关键字名
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/02/01    1.0         Li, Yi        创建
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
    函数名      ：ReadRouteTable
    功能        ：读入RouteTable信息并设置路由
    算法实现    ：
    引用全局变量：无
    输入参数说明：const s8* lpszProfileName, 文件名（含绝对路径）
    返回值说明  ：TRUE/FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/06/26    1.0         Li, Yi        创建
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
    函数名      ：Trim
    功能        ：去除字符串的前导及后续空格以及TAB键
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：s8* lpszSrc, 输入以及返回字符串
    返回值说明  ：void
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：StrUpper
    功能        ：字符串转换成大写
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：s8* lpszSrc, 输入以及返回字符串
    返回值说明  ：void
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：StrLower
    功能        ：字符串转换成小写
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：s8* lpszSrc, 输入以及返回字符串
    返回值说明  ：void
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：TableMemoryFree
    功能        ：释放掉动态申请（malloc）的表结构内存
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：void **ppMem, 指针数组指针
                  u32 dwEntryNum, 指针数组大小
    返回值说明  ：void
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/01/30    1.0         Li, Yi        创建
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
    函数名      ：IsValidTime
    功能        ：验证是否合法日期
    算法实现    ：（可选项）
    引用全局变量：无
    输入参数说明：u16 wYear, 年
                  u8 byMonth, 月
                  u8 byDay, 日
                  u8 byHour, 时
                  u8 byMin, 分
                  u8 bySec, 秒
    返回值说明  ：合法返回TRUE，负责返回FALSE
----------------------------------------------------------------------
    修改记录    ：
    日  期      版本        修改人        修改内容
    02/03/12    1.0         Li, Yi        创建
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
        
        //如果指定区段已经找到，那么此位置就是区段的截止位置
        if (bFind)
        {
            break;
        }

        strncpy(achRow,achRow + 1,nLen -2);
        achRow[nLen - 2] = '\0';
        StrUpper(achRow);
        if (0 == strcmp(achRow,achSecCopy))
        {
            bFind = TRUE;//找到区段的起始位置
            nSecStartPos = nNextOffSetLine;
        }
        
    } while (TRUE); 

    if (!bFind)
    {
        return FALSE;
    }
    fseek(pStream,0,SEEK_END);
    nRemainingLen = ftell(pStream) - nNextOffSetLine;
    pRemainingBuf = NULL;//保存文件剩余内容

    pRemainingBuf = (s8*)malloc(nRemainingLen);

    if (pRemainingBuf==NULL)
    {
        return FALSE;
    }
    fseek(pStream,nNextOffSetLine,SEEK_SET);
    fread(pRemainingBuf,sizeof(s8),nRemainingLen,pStream);//读取下个区段之后的所有内容
    
    //在当前区段的位置写下个区段之后的内容
    fseek(pStream,nSecStartPos,SEEK_SET);
    fwrite(pRemainingBuf,sizeof(s8),nRemainingLen,pStream);
    free(pRemainingBuf);
    
    nCurPos = ftell(pStream);

    //读取写完之后当前位置之前的文件的内容，重新覆盖原来的文件
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
            //如果读到了一个区段，说明本区段的子项已经读取完毕
            if (g_pStream != NULL)
            {
                fclose(g_pStream);//关闭文件
                g_pStream = NULL;
            }
            return FALSE;
        }
        else if(nLen == 0 || strstr( achRow, PROFILE_COMMENT1 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT2 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT3 ) == achRow ||
            strstr( achRow, PROFILE_COMMENT4 ) == achRow )
        {
            //跳过空行和注释行
            continue;
        }
        else if ( nLen > 0)
        {
            //读取到区段中的一个子项
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