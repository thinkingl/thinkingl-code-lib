///CWorldModel.h//////////////////
///WorldModel类的头文件./////
///李之兴///
////这个类目的是实现一个世界的抽象模型.
//修正:以前范的严重的错误.
//我竟然想把生成显示列表的事情封装到这里....晕 了.............
//OK,现在这个类作为一个基本的大类,只管调用显示模块.让显示模块给我显示什么东西,他就得显示什么东西,当然不显示他也不管.
//好,他的任务就是维护一个要显示的东西的数据结构.一个列表.当然,这只是他和显示模块通信的接口部分.
//以后尽量不使用全局的变量,结构了.这回这个列表我不会全局....
//他还有好多的任务哦.因为所有的"实用的"信息都由他维护.以后有空再添加.
///@#$!$!!%$^$%&$%&^%&*&*(*()()(^*$%^@~#%~@~$@~~$#%$%@#~#$#^&$@@$/////////////////
////现在能想到却解决不了的问题:
///虚拟世界的获取,载入.包括:场景,各个对象的属性,如,人物的状态,物品的状态,地面的状态等.从哪里来?
//网络游戏,当然要从一个网络接口类里来了.....

//一些公用的头文件,库什么的都在这里面包含.
#include "MyMathArithmetic.h"
#include "VisualServerTemp.h"

#pragma once

#include "MymathArithmetic.h"
#include "role.h"

//// 人物的参数结构
//struct RoleMoveStatus{
//	/*********角色移动用到的结构体**********/
//	float fSpeed;		// 角色的移动速度。
//	Vector3f vPos;		// 角色在显示时的绝对坐标。
//	Vector3f vRoleDirector; // 角色的方向.
//	Vector3f vMoveDirector;// 角色移动的方向.
//	Vector3f vUp;
//
//};



class CWorldModel
{
public:
	CWorldModel(void);
	~CWorldModel(void);


private:

//	RoleMoveStatus roleMoveStatus;
//	VisualServerTemp * m_visualServer;

	//将向量vRot绕轴v旋转angle角度 
	bool RotateV( Vector3f &vRot, Vector3f v,float angle);

	// 角色
	CRole m_mainRole;

	// 群众演员.
	typedef std::vector< CRole > CRoleList;
	CRoleList m_roleList;

public:
	// 显示绘制所有的人物. 临时这样处理, 放在这里并不合理.
	CRolePointList GetAllRole();

	CRole * GetMainRole();

public:

	// 初始化
	bool Init();

	bool UpdateViewDirection( int xMoved , int yMoved );

	// 向前走一步.
	void Forward(void);
	// 向后走一步.
	void Back(void);
	// 向左一步.
	void Left(void);
	// 向右一步.
	void Right(void);

	// 跳.
	void Jump();

	// 开火
	void Fire();

	// 让人物随便做点什么小动作.
	void RoleDosomeAction();

	// 更新现在角色的方向.
	//void UpdateRoleDirector(int xMoved , int yMoved);

	// 更新角色状态.
	//bool UpdateRoleState();


};