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

// ����һ����ʱ��.
int Timer::CreateTimer(int MSec)
{
	//	����, Ҫ�ҵ�һ���յ�id.
	OneTimer timer;
	timer.id = rand();
	if( IsExist( timer.id ))
		CreateTimer( MSec );		//	����Ѿ�������,��ݹ�.

	timer.time = GetTickCount();
	m_timer.push_back( timer );
	
	return timer.id;
}


//	���ö�ʱ���Ƿ����.�з���1,�񷵻�0.
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

//	��ⶨʱ���Ƿ���.......��������򷵻�ֵΪ true ,���򷵻�false.
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