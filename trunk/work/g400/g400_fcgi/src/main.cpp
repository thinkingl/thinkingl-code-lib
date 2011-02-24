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

bool HandleXMLMessage()
{
	const int CON_BUFLEN_DEF = 1024*100;
	char szInputBuf[ CON_BUFLEN_DEF ];

	while (FCGI_Accept() >= 0)
	{
		memset( szInputBuf, 0, sizeof( szInputBuf ) );
		// read the stdin to get the post data.
		char *contentLength = getenv("CONTENT_LENGTH");
		int inputLen = 0;
		if (contentLength != NULL)
		{
			inputLen = strtol(contentLength, NULL, 10);
		}

		int nlen = fread( szInputBuf, 1, inputLen, stdin );

		CXMLDom inputXML, outXML;

		if( nlen > 0 )
		{
			inputXML.ParseString( szInputBuf );

			CXMLMessageHandle::GetInstance()->HandleXMLMessage( inputXML[ G400XML::ELE_ROOT ], outXML[ G400XML::ELE_ROOT ] );
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
	return true;
}

int main ()
{
//    char **initialEnv = environ;
//    int count = 0;
	CUnitTestXMLMessageHandler utXmlHandler;
	CXMLMessageHandle::GetInstance()->RegObserver( &utXmlHandler );

	::HandleXMLMessage();


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
