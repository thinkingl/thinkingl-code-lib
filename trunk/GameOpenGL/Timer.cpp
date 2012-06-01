#include "Timer.h"

Timer *Timer::m_singleton = 0;

Timer::Timer(void)
{
}

Timer::~Timer(void)
{
}

Timer *Timer::GetInstance()
{
	if( m_singleton == 0 )
		m_singleton = new Timer;

	return ((Timer *)m_singleton);
}

void Timer::FreeInstance()
{
	if( m_singleton != 0 )
	{
		delete m_singleton;
		m_singleton = 0;
	}
}

// 创建一个定时器.
int Timer::CreateTimer(int MSec)
{
	//	首先, 要找到一个空的id.
	OneTimer timer;
	timer.id = rand();
	if( IsExist( timer.id ))
		CreateTimer( MSec );		//	如果已经存在了,则递归.

	timer.time = GetTickCount();
	m_timer.push_back( timer );
	
	return timer.id;
}


//	检测该定时器是否存在.有返回1,否返回0.
bool Timer::IsExist( int id )
{
	std::list<OneTimer>::iterator	timerIterator;
	for( timerIterator = m_timer.begin(); timerIterator != m_timer.end(); timerIterator++ )
	{
		if( timerIterator->id == id )
			return true;
	}
	return false;
}

//	检测定时器是否到期.......如果到期则返回值为 true ,否则返回false.
bool Timer::CheckTimer(int id)
{
	std::list<OneTimer>::iterator	timerIterator;
	for ( timerIterator = m_timer.begin(); timerIterator != m_timer.end();timerIterator++)
	{
		if( timerIterator->id == id )
		{
			if( timerIterator->time < GetTickCount())
			{

				return true;
			}
		}
	}
	return false;
}