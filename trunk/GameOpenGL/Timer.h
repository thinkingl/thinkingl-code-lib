#pragma once

#if 0

#include <windows.h>

#include <list>
#include <algorithm>

//	定时器
struct	OneTimer{
	int	id;					// id
	unsigned long time;		//	到期时间
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

	// 获得当前系统运行的时间.
	unsigned long	GetTimeMSec( void )		{ return GetTickCount (); }

	// 创建一个定时器,到指定的时间后返回1.否则返回0.
	int CreateTimer( int MSec );

	// 检测定时器是否到期
	bool CheckTimer( int id );

	//	检测是否有指定的id在定时器链表里.在定时器列表里找定时器id.
	bool IsExist( int id);
private:
	// singleton
	static Timer	*m_singleton;

	std::list <OneTimer>	 m_timer;



};

#endif 