
#pragma once

// 英雄的类。
// Hero.h
// created by 李之兴 2006.4.21

#include "MymathArithmetic.h"
#include "ModelManager.h"

#include "IShowObject.h"

//const unsigned int FORWARD = 0;
//const unsigned int BACK = 1;
//const unsigned int LEFT = 2;
//const unsigned int RIGHT = 3;

enum MoveDirector { Forward , Back , Left , Right};


// 人物的参数结构
struct stRolePropertiy{
	int Level;		// 等级
	int Exp;		// 经验
	int MaxExp;		// 到下一级所需的经验

	int Hp;			// 生命
	int MaxHp;		// 生命最大值
	int Mp;			// 魔法
	int MaxMp;		// 魔法最大值


	int Attack;		// 攻击
	int Defend;		// 防御

	int Magic;		// 魔力
	int Stamina;	// 耐力
	int Luck;		// 幸运
	int Agility;	// 敏捷


	int Money;		// 金钱
	WORD Arms;		// 武器
	WORD Corselet;	// 护具

	int CurGoodsNum;			// 物品总数
//	short Goods[MAXGOODS];		// 物品（最多可带64种）
//	short GoodsNum[MAXGOODS];	// 每种物品的数量

	int CurArmNum;				// 装备总数
//	short Arm[MAXARM];			// 装备（最多可带32种）
//	short ArmNum[MAXARM];		// 每种装备的数量

	int MagicsNum;				// 当前魔法总数
	short Magics[10];			// 装备的魔法

//	char Fight_Ini_Name[64];	// 战斗中角色图片数据的ini文件
//	int use;					// 用来判断此角色是否使用
};

//人物状态
struct stRoleState{
	//------状态数据----------
	int xPos, yPos ;	// 角色在当前地图中的当前坐标 脚底中心点
//	float xCoordinate,yCoordinate,zCoordinate;		// 角色在显示时的绝对坐标。
	int mapID;			// 角色所在地图代号。
//	int X, Y;			// 所在格子坐标
	Vector3f vDirection;// 角色当前的方向。
	Vector3f vAbsolutePos;// 角色当前绝对的坐标。
	BOOL bActive;		// 是否活动
	BOOL bShow;			// 是否可见

	int CurAttack;		// 角色当前的攻击。（角色的一些状态会临时的影响这些属性。）
	int CurDefend;		// 角色当前的防御。

	int Bp;				// 体力0%-100%
	int Ip;				// 精神0%-100%

	int CurMagic;		// 魔力
	int CurStamina;		// 耐力
	int CurLuck;		// 幸运
	int CurAgility;		// 敏捷

	float fStature;		// 角色身高
	float fSpeed;		// 角色的移动速度。
	
//	LPDIRECTDRAWSURFACE Surface;			// 行走的页面
//	LPDIRECTDRAWSURFACE lpShadow;			// 影子页面
//	int SW, SH;			// 影子大小
//	char SurfaceFileName[32];	// 对应文件
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
	bool Init( const stRoleState& initSt, int modelId );			// 初始化英雄状态。
	
	stRolePropertiy GetRolePropertiy();
	stRoleState		GetRoleState();

	Vector3f GetRolePos();	// 获得角色位置.
	Vector3f GetRoleDirector();	// 获得角色方向.

	bool SetRolePos( Vector3f rolePos);

	bool SetRoleDirector( Vector3f roleDir );

	

	// 向指定方向走一步.
	bool Walk( enum MoveDirector dir);

	// 改变角色方向
	bool UpdateDirection( int xMoved , int yMoved );

	// 角色的动作
	void DoAct(  int action, int autoStopTimeMsec = 500 );

	// 使角色的动作复原.

private:
	// 停止角色动作的定时器回调.
	static void WINAPI	StopActTimerProc (UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);

	void ResetStopActionTimer( int autoStopMSec );
	void KillStopActionTimer();
private:
//	static CRole *m_singleton;

	UINT m_stopActionTimerId;
//	ModelManager * pModelManager;	//	模型管理对象指针.

	stRolePropertiy rolePropertiy;	// 英雄属性
	stRoleState m_roleState;			// 英雄状态。

	int m_curRoleAct;
	//	定时器指针
//	Timer *m_pTimer ;

	//	角色正在执行的指令
//	int m_doing;

	// 角色的模型id.
	int m_modelId;

};

typedef std::vector< CRole* > CRolePointList;

