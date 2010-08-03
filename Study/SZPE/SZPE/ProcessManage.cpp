#include "StdAfx.h"
#include "ProcessManage.h"
#include "psapi.h"


CProcessManage::CProcessManage(void)
{
}


CProcessManage::~CProcessManage(void)
{
}

BOOL CProcessManage::Update()
{
	// 列出所有进程。
	DWORD aProcesses[1024*10], cbNeeded, cProcesses;
	memset( aProcesses, 0, sizeof( aProcesses ) );

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return FALSE;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

	for( size_t i=0; i<cProcesses; ++i )
	{
		TProcessId processId = aProcesses[i];
		if( this->m_processesTable.find( processId ) == this->m_processesTable.end() )
		{
			CProcess newProcess( processId );
			newProcess.Update();
			this->m_processesTable[ processId ] = newProcess;
		}
		else
		{
			this->m_processesTable[ processId ].Update();
		}
	}


	return TRUE;
}

void CProcessManage::GetAllProcessId( TProcessIdList& allProcessIdList ) const
{
	allProcessIdList.clear();

	TProcessTable::const_iterator cIt = this->m_processesTable.begin();
	for( ; cIt != m_processesTable.end(); ++cIt )
	{
		allProcessIdList.push_back( cIt->first );
	}
}

BOOL CProcessManage::GetProcess( TProcessId proId, CProcess& process ) const
{
	TProcessTable::const_iterator cIt = this->m_processesTable.find( proId );
	if( cIt != m_processesTable.end() )
	{
		process = cIt->second;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}