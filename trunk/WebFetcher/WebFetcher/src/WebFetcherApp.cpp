#include "WebFetcherApp.h"
#include "portabledefine.h"

LPCTSTR CMD_EXIT = _T( "exit" );

CWebFetcherApp::CWebFetcherApp(void)
{
}

CWebFetcherApp::~CWebFetcherApp(void)
{
}

int CWebFetcherApp::Run()
{
    tstring strCmd;
    
    while( strCmd != CMD_EXIT )
    {
        tcin >> strCmd;
    }
    

    return 0;
}