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

	// 初始化.
	void Init();

	// 获得指定纹理名称.
	GLuint GetTetureName( int textureID);

	// 使用指定ID的纹理.
	bool UseTexture( int textureID );

private:
	static      TextureManager *m_instance;

	int iTextureAmount;						// 纹理的数量.
	GLuint textureName[MAX_TEXTURE_AMOUNT];	// 存储纹理名称的数组.数组的下标是纹理的ID.


};

