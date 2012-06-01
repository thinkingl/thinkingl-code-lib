/// OpenGL�ĳ�����.cpp : Defines the entry point for the application.
//������ƣ���֮��

#include "stdafx.h"
#include "GLShow.h"
#include "WorldModel.h"
#include "ViewStatusManage.h"
#include "OpenGLWindow.h"
#include "Control.h"
#include "Role.h"

#include "map.h"
#include "TextureManager.h"

//////////////////////////////////////////////////////////
#define IDT_TIMERDRAW  1			//��ʱ�����1.Ϊ��Ļˢ��ʱ�䶨ʱ.
#define TIMETODRAW   30				//��ʱ��ʱ��,����Ļˢ�µļ��ʱ��..
#define TIMER_TARGET_RESOLUTION 1	//��ý�嶨ʱ���ľ���.���ϵͳ����С���ȱ�����,����ϵͳ��С����.
#define TIMER_INPUT		15			//��ý�嶨ʱ���ļ��.Ҳ���Ƕ�ȡ�����豸����ļ��ʱ��.
#define WM_MUTIMER1	WM_USER+1000	//�Զ���Ķ�ý�嶨ʱ������Ϣ���,��ϵͳ�����.


// ����ID***************************


//**********************************

UINT TIMER_ID;				//  ��ý�嶨ʱ���ı�ʶ.
Control gameControl;		//  ����һ����Ϸ���ƶ���,�����û�������Ϊ��Ϸ��������.
OpenGL  OpenGL;				//  ����һ��OpenGL��ʾ�Ķ���,��������ʾ.
WorldModel  worldModel;		//  ����һ������ģ�͵Ķ���.
Camera camera ;				//  ���������(��Ļ��ʾ)�����ָ��.
OpenGLWindow openGLWindow ;	// �����ࡣ
CRole * pRole;					//  ��Ϸ��ɫ��


/////////////////////////////////////////////////////////////////////////////


HWND	hWnd=NULL;	// ���� Windows ���������Ĵ��ھ��


bool	bExiting = FALSE;	//��ʶ�����Ƿ������˳�����.
bool	bActive = TRUE;	//��ʶ���ڴ����Ƿ��Ǽ���״̬.


// һЩ����������.
void ExitGameLoop(void);// �˳���������.
void WINAPI TimerProc (UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);// ��ý�嶨ʱ���ص���������.
void CarryOut( int * cmdQueue );	// ִ���û�������.

// ��Ϸ����ѭ������,�����˳������Ϣѭ��.

void GameLoop()
{ 
	static MSG msg; 
	while(GetMessage(&msg, NULL, NULL , NULL))
	{
			TranslateMessage(&msg);				// Translate The Message
			DispatchMessage(&msg);				// Dispatch The Message
    }

}

////�ص�����,��Ϣ�Ĵ�������������.
LRESULT WINAPI MsgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )// ��Ϣ����
{	
	switch(message)
	{	
		case WM_CLOSE:						// �رմ���
			OpenGL.CleanUp();			// ��������
			PostQuitMessage(0);
			return 0;		break;
		case WM_SIZE:						// ���ڳߴ�仯
			OpenGL.Resize();
			return 0;		break;
		case WM_DESTROY:					// �˳���Ϣ
            PostQuitMessage(0);
            return 0;		break;


		case WM_TIMER:	//ϵͳ��ʱ����Ϣ,��Ҫ����Ϊ��Ļ�ػ���ʱ.
			{
				camera.Update();


				OpenGL.Render();		//������ʱ����Ϣ���ػ���Ļ.
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

//  Main����,����ĳ����.
//  INT WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,INT )// WinMain�������
int PASCAL WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,  LPSTR lpCmdLine, int nCmdShow)
{  

	
	// �������ڲ����г�ʼ����
	hWnd = openGLWindow.IniWindow(hInst,nCmdShow,MsgProc);
	
	OpenGL.Init(hWnd);		// ��ʼ��OpenGL��

	// ��ʼ�����������.
	TextureManager::GetInstance();
	
	// ��Ϸ������ƶ����ʼ��.
	gameControl.Initialize( hWnd );

	// ��ʼ����Ϸ��ɫ��
	pRole = CRole::GetInstance();
	// ��ʼ����Ϸ
	worldModel.Init();

	// ��ʼ����ͼ.
//	map.Init();



	//  ϵͳ��ʱ��,Ϊ��Ļˢ�¶�ʱ.
	SetTimer( hWnd ,
		IDT_TIMERDRAW,
		TIMETODRAW ,
		(TIMERPROC) NULL);
	//  ��ȷ�Ķ�ý�嶨ʱ��.������Ҫ��ȷ��ʱ�Ŀ��������.
	//  ����ȷ������,��ϵͳ����ĺ�TIMER_TARGET_RESOLUTION�д���Ǹ�.
			TIMECAPS tc;
			UINT wTimerRes;			//  ��ý�嶨ʱ�����ȱ�ʶ.
			if(timeGetDevCaps( &tc , sizeof(TIMECAPS)) != TIMERR_NOERROR)
			{	
			//  ���ϵͳ�������С�����Ķ�ʱ������.
				MessageBox( NULL ," ϵͳ��ʱ�����Ȼ��ʧ��!","Erro!",NULL);
				KillTimer ( hWnd , IDT_TIMERDRAW);
				return 0;
			}
		   //  ȡ����Ϊϵͳ������С���Ⱥ�ָ�������д���Ǹ�.
			
			wTimerRes =  min ( max ( tc.wPeriodMin ,TIMER_TARGET_RESOLUTION ) ,tc.wPeriodMax);
			timeBeginPeriod( wTimerRes );
			//  ��ʱ����ʼ����.
			TIMER_ID = timeSetEvent( TIMER_INPUT , wTimerRes , (LPTIMECALLBACK) TimerProc , (DWORD)hWnd , TIME_PERIODIC );

	////////////////////////////////////////////

	GameLoop();	 //  ������Ϣѭ��

	// �˳����򣬽����ڴ�����
	OpenGL.CleanUp();	// ��������
	KillTimer( hWnd,IDT_TIMERDRAW);//  ɾ��ϵͳ��ʱ��.
	timeEndPeriod( wTimerRes );			//  ɾ����ý�嶨ʱ���ľ���.
	timeKillEvent( TIMER_ID );
	return 0;
}

////////////��ý�嶨ʱ���Ļص�����.�����ﴦ�������ɨ��.
void WINAPI	TimerProc (UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	if(bActive)
		{
			gameControl.Update();
			CarryOut( gameControl.GetOrderList());	// ִ���û����������.
//			worldModel.UpdateRoleDirector( gameControl.GetMouseRelMoveX() , gameControl.GetMouseRelMoveY() );
			pRole->UpdateDirection( gameControl.GetMouseRelMoveX() , gameControl.GetMouseRelMoveY () );
		}
}
///�˳���Ϸ����Ϣѭ��.
void ExitGameLoop(void)
{	
	if ( !bExiting )	// �Ƴ���Ϸʱ,�����Ի���.
	{
		::ShowCursor( TRUE );	// �������ʾ����.
		bExiting = TRUE;
		if( MessageBox ( hWnd, "ȷ��Ҫ�˳���?" , " Yes or No " , MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL) == IDYES)
		{///////////////////////����һ���Ի���,���û�ȷ��֮�����˳�����.

			PostMessage( hWnd , WM_QUIT,0,0 );

		}
		else
		{
			bExiting = FALSE;
			::ShowCursor( FALSE );	// �������ʾ����.
		}
	}
}


//  ִ���û�����.
void CarryOut( int * CmdQueue )
{
	for ( ;(*CmdQueue) > 0;(*CmdQueue) -- )
	{
		switch ( *(CmdQueue + *CmdQueue) )
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
		}
		
	}
}