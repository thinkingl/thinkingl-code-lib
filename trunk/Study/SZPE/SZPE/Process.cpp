#include "StdAfx.h"
#include "Process.h"
#include <Psapi.h>


CProcess::CProcess( TProcessId processId )
{
	this->m_processId = processId;
	m_memoryPrivate = 0;
	m_memoryWorkingSet = 0;
}

CProcess::CProcess()
{
	this->m_processId = 0;
	m_memoryPrivate = 0;
	m_memoryWorkingSet = 0;
}

CProcess::~CProcess(void)
{
}

void CProcess::Update()
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
								   FALSE, this->m_processId );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, 
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }

		// ÄÚ´æÇé¿ö
		PROCESS_MEMORY_COUNTERS_EX pmc;
		memset( &pmc, 0, sizeof( pmc ) );
		pmc.cb = sizeof( pmc );
		BOOL bGetMemInfo = GetProcessMemoryInfo( hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof( pmc ) );
		if( bGetMemInfo )
		{
			this->m_memoryPrivate = pmc.PrivateUsage;
			this->m_memoryWorkingSet = pmc.WorkingSetSize;
		}
		else
		{
			DWORD dwErrCode = ::GetLastError();

		}
    }
	else
	{
		DWORD dwErrCode = ::GetLastError();

	}
    CloseHandle( hProcess );

	this->m_processName = szProcessName;

}

tstring CProcess::GetName() const
{
	return this->m_processName;
}

TProcessId CProcess::GetPID() const
{
	return this->m_processId;
}

int CProcess::GetMemPrivate() const
{
	return this->m_memoryPrivate;
}

int CProcess::GetMemWorkingSet() const
{
	return this->m_memoryWorkingSet;
}