// COpenGL.cpp: implementation of the COpenGL class.
//程序设计：李之兴

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
	// 初始化变量.
	LightAmbient[0] = LightAmbient[1] = LightAmbient[2] = LightAmbient[3] =  0.5f ;		//Ambient light values.
	LightDiffuse[0] = LightDiffuse[1] = LightDiffuse[2] = LightDiffuse[3] = 1.0f ;		//Diffuse light values.
	LightPosition[2] = 1.0f  ;		//Light position.
	LightPosition[0] = LightPosition[1] = LightPosition[3] = 0.0f;
}
COpenGL::~COpenGL()
{
	CleanUp();
}
BOOL COpenGL::SetupPixelFormat(HDC hDC)	  // 检测安装OpenGL
{	
	int nPixelFormat;					  // 象素点格式

	m_hDC=hDC;
	PIXELFORMATDESCRIPTOR pfd = 
	{ 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小 
	    1,                                // 版本号 
	    PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图 
	    PFD_SUPPORT_OPENGL |              // 支持 OpenGL 
	    PFD_DOUBLEBUFFER,                 // 双缓存模式 
	    PFD_TYPE_RGBA,                    // RGBA 颜色模式 
	    16,                               // 16 位颜色深度 
	    0, 0, 0, 0, 0, 0,                 // 忽略颜色位 
	    0,                                // 没有非透明度缓存 
	    0,                                // 忽略移位位 
	    0,                                // 无累加缓存 
	    0, 0, 0, 0,                       // 忽略累加位 
	    32,                               // 32 位深度缓存     
	    0,                                // 无模板缓存 
	    0,                                // 无辅助缓存 
	    PFD_MAIN_PLANE,                   // 主层 
	    0,                                // 保留 
	    0, 0, 0                           // 忽略层,可见性和损毁掩模 
	}; 

	if (!(nPixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
		{ 
			MessageBox(NULL,"没找到合适的显示模式","Error",MB_OK|MB_ICONEXCLAMATION);
			 return FALSE;
		}

	SetPixelFormat(m_hDC,nPixelFormat,&pfd);		//	设置当前设备的像素点格式
	m_hRC = wglCreateContext(m_hDC);				//	获取渲染描述句柄
	wglMakeCurrent(m_hDC, m_hRC);					//	激活渲染描述句柄

/////////////Added by myself.;;;;;;;;;;;;;;;;;;;;
	
	//	if( !m_object.LoadGLTextures() )	// 加载贴图.
	//{
	//	MessageBox(0,"Erro!!加载贴图失败!","Erro Caption!" , MB_OK);
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
	//在此处初始化地图.生成显示列表.
	m_scene.LoadScene();
	///////////////////////////////////////////////////End
	
		
	return TRUE;
}
void COpenGL::Resize( )
{
	RECT rectClient;
   GetClientRect( m_hWnd , & rectClient );	//获得当前窗口的客户区大小。
	int Width = rectClient.right - rectClient.left;
	int Height = rectClient.bottom - rectClient.top;
   glViewport(0,0, Width , Height );				// 设置OpenGL视口大小。	
	glMatrixMode(GL_PROJECTION);				// 设置当前矩阵为投影矩阵。
	glLoadIdentity();									// 重置当前指定的矩阵为单位矩阵
	gluPerspective										// 设置透视图
		( 54.0f,												// 透视角设置为 45 度
		  (GLfloat)Width/(GLfloat)Height,			// 窗口的宽与高比
		  0.01f,												// 视野透视深度:近点1.0f
		  3000.0f											// 视野透视深度:始点0.1f远点3000.0f
		);
	//  这和照象机很类似，第一个参数设置镜头广角度，第二个参数是长宽比，后面是远近剪切。
	glMatrixMode(GL_MODELVIEW);				// 设置当前矩阵为模型视图矩阵
	glLoadIdentity();									// 重置当前指定的矩阵为单位矩阵

}
void COpenGL::Render()								// OpenGL图形处理
{	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		   // 设置刷新背景色
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);// 刷新背景



//	glLoadIdentity();								   // 重置当前的模型观察矩阵
	Show();											     //显示图形.
//	glFlush();									          // 更新窗口
	SwapBuffers(m_hDC);						 // 切换缓冲区


}
void COpenGL::CleanUp()//清除OpenGL
{
	 wglMakeCurrent(m_hDC, NULL);                       //清除OpenGL
	 wglDeleteContext(m_hRC);                           //清除OpenGL


}

void COpenGL::Show(void)
{
	for ( size_t i=0; i<m_showObjectList.size(); ++i )
	{
		m_showObjectList[i]->Show();
	}
	m_scene.DrawTheScene();			// 画出场景

	

	m_object.Box1();				// 测试箱子...............
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


	// OpenGL的初始化。绑定设备。
BOOL COpenGL::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hDC = GetDC( hWnd );
	SetupPixelFormat( m_hDC );
	InitOpenGL();
	Resize(  );
	return 0;
}

//初始化OpenGL的设置。
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