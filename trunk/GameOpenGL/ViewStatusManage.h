///��֮��
//2005.12.24ʥ��ǰҹ.
//������Ļ��ʾ.�۲��ߵķ�λ.
#include "MyMathArithmetic.h"



#define	VIEW_FPS  0	// һ��Բ���ε�ת��ģ��..��һ���˵��ӽǿ�����.���ϵķ���һֱ��Y��������.��ͷ�͵�ͷ�����ᳬ��90��
						// ��Ұ���������ϵķ���ת��,Ҳ������Զ����Y��ת.ת���ĽǶȺ���껬���ľ��������.
#define VIEW_WOW	1	// ������ħ��������ӵ㷽ʽ���ӵ㱣���ڽ�ɫ�ĺ��Ϸ���

#define VIEW_GOD	2	// ��Ϸ�����ϿյĹ۲�ģʽ.Ĭ�ϵľ��ǿ���45��,Ӧ�ÿ��Ը��ĵ��й۲�ľ���ͽǶ�.��ʱûŪ����.

#define VIEW_FPS_SPEED	0.001		// ��Բ��ת��ģ��������ƶ����Ƶ��ӽ�ת�����ٶ�.
#define VIEW_FPS_RADII	100			// �ӽ���תģ��Բ���İ뾶.
#define VIEW_FPS_MAX_RAD	1.5/VIEW_FPS_SPEED			// �ӽ���תģ����ֱ������ӽǵ����Ƕ�,���ܳ���90��.
#define HERO_HEIGHT	2.3f	// ����Ӣ�۵ĸ߶�,Ҳ�����ӵ�ĸ߶�.

#pragma once

struct ViewWow		// ħ������ʽ�Ĺ۲�ģʽʱ,�洢����ͽǶ�
{
	float viewDistance;		// �ӵ�����ɫ�ľ���
	float viewAngle;		// �ӵ�ͽ�ɫ֮�����ߺ͵�ƽ��֮��ĽǶ�.
};

class Camera
{
public:
	Camera(void);
	~Camera(void);

private:

	// �ӵ��״̬,��������Ļ�۲�ķ�λ.
	VIEWSTATUS	viewStatus;
	// ��Ļ��ʾ����ģʽ:
	int		viewModal;
	// ��ŵ�ǰӢ�۵�λ��.
	Vector3f rolePosition;
	// ��ŵ�ǰ��ɫ�ķ���.
	Vector3f roleDirection;

	ViewWow wowView;	
	//������vRot����v��תangle�Ƕ� 
	//bool RotateV( Vector3f &vRot, Vector3f v,float angle);

public:
	// �ı���ʾ�ӽ�ģ��ģʽ
	void SetViewModal(int viewModal);
	// ��õ�ǰ���ӽ�ģ�Ͳ���.
	VIEWSTATUS GetViewStatus(void);

	// ʵ�ֵ�ǰ���ӵ�λ�ú��ӽǡ�
	bool Update();

	// �������ڹۿ����ӽ�.
	//void SetCamera(int xMoved , int yMoved);
};
