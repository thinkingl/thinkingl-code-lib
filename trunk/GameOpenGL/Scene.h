//������Ϸ����,Ҳ���Ǽ�����Ϸ��ͼ.
//��Ϸ��ͼһ�����ؾͲ��ٸı�,�������¼���.
//һ����˵,������Ϸ��ͼ�ܸ���,�Ӵ�,��Ҫ�ķ��൱��ʱ��.
//��������Ϸ��ͼ����֮ǰ�ɷ�������ͻ��˴�����Ϸ��ͼ,�����������Ϳ������Ŀ��Ƶ�ͼ.
//��Ϸ�����ļ�Ӧ�����ľ������.
#include "stdafx.h"


#pragma once

class Scene
{
public:
	Scene(void);
	~Scene(void);
	//��������ʾ����.
	int DrawTheScene();
	//װ�س���,���ɵ�ͼ����ʾ�б�.
	int LoadScene(void);
	//����װ�س���,�ӵ�ͼװ�ص�ͼ.
	int LoadScene( char * FileName);
private:
	//��ͼ����ʾ�б������ֵ..
	GLuint SceneDisplayList;

	// �����
	void DrawSky(int mapID ,int a,int wi,int he,int le);

};
