
#pragma once

// Ӣ�۵��ࡣ
// Hero.h
// created by ��֮�� 2006.4.21

#include "MymathArithmetic.h"
#include "ModelManager.h"

#include "IShowObject.h"

//const unsigned int FORWARD = 0;
//const unsigned int BACK = 1;
//const unsigned int LEFT = 2;
//const unsigned int RIGHT = 3;

enum MoveDirector { Forward , Back , Left , Right};


// ����Ĳ����ṹ
struct stRolePropertiy{
	int Level;		// �ȼ�
	int Exp;		// ����
	int MaxExp;		// ����һ������ľ���

	int Hp;			// ����
	int MaxHp;		// �������ֵ
	int Mp;			// ħ��
	int MaxMp;		// ħ�����ֵ


	int Attack;		// ����
	int Defend;		// ����

	int Magic;		// ħ��
	int Stamina;	// ����
	int Luck;		// ����
	int Agility;	// ����


	int Money;		// ��Ǯ
	WORD Arms;		// ����
	WORD Corselet;	// ����

	int CurGoodsNum;			// ��Ʒ����
//	short Goods[MAXGOODS];		// ��Ʒ�����ɴ�64�֣�
//	short GoodsNum[MAXGOODS];	// ÿ����Ʒ������

	int CurArmNum;				// װ������
//	short Arm[MAXARM];			// װ�������ɴ�32�֣�
//	short ArmNum[MAXARM];		// ÿ��װ��������

	int MagicsNum;				// ��ǰħ������
	short Magics[10];			// װ����ħ��

//	char Fight_Ini_Name[64];	// ս���н�ɫͼƬ���ݵ�ini�ļ�
//	int use;					// �����жϴ˽�ɫ�Ƿ�ʹ��
};

//����״̬
struct stRoleState{
	//------״̬����----------
	int xPos, yPos ;	// ��ɫ�ڵ�ǰ��ͼ�еĵ�ǰ���� �ŵ����ĵ�
//	float xCoordinate,yCoordinate,zCoordinate;		// ��ɫ����ʾʱ�ľ������ꡣ
	int mapID;			// ��ɫ���ڵ�ͼ���š�
//	int X, Y;			// ���ڸ�������
	Vector3f vDirection;// ��ɫ��ǰ�ķ���
	Vector3f vAbsolutePos;// ��ɫ��ǰ���Ե����ꡣ
	BOOL bActive;		// �Ƿ�
	BOOL bShow;			// �Ƿ�ɼ�

	int CurAttack;		// ��ɫ��ǰ�Ĺ���������ɫ��һЩ״̬����ʱ��Ӱ����Щ���ԡ���
	int CurDefend;		// ��ɫ��ǰ�ķ�����

	int Bp;				// ����0%-100%
	int Ip;				// ����0%-100%

	int CurMagic;		// ħ��
	int CurStamina;		// ����
	int CurLuck;		// ����
	int CurAgility;		// ����

	float fStature;		// ��ɫ���
	float fSpeed;		// ��ɫ���ƶ��ٶȡ�
	
//	LPDIRECTDRAWSURFACE Surface;			// ���ߵ�ҳ��
//	LPDIRECTDRAWSURFACE lpShadow;			// Ӱ��ҳ��
//	int SW, SH;			// Ӱ�Ӵ�С
//	char SurfaceFileName[32];	// ��Ӧ�ļ�
};



class CRole : public IShowObject
{

public:
	CRole(void);

	// singleton functions
// 	static CRole	*GetInstance();
// 
// 
// 	static void	FreeInstance();


public:
	~CRole(void);

public:
	virtual void Show();
public:
	bool Init( const stRoleState& initSt, int modelId );			// ��ʼ��Ӣ��״̬��
	
	stRolePropertiy GetRolePropertiy();
	stRoleState		GetRoleState();

	Vector3f GetRolePos();	// ��ý�ɫλ��.
	Vector3f GetRoleDirector();	// ��ý�ɫ����.

	bool SetRolePos( Vector3f rolePos);

	bool SetRoleDirector( Vector3f roleDir );

	

	// ��ָ��������һ��.
	bool Walk( enum MoveDirector dir);

	// �ı��ɫ����
	bool UpdateDirection( int xMoved , int yMoved );

	// ��ɫ�Ķ���
	void DoAct(  int action, int autoStopTimeMsec = 500 );

	// ʹ��ɫ�Ķ�����ԭ.

private:
	// ֹͣ��ɫ�����Ķ�ʱ���ص�.
	static void WINAPI	StopActTimerProc (UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);

	void ResetStopActionTimer( int autoStopMSec );
	void KillStopActionTimer();
private:
//	static CRole *m_singleton;

	UINT m_stopActionTimerId;
//	ModelManager * pModelManager;	//	ģ�͹������ָ��.

	stRolePropertiy rolePropertiy;	// Ӣ������
	stRoleState m_roleState;			// Ӣ��״̬��

	int m_curRoleAct;
	//	��ʱ��ָ��
//	Timer *m_pTimer ;

	//	��ɫ����ִ�е�ָ��
//	int m_doing;

	// ��ɫ��ģ��id.
	int m_modelId;

};

typedef std::vector< CRole* > CRolePointList;

