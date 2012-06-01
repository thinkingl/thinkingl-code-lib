// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#include <windows.h>	// Windows的头文件

#include <vector>	//向量的STL模板头文件.

#include <mmsystem.h>
//#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <gl\gl.h>		// OpenGL32库的头文件
#include <gl\glu.h>		// GLu32库的头文件
//#include <gl\glaux.h>	// GLaux库的头文件
#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")	// OpenGL32连接库
#pragma comment( lib, "glu32.lib")		// GLu32连接库
//#pragma comment( lib, "glaux.lib")		// GLaux连接库
#pragma comment( lib, "Winmm.lib")		//多媒体库.
#pragma comment(lib,"dxguid.lib")		//DirectX Audio lib
#pragma comment(lib,"dinput8.lib")		//DirectX Input lib

///// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


