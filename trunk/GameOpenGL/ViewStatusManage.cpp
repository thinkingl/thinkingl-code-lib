#include ".\viewstatusmanage.h"
#using <mscorlib.dll>
#include "role.h"

#include "ModelManager.h"
#include "include\iniset.h"

#define constSetupFile  "ini\\setup.ini"

Camera::Camera(void)
{
	//////////////进行视点的状态初始化,完成后,视点在0点,望向屏幕里(Z轴负方向),屏幕正上方与视点正上方重合.
	viewStatus.viewAt = Vector3f( 0.0f , HERO_HEIGHT , 0.0f );
	viewStatus.lookTo = Vector3f( 0.0f , 0.0f , -1.0f );
	viewStatus.upDirection = Vector3f( 0.0f ,1.0f ,0.0f );

	CIniSet iniSet( constSetupFile );
	
	viewModal = iniSet.ReadInt( "View" , "ViewMode");
	////////////初始化完成...................................................
//	viewModal = VIEW_FPS	;	//默认的观察模式为单人视角模式.

//	viewModal = VIEW_WOW;

	wowView.viewAngle = 0.0f ;
	wowView.viewDistance = 5.0f ;

}

Camera::~Camera(void)
{
}

void Camera::SetViewModal(int viewModal)
{

}

// 获得当前的视角模型参数.
VIEWSTATUS Camera::GetViewStatus(void)
{
	return viewStatus;
}


//
//// 更新角色位置.
//void Camera::SetRolePos(Vector3f rolePos)
//{
//	rolePosition = rolePos;
//}
//
//// 更新角色方向.
//void Camera::SetroleDirection( Vector3f vDirection )
//{
//	roleDirection = vDirection;
//}

// 更新摄像机位置和角度。
bool Camera::Update()
{
	CRole * pRole;
	pRole = CRole::GetInstance();
	switch ( viewModal )	// Check the view modal .
	{	
		
	case VIEW_FPS:	// 单人视角模式.圆柱形模型.
		{
			// 下面刷新视点的位置..
			
			viewStatus.viewAt = pRole->GetRolePos() + Vector3f(0.0f , HERO_HEIGHT ,0.0f );

			viewStatus.lookTo = pRole->GetRoleDirector();

			//	修正视点位置,防止被角色模型挡住.
			viewStatus.viewAt = viewStatus.viewAt + viewStatus.lookTo * 1.0f;
			glLoadIdentity();

			gluLookAt( viewStatus.viewAt.x, viewStatus.viewAt.y, viewStatus.viewAt.z,
				viewStatus.lookTo.x + viewStatus.viewAt.x , viewStatus.lookTo.y + viewStatus.viewAt.y, viewStatus.lookTo.z + viewStatus.viewAt.z,
				viewStatus.upDirection.x , viewStatus.upDirection.y, viewStatus.upDirection.z );
			break;
		}
	case VIEW_WOW:	// 魔兽世界模式。视点保持在角色后方。视线一直在角色身上.
		// 刷新视点。	

		viewStatus.viewAt = pRole->GetRolePos() - Normalize( pRole->GetRoleDirector() )* wowView.viewDistance;
		viewStatus.lookTo = pRole->GetRolePos() + Vector3f( 0.0f , HERO_HEIGHT , 0.0f );

		//	修正视点高度,使适合角色高度
		viewStatus.viewAt = viewStatus.viewAt + Vector3f( 0.0f , HERO_HEIGHT , 0.0f );
		glLoadIdentity();

		gluLookAt( viewStatus.viewAt.x, viewStatus.viewAt.y, viewStatus.viewAt.z,
			viewStatus.lookTo.x  , viewStatus.lookTo.y , viewStatus.lookTo.z ,
			viewStatus.upDirection.x , viewStatus.upDirection.y, viewStatus.upDirection.z );
		break;
	}
	

	return 0;
}
