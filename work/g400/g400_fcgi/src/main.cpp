/*
 * echo.c --
 *
 *	Produce a page containing all FastCGI inputs
 *
 *
 * Copyright (c) 1996 Open Market, Inc.
 *
 * See the file "LICENSE.TERMS" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */
#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include "fcgi_config.h"

#include <stdlib.h>
#include <memory.h>
#include <string>

#include <time.h>
#include <errno.h>
#include <fstream>

using namespace std;

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif


#include "xmldom.h"
#include "xmlmessagehandle.h"
#include "g400define.h"
#include "unittestxmlmessagehandler.h"

#include "fcgi_stdio.h"

//static void PrintEnv(char *label, char **envp)
//{
//    printf("%s:<br>\n<pre>\n", label);
//    for ( ; *envp != NULL; envp++) {
//        printf("%s\n", *envp);
//    }
//    printf("</pre><p>\n");
//}
void UnitTest()
{
	printf( "== unit test out =====================================\n" );

	// unittest.
	ctstring strUnittestDir = "/mnt/svn-work/g400/xml_msg_sample/";
	ctstring strUnittestIn = strUnittestDir + "unittestin.xml";

	CXMLDom unittestIn;
	unittestIn.ParseFile( strUnittestIn.c_str() );
	tstring strXMLInPath = strUnittestDir + unittestIn["utroot"][ "infile_1" ].Value();
	int nCursor = 1;
	while( strXMLInPath != strUnittestDir )
	{
		CXMLDom utinXML;
		utinXML.ParseFile( strXMLInPath.c_str() );

		CXMLDom utoutXML;

		CXMLMessageHandle::GetInstance()->HandleXMLMessage(
				utinXML[ G400XML::ELE_ROOT ], utoutXML[ G400XML::ELE_ROOT ] );

		printf( "unittest in: \n %s \n", utinXML.ToString().c_str() );
		printf( "unittest out: \n %s \n\n", utoutXML.ToString().c_str() );

		nCursor++;
		char szPath[100] = {0};
		sprintf( szPath, "infile_%d", nCursor );
		strXMLInPath = strUnittestDir + unittestIn["utroot"][ szPath ].Value();
	}
}

int GetInputLen()
{
	char *contentLength = getenv("CONTENT_LENGTH");
	int inputLen = 0;
	if (contentLength != NULL)
	{
		inputLen = strtol(contentLength, NULL, 10);
	}
	return inputLen;
}

string ReadLine( FILE *pFile, const char * chToken )
{
	return "";
}

bool HandleFileUpload( CXMLDom& outXML )
{
	char *queryString = getenv("QUERY_STRING");
	if( queryString )
	{
		const tstring UPLOAD_PARAM_TOKEN = "upload=";
		tstring strQueryString = queryString;
		size_t upPos = strQueryString.find( UPLOAD_PARAM_TOKEN );
		if( upPos != tstring::npos )
		{
			int upEnd = strQueryString.find( "&", upPos );
			upPos += UPLOAD_PARAM_TOKEN.size();
			// File upload req..
			tstring strPath = strQueryString.substr( upPos, upEnd-upPos );



			// Read the post in std input, to the file .
			FILE *pUploadFile = fopen( strPath.c_str(), "w+");
			if( pUploadFile )
			{
				// the total length of post data.
				int nInputLen = GetInputLen();
				// the end of upload file in post data.
				int nUploadFileEndPos = nInputLen;
				int nRWLen = 0;
				bool bReadHead = true;
				int nEndLen = 0;
				tstring strFileName;
				while( nRWLen < nInputLen )
				{
					char szBuf[1024*10] = {0};
					int rdLen = 0;
					if( bReadHead )
					{
						char *rdLine = gets( szBuf );

						// debug.
//						char szEle[100] = {0};
//						sprintf( szEle, "rdPos_%d", nRWLen );
//						outXML[G400XML::ELE_ROOT][G400XML::ELE_CONTENT][szEle].Value( rdLine );

						// the length of read  is the string + '\n' .
						rdLen = strlen( rdLine ) + 1 ;//fread( szBuf, 1, sizeof(szBuf), stdin );

						// the head end with a empty line "\r\n"
						if( strlen(rdLine) == 1 && *rdLine=='\r')
						{
							bReadHead = false;
						}

						// remove '\r' at the end.
						if( *rdLine && rdLine[ strlen(rdLine) - 1 ] == '\r')
						{
							rdLine[ strlen(rdLine) - 1 ] = 0;
						}

						if( nRWLen == 0 )
						{
							// the length of the end equal to the length of first line + 2 + 2.
							// eg. first line is ------WebKitFormBoundarytQNhytTAmfxwNQML
							// last line is ------WebKitFormBoundarytQNhytTAmfxwNQML--\r\n
							nEndLen = rdLen +2 + 2;
							nUploadFileEndPos = nInputLen - nEndLen;
						}
					}
					else if( nRWLen < nUploadFileEndPos )
					{
						// read the upload file.
						int nNeedRd = min( (int)sizeof( szBuf ), nUploadFileEndPos-nRWLen );
						rdLen = fread( szBuf, 1, nNeedRd, stdin );
						fwrite( szBuf, 1, rdLen, pUploadFile );

//						sleep(1000);
					}
					else
					{
						// read the end.
						int nNeedRd = sizeof( szBuf );
						rdLen = fread( szBuf, 1, nNeedRd, stdin );
					}

					if( 0 == rdLen )
					{
						break;
					}
					nRWLen += rdLen;
				}
				fclose( pUploadFile );
				pUploadFile = NULL;
				outXML[G400XML::ELE_ROOT][G400XML::ELE_STATUS].Value( true );
//				outXML[G400XML::ELE_ROOT][G400XML::ELE_CONTENT]["path"].Value( strPath.c_str() );
			}
			else
			{
				outXML[G400XML::ELE_ROOT][G400XML::ELE_STATUS].Value( false );
				outXML[G400XML::ELE_ROOT][G400XML::ELE_CONTENT]["errcode"].Value( errno );
				outXML[G400XML::ELE_ROOT][G400XML::ELE_CONTENT]["error"].Value( strerror( errno ) );
				outXML[G400XML::ELE_ROOT][G400XML::ELE_CONTENT][G400XML::ELE_ERRMSG].Value( strerror( errno ) );
			}

			return true;
		}
	}
	return false;
}

bool HandleXMLMessage( CXMLDom& outXML )
{
	const int CON_BUFLEN_DEF = 1024*100;
	char szInputBuf[ CON_BUFLEN_DEF ];

	memset( szInputBuf, 0, sizeof( szInputBuf ) );
	// read the stdin to get the post data.
	int inputLen = GetInputLen();

	int nlen = fread( szInputBuf, 1, min( inputLen, CON_BUFLEN_DEF ), stdin );

	CXMLDom inputXML;

	if( nlen == inputLen )
	{
		inputXML.ParseString( szInputBuf );

		CXMLMessageHandle::GetInstance()->HandleXMLMessage( inputXML[ G400XML::ELE_ROOT ], outXML[ G400XML::ELE_ROOT ] );
	}
	else if( nlen < inputLen )
	{
		outXML[G400XML::ELE_ROOT][G400XML::ELE_STATUS].Value( 0 );
		outXML[G400XML::ELE_ROOT][G400XML::ELE_CONTENT][G400XML::ELE_ERRMSG].Value( "Can't read all input data! Maybe input too long!" );
	}
	else if( nlen == 0 )
	{
		return false;
	}
	return true;
}

int main ()
{
//    char **initialEnv = environ;
//    int count = 0;
	CUnitTestXMLMessageHandler utXmlHandler;
	CXMLMessageHandle::GetInstance()->RegObserver( &utXmlHandler );

	while (FCGI_Accept() >= 0)
	{
		CXMLDom outXML;

		if( ::HandleFileUpload( outXML ) )
		{

		}
		else if( ::HandleXMLMessage( outXML ) )
		{

		}
		else
		{
			::UnitTest();
		}


		time_t now;
		time( &now );
		int nRadom = rand();

		// update the time to avoid cache.
		printf( "Last-Modified: %d %d GMT\r\n", now, nRadom );

		// return xml.
		printf("Content-type: text/xml\r\n"
				"\r\n" );
		printf( "%s", outXML.ToString().c_str() );
	}

	CXMLMessageHandle::ReleaseInstance();

	return 0;

/*
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

	printf("Content-type: text/html\r\n"
	    "\r\n"
	    "<title>FastCGI echo</title>"
	    "<h1>FastCGI echo</h1>\n"
            "Request number %d,  Process ID: %d<p>\n", ++count, getpid());

        if (contentLength != NULL) {
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
        }

        if (len <= 0) {
	    printf("No data from standard input.<p>\n");
        }
        else {
            int i, ch;

	    printf("Standard input:<br>\n<pre>\n");
            for (i = 0; i < len; i++) {
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
		}
                putchar(ch);
            }
            printf("\n</pre><p>\n");
        }

        PrintEnv("Request environment", environ);
        PrintEnv("Initial environment", initialEnv);
        */


    return 0;
}
