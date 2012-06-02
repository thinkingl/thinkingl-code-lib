/************************************************
*	ģ�͹�����
*
*************************************************/
#include "include\MD2.h"
#include <vector>

#ifndef MODEL_MANAGER_H_
#define MODEL_MANAGER_H_

//#define MAX_MODEL_NUM 20

/*
* !!!!ֻ����һ��ģ�ͣ�����һ��ģ�͵Ĳ���
*/
class ModelManager
{
	public:
		ModelManager();
		~ModelManager();
		int  Init();
		int  LoadModel(int modelID , char* fileName); //�ļ�����������չ�� 
		void DeleteModel( int modelID=0 );
		void Show( int modelID = 0);
		int  SetAnimType( int modelID=0 ,animType_t type=STAND );
		static ModelManager* GetInstance();
		void Destory();
		void ScaleModel( int modelID = 0 , float fScale = 1.0f );
	
	protected:
		typedef std::vector< CMD2Model > CMD2ModelList;
		CMD2ModelList m_modelList;
// 		CMD2Model* 	   	m_ptrModels[MAX_MODEL_NUM]; //ָ��ģ��ָ������ 
// 		int 			m_numOfModels;  //��ǰģ�͵ĸ���
		static ModelManager*	m_singleton;  //ָ��ģ�͹�������ָ��
	
};

#endif
