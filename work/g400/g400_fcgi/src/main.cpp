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

using namespace std;

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#include "fcgi_stdio.h"
#include "xmldom.h"



static void PrintEnv(char *label, char **envp)
{
    printf("%s:<br>\n<pre>\n", label);
    for ( ; *envp != NULL; envp++) {
        printf("%s\n", *envp);
    }
    printf("</pre><p>\n");
}

int main ()
{
    char **initialEnv = environ;
    int count = 0;

    while (FCGI_Accept() >= 0) {

		// Read g400 xml message sample
		const char * XML_FILE_PATH = "/mnt/svn-work/g400/xml_msg_sample/login_ack.xml";

		printf("Content-type: text/xml\r\n"
	    "\r\n" );

		FILE* pXmlSample = fopen( XML_FILE_PATH, "r" );
		if( NULL == pXmlSample )
		{
			printf( "can't open file!!! %s", XML_FILE_PATH );
		}
		else
		{
			char arData[1000] = { 0 };
			while( fread( arData, 1, 1000, pXmlSample ) )
			{
				printf( "%s", arData );
				memset( arData, 0, sizeof( arData ) );
			}
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
		if( strQueryString.find( "dumpinput=1" ) != -1 )
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
			printf( "<dump2>%s</dump2>", strInput.c_str() );

			CXMLDom xmlTest;
			xmlTest.ParseString( "" );

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