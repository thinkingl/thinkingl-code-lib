#pragma once

#if 0

#include <windows.h>

#include <list>
#include <algorithm>

//	��ʱ��
struct	OneTimer{
	int	id;					// id
	unsigned long time;		//	����ʱ��
};

class Timer
{
public:
	Timer(void);
public:
	~Timer(void);

public:
	// singleton functions
	static Timer	*GetInstance( void );

	static void	FreeInstance( void );

	// ��õ�ǰϵͳ���е�ʱ��.
	unsigned long	GetTimeMSec( void )		{ return GetTickCount (); }

	// ����һ����ʱ��,��ָ����ʱ��󷵻�1.���򷵻�0.
	int CreateTimer( int MSec );

	// ��ⶨʱ���Ƿ���
	bool CheckTimer( int id );

	//	����Ƿ���ָ����id�ڶ�ʱ��������.�ڶ�ʱ���б����Ҷ�ʱ��id.
	bool IsExist( int id);
private:
	// singleton
	static Timer	*m_singleton;

	std::list <OneTimer>	 m_timer;



};

#endif 