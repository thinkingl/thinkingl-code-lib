#include <iostream>

#include <Windows.h>
#include <Winsock.h>
#include "APIHook.h"

#include "ThinkVPNCommonLib.h"

using namespace std;

void test()
{
    cout << "test dll!!!" << endl;
};

//WINUSERAPI
//DECLEAR_EX
int
WINAPI
MessageBoxWHook(
            __in_opt HWND hWnd,
            __in_opt LPCWSTR lpText,
            __in_opt LPCWSTR lpCaption,
            __in UINT uType)
{
	vpn::log << _T( "MessageBoxW call ! wnd: " ) << hWnd << endl;

     ::MessageBoxW( hWnd, _T( "Spy dll messagebox!" ), _T( "哈哈！"), MB_OK );

     
     return ::MessageBoxW( hWnd, lpText, lpCaption, uType );
}

int PASCAL FAR WSAStartup_Hook(
                          IN WORD wVersionRequired,
                          OUT LPWSADATA lpWSAData)
{
    vpn::log << _T( "WSAStartup call!" ) << endl;
    return WSAStartup( wVersionRequired, lpWSAData );   
}

int PASCAL FAR bind_Hook (
                     IN SOCKET s,
                     IN const struct sockaddr FAR *addr,
                     IN int namelen)
{
    vpn::log << _T( "bind call!" ) << endl;
    return bind( s, addr, namelen );
}

SOCKET PASCAL FAR socket_Hook (
						  IN int af,
						  IN int type,
						  IN int protocol)
{
	vpn::log << _T( "socket call!" ) << endl;
	return socket( af, type, protocol );
}

struct THookInfo
{
    string m_strCalleeModuleName;
    string m_strFunName;
    PROC m_pHookFun;

    THookInfo( LPCSTR strModule, LPCSTR strFun, void * funHook )
    {
        m_strCalleeModuleName = strModule;
        m_strFunName = strFun;
        m_pHookFun = (PROC)funHook;
    }
};

THookInfo g_arHookInfo[] =
{
    THookInfo( "WSOCK32.dll", "WSAStartup", WSAStartup_Hook ),
    THookInfo( "WSOCK32.dll", "bind", bind_Hook ),
	THookInfo( "WSOCK32.dll", "socket", socket_Hook ),
	THookInfo( "WS2_32.dll", "WSAStartup", WSAStartup_Hook ),
	THookInfo( "WS2_32.dll", "bind", bind_Hook ),
	THookInfo( "WS2_32.dll", "socket", socket_Hook ),
	THookInfo( "USER32.dll", "MessageBoxW", MessageBoxWHook ),
    THookInfo( "", "", 0 )
};



typedef std::vector< CAPIHook * > TAPIHookList;

TAPIHookList g_tAPIHookList;
//CAPIHook *g_pAPIHook = NULL;




BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad)
{
    

    // 获取自身的路径。
    tstring strDllPath = GetModulePath( GetModuleHandle( _T( "ThinkVPNSpyDll.dll" ) ) );

    tstring strLogDir = ParsePath( strDllPath.c_str() ).m_strDirectory;
    vpn::log.SetLogFileDir( strLogDir.c_str(), _T( "ThinkVPNSpyDll" )  );

    tofstream tmp;
    tmp.open( _T( "d:\\vpnspydllkkkkkaaaa" ) );
    tmp << "vpn log" << endl;
    tmp << _T( "vpn log" ) << endl;
    tmp << "log dir: " << strLogDir << endl;

    switch(fdwReason) 
    {
    case DLL_PROCESS_ATTACH:
        //The DLL is being mapped into the process's address space.
        vpn::log << _T( "VPN Spy DLL! The DLL is being mapped into the process's address space." ) << endl;
        //if ( NULL == g_pAPIHook )
        //{
        //    g_pAPIHook = new CAPIHook( ( "USER32.dll" ), ( "MessageBoxW" ), (PROC)MessageBoxWHook );
        //}
        for ( int i=0; g_arHookInfo[i].m_pHookFun != NULL; ++i )
        {
            g_tAPIHookList.push_back( new CAPIHook( g_arHookInfo[i].m_strCalleeModuleName.c_str(), 
                g_arHookInfo[i].m_strFunName.c_str(), g_arHookInfo[i].m_pHookFun ) );
        }
        
        break;

    case DLL_THREAD_ATTACH:
        //A thread is being created.
        vpn::log << _T( "VPN Spy DLL! A thread is being created. " ) << endl;
        break;

    case DLL_THREAD_DETACH:
        //A thread is exiting cleanly.
        vpn::log << _T( "VPN Spy DLL! A thread is exiting cleanly." ) << endl;
        break;

    case DLL_PROCESS_DETACH:
        //The DLL is being unmapped from the process's address space.
        vpn::log << _T( "VPN Spy DLL! The DLL is being unmapped from the process's address space. " ) << endl;
        //if ( g_pAPIHook )
        //{
        //    delete g_pAPIHook;
        //    g_pAPIHook = NULL;
        //}
        for ( TAPIHookList::iterator iter = g_tAPIHookList.begin(); iter != g_tAPIHookList.end(); ++iter )
        {
            delete *iter;
            *iter = 0;
        }

        break;
    }

    

    return(TRUE);  // Used only for DLL_PROCESS_ATTACH
}
