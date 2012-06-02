#include "ScriptParse.h"
#include <string>

using namespace std;


CScriptParse::CScriptParse(void)
	:m_finishedTickcount( 0 )
{
	this->Reset();
}


CScriptParse::~CScriptParse(void)
{
}

COrderList CScriptParse::GetOrderList( int curTickCount )
{
	if ( m_finishedTickcount == 0 )
	{
		m_finishedTickcount = curTickCount;
	}

	COrderList orderList;
	while ( m_finishedTickcount <= curTickCount )
	{
		if ( !m_scriptFile )
		{
			break;
		}
				
		std::string order;
		m_scriptFile >> order;
		int timeWait;
		m_scriptFile >> timeWait;
		m_finishedTickcount += timeWait;

		if ( order == "wait" )
		{			
			continue;
		}
		else if( order == "sound" )
		{
			string soundFile;
			m_scriptFile >> soundFile;
			PlaySound( soundFile.c_str(),NULL,SND_ASYNC );
		}
		else if( order == "mainsalut" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_MainSalut;
			newOrder.m_time = timeWait;
			orderList.push_back( newOrder );
		}
		else if( order == "othersalut" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_OtherSalut;
			newOrder.m_time = timeWait;
			orderList.push_back( newOrder );
		}
		else if( order == "turnleft" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_TurnLeft;
			newOrder.m_time = timeWait;
			orderList.push_back( newOrder );
		}
		else if( order == "turnright" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_TurnRight;
			newOrder.m_time = timeWait;
			orderList.push_back( newOrder );
		}
		else if( order == "turndown" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_TurnDown;
			newOrder.m_time = timeWait;
			orderList.push_back( newOrder );
		}
		else if( order == "turnup" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_TurnUp;
			newOrder.m_time = timeWait;
			orderList.push_back( newOrder );
		}
		else if( order == "fire" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_FIRE;
			newOrder.m_time = timeWait;
			orderList.push_back( newOrder );
		}	
		else if( order == "otherdie" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_OtherDie;
			newOrder.m_time = timeWait;

			m_scriptFile >> newOrder.m_param;

			orderList.push_back( newOrder );
		}
		
		else if( order == "exit" )
		{
			COrder newOrder;
			newOrder.m_cmd = CMD_RunScript;
			orderList.push_back( newOrder );
		}
	

	}

	return orderList;
}

void CScriptParse::Reset()
{
	m_finishedTickcount = 0;
	m_scriptFile.close();
	m_scriptFile.open( "./ini/run.txt" );
}