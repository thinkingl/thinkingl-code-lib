// COpenGL.h: interface for the COpenGL class.
//
//////////////////////////////////////////////////////////////////////
#include "Objects.h"
#include "stdafx.h"
#include "Scene.h"
#include "IShowObject.h"

#pragma once


class COpenGL  
{	
public:	
	COpenGL();
	virtual ~COpenGL();
	public:


	BOOL	SetupPixelFormat(HDC hDC);

	void	Resize( );
	void	Render();
	void	CleanUp();
//	void LightContrl(bool order);// 控制光源效果的开关.
private:
	HWND  m_hWnd;			// 窗口句柄
	HDC		m_hDC;			// GDI设备描述表
	HGLRC	m_hRC;				// 永久着色描述表

	//BOOL	SetupPixelFormat(HDC hDC);
	GLfloat LightAmbient[4] ;		//	Ambient light values.
	GLfloat LightDiffuse[4] ;		//	Diffuse light values.
	GLfloat LightPosition[4] ;		//	Light position.

	Objects m_object;
	Scene m_scene;

//	float xViewAt,yViewAt,zViewAt;	//分别实现视角的移动效果.

	

	void Show(void);
public:
	void ChangeFilter(void);

	// OpenGL的初始化。绑定设备。
	BOOL Init(HWND hWnd);
	bool InitOpenGL(void);

public:
	void AddShowObject( IShowObject* pShowObj );
private:
	CShowObjectPointList m_showObjectList;
};

