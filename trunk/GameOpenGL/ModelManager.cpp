#include "ModelManager.h"
#include "stdafx.h"

ModelManager* ModelManager::m_singleton = 0;

ModelManager* ModelManager::GetInstance()
{
    if( !m_singleton )
        m_singleton = new ModelManager();
    return m_singleton;
}

ModelManager::ModelManager()
{
	m_numOfModels = 0;
	for( int i=0; i<MAX_MODEL_NUM ; i++)
	{
		m_ptrModels[i] = new CMD2Model(); 
	}

	//////////////////////////////////////////////////////////////////////
	Init();
	/******************************************************************/////
}

ModelManager::~ModelManager()
{
	for( int i=0; i<MAX_MODEL_NUM ; i++)
	{
		if(m_ptrModels[i] != 0) 
		 	delete m_ptrModels[i];
		 	m_ptrModels[i] = 0;
	}
}

int ModelManager::Init()
{
	LoadModel( 0, "tris");
    SetAnimType( 0 , STAND );
	ScaleModel( 0 , 0.050f );
	
	LoadModel(1 , "weapon");
    SetAnimType( 1 , STAND );
	ScaleModel( 1 , 0.050f );
	
	m_numOfModels = 2;
	return 0;	
}

int  ModelManager::LoadModel(int ModelID, char* fileName)
{
    if(!fileName) return -1;
    char tempStr[255];
    strcpy( tempStr, "data\\MD2\\" );
    strcat( tempStr, fileName);
    strcat( tempStr, ".MD2");
    
	if( !m_ptrModels[ModelID]->LoadModel(tempStr))
        return -1;
    
    strcpy( tempStr, "data\\MD2\\" );
    strcat( tempStr, fileName);
    strcat( tempStr, ".pcx");
    m_ptrModels[ModelID]->LoadSkin( tempStr );
    return 0;
}

void ModelManager::DeleteModel( int modelID )
{
	
}

int  ModelManager::SetAnimType( int modelID ,animType_t type )
{
	m_ptrModels[modelID]->SetAnim( type );
	return 0;
}

void ModelManager::Show( int modelID )
{
	float curTimeSec = GetTickCount() / 1000.0f;
	for( int i = 0; i < m_numOfModels ; i++ )
	{
		m_ptrModels[i]->DrawModel( curTimeSec );
	}
}

void ModelManager::Destory()
{
    for( int i = 0; i < m_numOfModels ; i++ )
    {
	 	if( m_ptrModels[i] )
        	delete m_ptrModels[i];
        
        m_ptrModels[i] = 0;
	}

     m_numOfModels = 0;
     
     if (m_singleton)
     	delete m_singleton;
}

void ModelManager::ScaleModel( int modelID , float fScale )
{
     m_ptrModels[modelID]->ScaleModel( fScale );
}
