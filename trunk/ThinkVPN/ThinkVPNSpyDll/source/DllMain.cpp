#include <iostream>

#include "ThinkVPNCommonLib.h"

using namespace std;

void test()
{
    cout << "test dll!!!" << endl;
};

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad)
{
    

    // 获取自身的路径。
    tstring strDllPath = GetModulePath( GetModuleHandle( _T( "ThinkVPNSpyDll.dll" ) ) );

    tstring strLogDir = ParsePath( strDllPath.c_str() ).m_strDirectory;
    vpn::log.SetLogFileDir( strLogDir.c_str(), _T( "ThinkVPNSpyDll" )  );

    tofstream tmp;
    tmp.open( _T( "c:\\vpnspydllkkkkkaaaa" ) );
    tmp << "vpn log" << endl;
    tmp << _T( "vpn log" ) << endl;
    tmp << "log dir: " << strLogDir << endl;

    switch(fdwReason) 
    {
    case DLL_PROCESS_ATTACH:
        //The DLL is being mapped into the process's address space.
        vpn::log << _T( "VPN Spy DLL! The DLL is being mapped into the process's address space." ) << endl;
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
        break;
    }

    

    return(TRUE);  // Used only for DLL_PROCESS_ATTACH
}
