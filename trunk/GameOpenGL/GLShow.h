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
//	void LightContrl(bool order);// ���ƹ�ԴЧ���Ŀ���.
private:
	HWND  m_hWnd;			// ���ھ��
	HDC		m_hDC;			// GDI�豸������
	HGLRC	m_hRC;				// ������ɫ������

	//BOOL	SetupPixelFormat(HDC hDC);
	GLfloat LightAmbient[4] ;		//	Ambient light values.
	GLfloat LightDiffuse[4] ;		//	Diffuse light values.
	GLfloat LightPosition[4] ;		//	Light position.

	Objects m_object;
	Scene m_scene;

//	float xViewAt,yViewAt,zViewAt;	//�ֱ�ʵ���ӽǵ��ƶ�Ч��.

	

	void Show(void);
public:
	void ChangeFilter(void);

	// OpenGL�ĳ�ʼ�������豸��
	BOOL Init(HWND hWnd);
	bool InitOpenGL(void);

public:
	void AddShowObject( IShowObject* pShowObj );
private:
	CShowObjectPointList m_showObjectList;
};

