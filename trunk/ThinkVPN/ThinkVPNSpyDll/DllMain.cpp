#include <iostream>

using namespace std;

void test()
{
    cout << "test dll!!!" << endl;
};

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad)
{
    switch(fdwReason) 
    {
    case DLL_PROCESS_ATTACH:
        //The DLL is being mapped into the process's address space.
        break;

    case DLL_THREAD_ATTACH:
        //A thread is being created.
        break;

    case DLL_THREAD_DETACH:
        //A thread is exiting cleanly.
        break;

    case DLL_PROCESS_DETACH:
        //The DLL is being unmapped from the process's address space.
        break;
    }
    return(TRUE);  // Used only for DLL_PROCESS_ATTACH
}
