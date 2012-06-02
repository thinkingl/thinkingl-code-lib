/// OpenGL的程序框架.cpp : Defines the entry point for the application.
//程序设计：李之兴

#include "stdafx.h"
#include "GLShow.h"
#include "WorldModel.h"
#include "ViewStatusManage.h"
#include "OpenGLWindow.h"
#include "Control.h"
#include "Role.h"

#include "map.h"
#include "TextureManager.h"
#include "ScriptParse.h"

//////////////////////////////////////////////////////////
#define IDT_TIMERDRAW  1			//定时器序号1.为屏幕刷新时间定时.
#define TIMETODRAW   30				//定时器时间,即屏幕刷新的间隔时间..
#define TIMER_TARGET_RESOLUTION 1	//多媒体定时器的精度.如果系统的最小精度比他大,则用系统最小精度.
#define TIMER_INPUT		15			//多媒体定时器的间隔.也就是读取输入设备输入的间隔时间.
#define WM_MUTIMER1	WM_USER+1000	//自定义的多媒体定时器的消息标号,管系统输入的.


// 命令ID***************************


//**********************************

UINT TIMER_ID;				//  多媒体定时器的标识.
Control gameControl;		//  定义一个游戏控制对象,翻译用户的输入为游戏控制命令.
COpenGL  openGL;				//  定义一个OpenGL显示的对象,功能是显示.
CWorldModel  worldModel;		//  定义一个世界模型的对象.
Camera * g_pCamera = 0;				//  控制摄像机(屏幕显示)的类的指针.
OpenGLWindow openGLWindow ;	// 窗口类。
//CRole * pRole;					//  游戏角色。



/////////////////////////////////////////////////////////////////////////////


HWND	hWnd=NULL;	// 保存 Windows 分配给程序的窗口句柄


bool	bExiting = FALSE;	//标识现在是否正在退出程序.
bool	bActive = TRUE;	//标识现在窗口是否是激活状态.

CScriptParse g_scriptParser;
bool g_bRunScript = false;		// 是否执行脚本.

// 一些函数的声明.
void ExitGameLoop(void);// 退出函数声明.
void WINAPI TimerProc (UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);// 多媒体定时器回调函数声明.
void CarryOut( const COrderList& );	// 执行用户的命令.
void RunScript( const COrderList& orderList );

// 游戏的主循环函数,包含了程序的消息循环.

void GameLoop()
{ 
	static MSG msg; 
	while(GetMessage(&msg, NULL, NULL , NULL))
	{
			TranslateMessage(&msg);				// Translate The Message
			DispatchMessage(&msg);				// Dispatch The Message
    }

}

////回调函数,消息的处理都集中在这里.
LRESULT WINAPI MsgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )// 消息处理
{	
	switch(message)
	{	
		case WM_CLOSE:						// 关闭窗口
			openGL.CleanUp();			// 结束处理
			PostQuitMessage(0);
			return 0;		break;
		case WM_SIZE:						// 窗口尺寸变化
			openGL.Resize();
			return 0;		break;
		case WM_DESTROY:					// 退出消息
            PostQuitMessage(0);
            return 0;		break;


		case WM_TIMER:	//系统定时器消息,主要用来为屏幕重画定时.
			{
				g_pCamera->Update();


				openGL.Render();		//遇到定时器消息则重画屏幕.
			}
			return 0;		break;
			
		case WM_ACTIVATE:
			
				if( wParam == WA_ACTIVE)
					bActive = true;
				if( wParam == WA_INACTIVE)
					bActive = false;
			return 0;		break;
	
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}

//  Main函数,程序的出入口.
//  INT WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,INT )// WinMain程序入口
int PASCAL WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,  LPSTR lpCmdLine, int nCmdShow)
{  

	
	// 创建窗口并进行初始化。
	hWnd = openGLWindow.IniWindow(hInst,nCmdShow,MsgProc);
	
	openGL.Init(hWnd);		// 初始化OpenGL。

	// 初始化纹理管理器.
	TextureManager::GetInstance();
	
	// 游戏输入控制对象初始化.
	gameControl.Initialize( hWnd );

	// 初始化游戏
	worldModel.Init();

	CRolePointList allRole = worldModel.GetAllRole();
	for ( size_t i=0; i<allRole.size(); ++i )
	{
		openGL.AddShowObject( allRole[i] );
	}

	g_pCamera = new Camera( worldModel.GetMainRole() );
	
	// 初始化地图.
//	map.Init();



	//  系统定时器,为屏幕刷新定时.
	SetTimer( hWnd ,
		IDT_TIMERDRAW,
		TIMETODRAW ,
		(TIMERPROC) NULL);
	//  精确的多媒体定时器.用于需要精确定时的控制输入等.
	//  首先确定精度,用系统允许的和TIMER_TARGET_RESOLUTION中大的那个.
			TIMECAPS tc;
			UINT wTimerRes;			//  多媒体定时器精度标识.
			if(timeGetDevCaps( &tc , sizeof(TIMECAPS)) != TIMERR_NOERROR)
			{	
			//  获得系统允许的最小和最大的定时器精度.
				MessageBox( NULL ," 系统定时器精度获得失败!","Erro!",NULL);
				KillTimer ( hWnd , IDT_TIMERDRAW);
				return 0;
			}
		   //  取精度为系统允许最小精度和指定精度中大的那个.
			
			wTimerRes =  min ( max ( tc.wPeriodMin ,TIMER_TARGET_RESOLUTION ) ,tc.wPeriodMax);
			timeBeginPeriod( wTimerRes );
			//  定时器开始工作.
			TIMER_ID = timeSetEvent( TIMER_INPUT , wTimerRes , (LPTIMECALLBACK) TimerProc , (DWORD)hWnd , TIME_PERIODIC );

	////////////////////////////////////////////

	GameLoop();	 //  进入消息循环

	// 退出程序，进行内存清理。
	openGL.CleanUp();	// 结束处理
	KillTimer( hWnd,IDT_TIMERDRAW);//  删除系统定时器.
	timeEndPeriod( wTimerRes );			//  删除多媒体定时器的精度.
	timeKillEvent( TIMER_ID );
	return 0;
}

////////////多媒体定时器的回调函数.在这里处理输入的扫描.
void WINAPI	TimerProc (UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	if(bActive)
		{
			gameControl.Update();

			if( g_bRunScript )
			{				
				COrderList ol = g_scriptParser.GetOrderList( GetTickCount() );
				RunScript( ol );
			}
			else
			{
				COrderList orList;
				int * CmdQueue = gameControl.GetOrderList();
				int cmdNum = CmdQueue[0];
				for ( int i=0; i<cmdNum; ++i )
				{
					COrder or;
					or.m_cmd = (EOrderCmd)CmdQueue[ i+1 ];

					orList.push_back( or );
				}
				CarryOut( orList );	// 执行用户输入的命令.
			}
			


//			worldModel.UpdateRoleDirector( gameControl.GetMouseRelMoveX() , gameControl.GetMouseRelMoveY() );

			worldModel.UpdateViewDirection( gameControl.GetMouseRelMoveX() , gameControl.GetMouseRelMoveY () );
		}
}
///退出游戏的消息循环.
void ExitGameLoop(void)
{	
	if ( !bExiting )	// 推出游戏时,弹出对话框.
	{
		::ShowCursor( TRUE );	// 将光标显示出来.
		bExiting = TRUE;
		if( MessageBox ( hWnd, "确定要退出吗?" , " Yes or No " , MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL) == IDYES)
		{///////////////////////弹出一个对话框,让用户确定之后再退出程序.

			PostMessage( hWnd , WM_QUIT,0,0 );

		}
		else
		{
			bExiting = FALSE;
			::ShowCursor( FALSE );	// 将光标显示出来.
		}
	}
}


//  执行用户命令.
void CarryOut( const COrderList& orderList )
{
	for ( size_t i=0; i<orderList.size(); ++i )
	{
		switch ( orderList[i].m_cmd )
		{
			case CMD_EXIT:
				ExitGameLoop();
				break;
			case CMD_FORWARD_STEP:
				worldModel.Forward();
				break;
			case CMD_BACK_STEP:
				worldModel.Back();
				break;
			case CMD_LEFT_STEP:
				worldModel.Left();
				break;
			case CMD_RIGHT_STEP:	
				worldModel.Right();
				break;
			case CMD_CREATE_MAP:
				Map::GetInstance()->CreateNewMap( 1, -10 , -0 ,-10,124,100);
				Map::GetInstance()->SaveMapToFile( 1,"map\\default.map" );
				break;
			case CMD_DO_SOME_ACTION:
				worldModel.RoleDosomeAction();
				break;
			case CMD_JUMP:
				worldModel.Jump();
				break;
			case CMD_FIRE:
				worldModel.Fire();
				break;

			case CMD_RunScript:
				g_bRunScript = !g_bRunScript;
				if( !g_bRunScript )
				{
					g_scriptParser.Reset();
				}
				break;

			case CMD_MainSalut:
				worldModel.MainSalut( orderList[i].m_time );
				break;
			case CMD_OtherSalut:
				worldModel.OtherSalut( orderList[i].m_time );
				break;
			case CMD_TurnLeft:
				worldModel.UpdateViewDirection( -100, 0 );
				break;
			case CMD_TurnRight:
				worldModel.UpdateViewDirection( 100, 0 );
				break;
			case CMD_TurnUp:
				worldModel.UpdateViewDirection( 0, -100 );
				break;
			case CMD_TurnDown:
				worldModel.UpdateViewDirection( 0, 100 );
				break;
			case CMD_OtherDie:
				worldModel.OtherDie( atoi( orderList[i].m_param.c_str() ) );
				break;

		}
		
	}
}

// 运行脚本.
void RunScript( const COrderList& orderList )
{
	for ( size_t i=0; i<orderList.size(); ++i )
	{
		const COrder& order = orderList[i];

		CarryOut( orderList );
	}
}