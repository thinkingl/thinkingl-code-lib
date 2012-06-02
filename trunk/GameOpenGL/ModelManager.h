/************************************************
*	模型管理器
*
*************************************************/
#include "include\MD2.h"
#include <vector>

#ifndef MODEL_MANAGER_H_
#define MODEL_MANAGER_H_

//#define MAX_MODEL_NUM 20

/*
* !!!!只加载一个模型，设置一个模型的参数
*/
class ModelManager
{
	public:
		ModelManager();
		~ModelManager();
		int  Init();
		int  LoadModel(int modelID , char* fileName); //文件名不包含扩展名 
		void DeleteModel( int modelID=0 );
		void Show( int modelID = 0);
		int  SetAnimType( int modelID=0 ,animType_t type=STAND );
		static ModelManager* GetInstance();
		void Destory();
		void ScaleModel( int modelID = 0 , float fScale = 1.0f );
	
	protected:
		typedef std::vector< CMD2Model > CMD2ModelList;
		CMD2ModelList m_modelList;
// 		CMD2Model* 	   	m_ptrModels[MAX_MODEL_NUM]; //指向模型指针数组 
// 		int 			m_numOfModels;  //当前模型的个数
		static ModelManager*	m_singleton;  //指向模型管理器的指针
	
};

#endif
