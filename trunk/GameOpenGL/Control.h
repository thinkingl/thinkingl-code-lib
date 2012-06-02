
//  Control.h
//  ������.
//  ��֮��
//  ���������Ϣ�ķ����ͷ���.
//  ���˺ܾ�,����������λ�ڵײ�.Ϊ����ģ���ṩ������Ĳ���ָ��.
//  ���������ʵ���û��Ŀ��ƶ���.

int const MAX_ORDER_NUM=500;	// �ü��̿��Բ��������������.
int const MOUSE_ORDER_NUM=5;	// ����ܷ��������

enum EOrderCmd
{
	CMD_EXIT,
	CMD_FORWARD_STEP,
	CMD_BACK_STEP,
	CMD_LEFT_STEP,
	CMD_RIGHT_STEP,
	CMD_CREATE_MAP,				// ������ͼ(û��ʵ��)
	CMD_DO_SOME_ACTION,		// ������㶯��.
	CMD_JUMP,
	CMD_FIRE,	// ��ǹ.

	CMD_RunScript,
	CMD_MainSalut,	// ���Ǿ���.
	CMD_OtherSalut,	// �����˾���.
	CMD_TurnLeft,		// ����ת
	CMD_TurnRight,	// ����ת.
	CMD_TurnUp,		// ����ת, ��Ұ����
	CMD_TurnDown,	// ����ת.
	CMD_OtherDie,		// Ⱥ����Ա����.

	CMD_NUM,	// ��������.
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

	//  �����û�������ֵ�.
	int  m_nCmdDictionary[MAX_ORDER_NUM+MOUSE_ORDER_NUM];

	bool	m_bInputMode ;		//  ��ʶ�����Ƿ�ʹ��DirectInput��������.
	bool     m_bUseCursor ;		//  �Ƿ�ʹ�������.

	InputReader m_inputReader;			//  ��������Ķ���.

	int nOrderList[MAX_ORDER_NUM];		//  �û��������������.�洢���û��������������.��һ��Ԫ�ش洢�������Ŀ.


public:

	// ����û������������С�
	int * GetOrderList( );
	// ��ʼ������
	bool Initialize( HWND hWnd );
	int GetMousePosX(){ return m_inputReader.GetMousePosX();}
	int GetMousePosY(){ return m_inputReader.GetMousePosY();}
	int GetMouseRelMoveX(){ return m_inputReader.GetMouseRelativeMoveX();}
	int GetMouseRelMoveY(){ return m_inputReader.GetMouseRelativeMoveY();}
	int GetMouseRelMoveZ(){ return m_inputReader.GetMouseRelativeMoveZ();}
	void Update() { m_inputReader.Update(); }
};
