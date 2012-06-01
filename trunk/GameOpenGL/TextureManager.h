#pragma once

#include "stdafx.h"

const int MAX_TEXTURE_AMOUNT = 100;

class TextureManager
{
public:
	TextureManager(void);
public:
	~TextureManager(void);

public:
	static TextureManager  *GetInstance();
	static void FreeInstance();

	// ��ʼ��.
	void Init();

	// ���ָ����������.
	GLuint GetTetureName( int textureID);

	// ʹ��ָ��ID������.
	bool UseTexture( int textureID );

private:
	static      TextureManager *m_instance;

	int iTextureAmount;						// ���������.
	GLuint textureName[MAX_TEXTURE_AMOUNT];	// �洢�������Ƶ�����.������±��������ID.


};

