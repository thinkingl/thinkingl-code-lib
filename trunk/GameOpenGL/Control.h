
//  Control.h
//  控制类.
//  李之兴
//  负责控制信息的分析和发送.
//  想了很久,决定把他定位在底层.为其他模块提供处理过的操作指令.
//  在这里可以实现用户的控制定制.

int const MAX_ORDER_NUM=500;	// 用键盘可以产生出的最多命令.
int const MOUSE_ORDER_NUM=5;	// 鼠标能发出的命令。

enum EOrderCmd
{
	CMD_EXIT,
	CMD_FORWARD_STEP,
	CMD_BACK_STEP,
	CMD_LEFT_STEP,
	CMD_RIGHT_STEP,
	CMD_CREATE_MAP,				// 创建地图(没有实现)
	CMD_DO_SOME_ACTION,		// 随便做点动作.
	CMD_JUMP,
	CMD_FIRE,	// 开枪.

	CMD_RunScript,
	CMD_MainSalut,	// 主角军礼.
	CMD_OtherSalut,	// 其他人军礼.
	CMD_TurnLeft,		// 向左转
	CMD_TurnRight,	// 向右转.
	CMD_TurnUp,		// 向上转, 视野向下
	CMD_TurnDown,	// 向下转.
	CMD_OtherDie,		// 群众演员挂了.

	CMD_NUM,	// 命令总数.
};




#include "stdafx.h"
#include "CustomkeyIniSet.h"
#include "InputReader.h"
#pragma once

class Control
{
public:
	Control(void);
	~Control(void);
private:

	//  翻译用户输入的字典.
	int  m_nCmdDictionary[MAX_ORDER_NUM+MOUSE_ORDER_NUM];

	bool	m_bInputMode ;		//  标识当今是否使用DirectInput处理输入.
	bool     m_bUseCursor ;		//  是否使用鼠标光标.

	InputReader m_inputReader;			//  管理输入的对象.

	int nOrderList[MAX_ORDER_NUM];		//  用户发出的命令队列.存储着用户发出的命令代号.第一个元素存储命令的数目.


public:

	// 获得用户输入的命令队列。
	int * GetOrderList( );
	// 初始化函数
	bool Initialize( HWND hWnd );
	int GetMousePosX(){ return m_inputReader.GetMousePosX();}
	int GetMousePosY(){ return m_inputReader.GetMousePosY();}
	int GetMouseRelMoveX(){ return m_inputReader.GetMouseRelativeMoveX();}
	int GetMouseRelMoveY(){ return m_inputReader.GetMouseRelativeMoveY();}
	int GetMouseRelMoveZ(){ return m_inputReader.GetMouseRelativeMoveZ();}
	void Update() { m_inputReader.Update(); }
};
