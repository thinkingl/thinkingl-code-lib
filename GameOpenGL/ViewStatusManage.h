///李之兴
//2005.12.24圣诞前夜.
//处理屏幕显示.观察者的方位.
#include "MyMathArithmetic.h"



#define	VIEW_FPS  0	// 一个圆柱形的转动模型..以一个人的视角看世界.向上的方向一直是Y轴正方向.仰头和低头都不会超过90度
						// 视野都是绕向上的方向转动,也就是永远都绕Y轴转.转动的角度和鼠标滑动的距离成正比.
#define VIEW_WOW	1	// 类似于魔兽世界的视点方式。视点保持在角色的后上方。

#define VIEW_GOD	2	// 游戏世界上空的观察模式.默认的就是空中45度,应该可以更改的有观察的距离和角度.暂时没弄明白.

#define VIEW_FPS_SPEED	0.001		// 在圆柱转动模型中鼠标移动控制的视角转动的速度.
#define VIEW_FPS_RADII	100			// 视角旋转模型圆柱的半径.
#define VIEW_FPS_MAX_RAD	1.5/VIEW_FPS_SPEED			// 视角旋转模型竖直方向的视角调整角度,不能超过90度.
#define HERO_HEIGHT	2.3f	// 定义英雄的高度,也就是视点的高度.

#pragma once

struct ViewWow		// 魔兽世界式的观察模式时,存储距离和角度
{
	float viewDistance;		// 视点距离角色的距离
	float viewAngle;		// 视点和角色之间连线和地平面之间的角度.
};

class Camera
{
public:
	Camera(void);
	~Camera(void);

private:

	// 视点的状态,控制着屏幕观察的方位.
	VIEWSTATUS	viewStatus;
	// 屏幕显示控制模式:
	int		viewModal;
	// 存放当前英雄的位置.
	Vector3f rolePosition;
	// 存放当前角色的方向.
	Vector3f roleDirection;

	ViewWow wowView;	
	//将向量vRot绕轴v旋转angle角度 
	//bool RotateV( Vector3f &vRot, Vector3f v,float angle);

public:
	// 改变显示视角模型模式
	void SetViewModal(int viewModal);
	// 获得当前的视角模型参数.
	VIEWSTATUS GetViewStatus(void);

	// 实现当前的视点位置和视角。
	bool Update();

	// 更新现在观看的视角.
	//void SetCamera(int xMoved , int yMoved);
};
