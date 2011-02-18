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

#include "fcgi_stdio.h"

//static void PrintEnv(char *label, char **envp)
//{
//    printf("%s:<br>\n<pre>\n", label);
//    for ( ; *envp != NULL; envp++) {
//        printf("%s\n", *envp);
//    }
//    printf("</pre><p>\n");
//}

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

		fread( szInputBuf, 1, inputLen, stdin );

		CXMLDom inputXML, outXML;

		inputXML.ParseString( szInputBuf );

		CXMLMessageHandle::GetInstance()->HandleXMLMessage( inputXML, outXML );

		time_t now;
		time( &now );

//		char szTime[1000] = {0};
//		sprintf( szTime, "%ld", now );
//		outXML[G400XML::ELE_ROOT]["time"].Value( szTime );

		// update the time to avoid cache.
		printf( "Last-Modified: %d GMT\r\n", now );

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

	::HandleXMLMessage();
	return 0;

    while (FCGI_Accept() >= 0) {



		// Read g400 xml message sample
		const char * XML_FILE_PATH = "/mnt/svn-work/g400/xml_msg_sample/login_ack.xml";

		printf("Content-type: text/xml\r\n"
	    "\r\n" );

		FILE* pXmlSample = fopen( XML_FILE_PATH, "r" );
		char arData[10000] = { 0 };
		if( NULL == pXmlSample )
		{
			printf( "can't open file!!! %s", XML_FILE_PATH );
		}
		else
		{
			fread( arData, 1, sizeof( arData ), pXmlSample ) ;
//			printf( "ardata: \n %s \n", arData );

			fclose( pXmlSample );
			pXmlSample = NULL;
		}

		string strQueryString;
		char * pQS = getenv( "QUERY_STRING" );
		if( pQS )
		{
			strQueryString = pQS;
		}
		else
		{
			strQueryString = "dumpinput=1";
		}
		if( strQueryString.find( "dumpinput=1" ) != string::npos )
		{
			char *contentLength = getenv("CONTENT_LENGTH");
	        int len = 0;
	        string strInput;

	        if (contentLength != NULL) {
            	len = strtol(contentLength, NULL, 10);
	        }

	        for ( int i = 0; i < len; i++) {
                char ch = getchar();
                strInput.push_back( ch );
			}
//			printf( "<dump2>%s</dump2>\n", strInput.c_str() );

			CXMLDom xmlTest;
			xmlTest.ParseString( arData );

			xmlTest["KedacomXMLData"]["DumpInput"].Value( strInput.c_str() );

			printf( "%s", xmlTest.ToString().c_str() );

	//		printf( "end!!!!" );

		}
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

    } /* while */

    return 0;
}
