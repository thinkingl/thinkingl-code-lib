//画出游戏场景,也就是加载游戏地图.
//游戏地图一经加载就不再改变,除非重新加载.
//一般来说,由于游戏地图很复杂,庞大,需要耗费相当的时间.
//可以在游戏地图加载之前由服务器向客户端传送游戏地图,这样服务器就可以灵活的控制地图.
//游戏配置文件应该做的尽量简洁.
#include "stdafx.h"


#pragma once

class Scene
{
public:
	Scene(void);
	~Scene(void);
	//将场景显示出来.
	int DrawTheScene();
	//装载场景,生成地图的显示列表.
	int LoadScene(void);
	//重载装载场景,从地图装载地图.
	int LoadScene( char * FileName);
private:
	//地图的显示列表的索引值..
	GLuint SceneDisplayList;

	// 画天空
	void DrawSky(int mapID ,int a,int wi,int he,int le);

};
