///CWorldModel.h//////////////////
///WorldModel���ͷ�ļ�./////
///��֮��///
////�����Ŀ����ʵ��һ������ĳ���ģ��.
//����:��ǰ�������صĴ���.
//�Ҿ�Ȼ���������ʾ�б�������װ������....�� ��.............
//OK,�����������Ϊһ�������Ĵ���,ֻ�ܵ�����ʾģ��.����ʾģ�������ʾʲô����,���͵���ʾʲô����,��Ȼ����ʾ��Ҳ����.
//��,�����������ά��һ��Ҫ��ʾ�Ķ��������ݽṹ.һ���б�.��Ȼ,��ֻ��������ʾģ��ͨ�ŵĽӿڲ���.
//�Ժ�����ʹ��ȫ�ֵı���,�ṹ��.�������б��Ҳ���ȫ��....
//�����кö������Ŷ.��Ϊ���е�"ʵ�õ�"��Ϣ������ά��.�Ժ��п������.
///@#$!$!!%$^$%&$%&^%&*&*(*()()(^*$%^@~#%~@~$@~~$#%$%@#~#$#^&$@@$/////////////////
////�������뵽ȴ������˵�����:
///��������Ļ�ȡ,����.����:����,�������������,��,�����״̬,��Ʒ��״̬,�����״̬��.��������?
//������Ϸ,��ȻҪ��һ������ӿ���������.....

//һЩ���õ�ͷ�ļ�,��ʲô�Ķ������������.
#include "MyMathArithmetic.h"
#include "VisualServerTemp.h"

#pragma once

#include "MymathArithmetic.h"
#include "role.h"

//// ����Ĳ����ṹ
//struct RoleMoveStatus{
//	/*********��ɫ�ƶ��õ��Ľṹ��**********/
//	float fSpeed;		// ��ɫ���ƶ��ٶȡ�
//	Vector3f vPos;		// ��ɫ����ʾʱ�ľ������ꡣ
//	Vector3f vRoleDirector; // ��ɫ�ķ���.
//	Vector3f vMoveDirector;// ��ɫ�ƶ��ķ���.
//	Vector3f vUp;
//
//};



class CWorldModel
{
public:
	CWorldModel(void);
	~CWorldModel(void);


private:

//	RoleMoveStatus roleMoveStatus;
//	VisualServerTemp * m_visualServer;

	//������vRot����v��תangle�Ƕ� 
	bool RotateV( Vector3f &vRot, Vector3f v,float angle);

	// ��ɫ
	CRole m_mainRole;

	// Ⱥ����Ա.
	typedef std::vector< CRole > CRoleList;
	CRoleList m_roleList;

public:
	// ��ʾ�������е�����. ��ʱ��������, �������ﲢ������.
	CRolePointList GetAllRole();

	CRole * GetMainRole();

public:

	// ��ʼ��
	bool Init();

	bool UpdateViewDirection( int xMoved , int yMoved );

	// ��ǰ��һ��.
	void Forward(void);
	// �����һ��.
	void Back(void);
	// ����һ��.
	void Left(void);
	// ����һ��.
	void Right(void);

	// ��.
	void Jump();

	// ����
	void Fire();

	// �������������ʲôС����.
	void RoleDosomeAction();

	// �������ڽ�ɫ�ķ���.
	//void UpdateRoleDirector(int xMoved , int yMoved);

	// ���½�ɫ״̬.
	//bool UpdateRoleState();


};