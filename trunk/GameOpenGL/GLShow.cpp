// COpenGL.cpp: implementation of the COpenGL class.
//������ƣ���֮��

//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
//#include "COpenGL.h"
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "Objects.h"
//#include "GLShow.h"
#include ".\glshow.h"
#include "role.h"
//////////////////////////////////////////////////////////////////////
extern HWND	hWnd;

//////////////////////////////////////////////////////////////////////



COpenGL::COpenGL()
{
	// ��ʼ������.
	LightAmbient[0] = LightAmbient[1] = LightAmbient[2] = LightAmbient[3] =  0.5f ;		//Ambient light values.
	LightDiffuse[0] = LightDiffuse[1] = LightDiffuse[2] = LightDiffuse[3] = 1.0f ;		//Diffuse light values.
	LightPosition[2] = 1.0f  ;		//Light position.
	LightPosition[0] = LightPosition[1] = LightPosition[3] = 0.0f;
}
COpenGL::~COpenGL()
{
	CleanUp();
}
BOOL COpenGL::SetupPixelFormat(HDC hDC)	  // ��ⰲװOpenGL
{	
	int nPixelFormat;					  // ���ص��ʽ

	m_hDC=hDC;
	PIXELFORMATDESCRIPTOR pfd = 
	{ 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
	    1,                                // �汾�� 
	    PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
	    PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
	    PFD_DOUBLEBUFFER,                 // ˫����ģʽ 
	    PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
	    16,                               // 16 λ��ɫ��� 
	    0, 0, 0, 0, 0, 0,                 // ������ɫλ 
	    0,                                // û�з�͸���Ȼ��� 
	    0,                                // ������λλ 
	    0,                                // ���ۼӻ��� 
	    0, 0, 0, 0,                       // �����ۼ�λ 
	    32,                               // 32 λ��Ȼ���     
	    0,                                // ��ģ�建�� 
	    0,                                // �޸������� 
	    PFD_MAIN_PLANE,                   // ���� 
	    0,                                // ���� 
	    0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
	}; 

	if (!(nPixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
		{ 
			MessageBox(NULL,"û�ҵ����ʵ���ʾģʽ","Error",MB_OK|MB_ICONEXCLAMATION);
			 return FALSE;
		}

	SetPixelFormat(m_hDC,nPixelFormat,&pfd);		//	���õ�ǰ�豸�����ص��ʽ
	m_hRC = wglCreateContext(m_hDC);				//	��ȡ��Ⱦ�������
	wglMakeCurrent(m_hDC, m_hRC);					//	������Ⱦ�������

/////////////Added by myself.;;;;;;;;;;;;;;;;;;;;
	
	//	if( !m_object.LoadGLTextures() )	// ������ͼ.
	//{
	//	MessageBox(0,"Erro!!������ͼʧ��!","Erro Caption!" , MB_OK);
	//}



	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	

	glLightfv(GL_LIGHT1,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT1,GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

// 	GLuint	fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
// 	GLfloat	fogColor[4] = {0.5f,0.5f,0.5f,1.0f};		// Fog Color
// 	glFogi(GL_FOG_MODE, fogMode[0]);			// Fog Mode
// 	glFogfv(GL_FOG_COLOR, fogColor);					// Set Fog Color
// 	glFogf(GL_FOG_DENSITY, 0.05f);						// How Dense Will The Fog Be
// 	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
// 	glFogf(GL_FOG_START, 1.0f);							// Fog Start Depth
// 	glFogf(GL_FOG_END, 5.0f);							// Fog End Depth
// 	glEnable(GL_FOG);									// Enables GL_FOG

	glColor4f(1.0f,1.0f,1.0f,0.5f);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE);

	//////////////////////////////////////////////////
	//�ڴ˴���ʼ����ͼ.������ʾ�б�.
	m_scene.LoadScene();
	///////////////////////////////////////////////////End
	
		
	return TRUE;
}
void COpenGL::Resize( )
{
	RECT rectClient;
   GetClientRect( m_hWnd , & rectClient );	//��õ�ǰ���ڵĿͻ�����С��
	int Width = rectClient.right - rectClient.left;
	int Height = rectClient.bottom - rectClient.top;
   glViewport(0,0, Width , Height );				// ����OpenGL�ӿڴ�С��	
	glMatrixMode(GL_PROJECTION);				// ���õ�ǰ����ΪͶӰ����
	glLoadIdentity();									// ���õ�ǰָ���ľ���Ϊ��λ����
	gluPerspective										// ����͸��ͼ
		( 54.0f,												// ͸�ӽ�����Ϊ 45 ��
		  (GLfloat)Width/(GLfloat)Height,			// ���ڵĿ���߱�
		  0.01f,												// ��Ұ͸�����:����1.0f
		  3000.0f											// ��Ұ͸�����:ʼ��0.1fԶ��3000.0f
		);
	//  �������������ƣ���һ���������þ�ͷ��Ƕȣ��ڶ��������ǳ���ȣ�������Զ�����С�
	glMatrixMode(GL_MODELVIEW);				// ���õ�ǰ����Ϊģ����ͼ����
	glLoadIdentity();									// ���õ�ǰָ���ľ���Ϊ��λ����

}
void COpenGL::Render()								// OpenGLͼ�δ���
{	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		   // ����ˢ�±���ɫ
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);// ˢ�±���



//	glLoadIdentity();								   // ���õ�ǰ��ģ�͹۲����
	Show();											     //��ʾͼ��.
//	glFlush();									          // ���´���
	SwapBuffers(m_hDC);						 // �л�������


}
void COpenGL::CleanUp()//���OpenGL
{
	 wglMakeCurrent(m_hDC, NULL);                       //���OpenGL
	 wglDeleteContext(m_hRC);                           //���OpenGL


}

void COpenGL::Show(void)
{
	for ( size_t i=0; i<m_showObjectList.size(); ++i )
	{
		m_showObjectList[i]->Show();
	}
	m_scene.DrawTheScene();			// ��������

	

	m_object.Box1();				// ��������...............
}

//void COpenGL::LightContrl(bool order)
//{
//	if(order)
//	{////////order =1 ,open the light.
//		glEnable(GL_LIGHTING);
//	}
//	else
//	{
//		glDisable(GL_LIGHTING);
//	}
//}
void COpenGL::ChangeFilter(void)
{
	m_object.filter++;
	if(m_object.filter > 2)
	{
		m_object.filter = 0;
	}
}


	// OpenGL�ĳ�ʼ�������豸��
BOOL COpenGL::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hDC = GetDC( hWnd );
	SetupPixelFormat( m_hDC );
	InitOpenGL();
	Resize(  );
	return 0;
}

//��ʼ��OpenGL�����á�
bool COpenGL::InitOpenGL(void)
{
	   
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);			
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		
	return true;
}

void COpenGL::AddShowObject( IShowObject* pShowObj )
{
	m_showObjectList.push_back( pShowObj );
}