#include ".\viewstatusmanage.h"
#using <mscorlib.dll>
#include "role.h"

#include "ModelManager.h"
#include "include\iniset.h"

#define constSetupFile  "ini\\setup.ini"

Camera::Camera(void)
{
	//////////////�����ӵ��״̬��ʼ��,��ɺ�,�ӵ���0��,������Ļ��(Z�Ḻ����),��Ļ���Ϸ����ӵ����Ϸ��غ�.
	viewStatus.viewAt = Vector3f( 0.0f , HERO_HEIGHT , 0.0f );
	viewStatus.lookTo = Vector3f( 0.0f , 0.0f , -1.0f );
	viewStatus.upDirection = Vector3f( 0.0f ,1.0f ,0.0f );

	CIniSet iniSet( constSetupFile );
	
	viewModal = iniSet.ReadInt( "View" , "ViewMode");
	////////////��ʼ�����...................................................
//	viewModal = VIEW_FPS	;	//Ĭ�ϵĹ۲�ģʽΪ�����ӽ�ģʽ.

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

// ��õ�ǰ���ӽ�ģ�Ͳ���.
VIEWSTATUS Camera::GetViewStatus(void)
{
	return viewStatus;
}


//
//// ���½�ɫλ��.
//void Camera::SetRolePos(Vector3f rolePos)
//{
//	rolePosition = rolePos;
//}
//
//// ���½�ɫ����.
//void Camera::SetroleDirection( Vector3f vDirection )
//{
//	roleDirection = vDirection;
//}

// ���������λ�úͽǶȡ�
bool Camera::Update()
{
	CRole * pRole;
	pRole = CRole::GetInstance();
	switch ( viewModal )	// Check the view modal .
	{	
		
	case VIEW_FPS:	// �����ӽ�ģʽ.Բ����ģ��.
		{
			// ����ˢ���ӵ��λ��..
			
			viewStatus.viewAt = pRole->GetRolePos() + Vector3f(0.0f , HERO_HEIGHT ,0.0f );

			viewStatus.lookTo = pRole->GetRoleDirector();

			//	�����ӵ�λ��,��ֹ����ɫģ�͵�ס.
			viewStatus.viewAt = viewStatus.viewAt + viewStatus.lookTo * 1.0f;
			glLoadIdentity();

			gluLookAt( viewStatus.viewAt.x, viewStatus.viewAt.y, viewStatus.viewAt.z,
				viewStatus.lookTo.x + viewStatus.viewAt.x , viewStatus.lookTo.y + viewStatus.viewAt.y, viewStatus.lookTo.z + viewStatus.viewAt.z,
				viewStatus.upDirection.x , viewStatus.upDirection.y, viewStatus.upDirection.z );
			break;
		}
	case VIEW_WOW:	// ħ������ģʽ���ӵ㱣���ڽ�ɫ�󷽡�����һֱ�ڽ�ɫ����.
		// ˢ���ӵ㡣	

		viewStatus.viewAt = pRole->GetRolePos() - Normalize( pRole->GetRoleDirector() )* wowView.viewDistance;
		viewStatus.lookTo = pRole->GetRolePos() + Vector3f( 0.0f , HERO_HEIGHT , 0.0f );

		//	�����ӵ�߶�,ʹ�ʺϽ�ɫ�߶�
		viewStatus.viewAt = viewStatus.viewAt + Vector3f( 0.0f , HERO_HEIGHT , 0.0f );
		glLoadIdentity();

		gluLookAt( viewStatus.viewAt.x, viewStatus.viewAt.y, viewStatus.viewAt.z,
			viewStatus.lookTo.x  , viewStatus.lookTo.y , viewStatus.lookTo.z ,
			viewStatus.upDirection.x , viewStatus.upDirection.y, viewStatus.upDirection.z );
		break;
	}
	

	return 0;
}
