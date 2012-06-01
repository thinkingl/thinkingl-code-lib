#include "TextureManager.h"
#include "include\iniset.h"

#include "include\bmp.h"
#include "include\pcx.h"

#define TEXTURE_FILE "ini\\texture.ini"
#define NAME "File"
const int DEFAULT_TEXTURE = 0;

TextureManager *TextureManager::m_instance = 0;

TextureManager::TextureManager(void)
{
	Init();
}

TextureManager::~TextureManager(void)
{
}

TextureManager *TextureManager::GetInstance()
{
	if(m_instance == 0)
        m_instance = new TextureManager;
    return m_instance;
}

void TextureManager::FreeInstance()
{
    if(m_instance != 0)
		delete m_instance;
    m_instance = 0;
}

// 初始化.
void TextureManager::Init()
{
	iTextureAmount = 0;
	// 读取纹理配置文件.
	CIniSet iniTextureFile = CIniSet( TEXTURE_FILE );

	// 获得纹理和纹理文件列表
	char index[20] ;
	char *fileName ;

	sprintf_s( index ,20,"Texture%d",iTextureAmount);
	fileName = iniTextureFile.ReadText( index , NAME);
	while( fileName )
	{
		// 读取图片文件.
		unsigned char	*texels = 0;
		int success = 0;
		int width, height;

		success = LoadFileBMP( fileName , &texels , &width , &height , false);

		if( success == 0 )
			success = LoadFilePCX( fileName , &texels , &width , &height , false );

		if(success == 1)
		{
			glGenTextures(1, &textureName[iTextureAmount]);
			glBindTexture( GL_TEXTURE_2D, textureName[iTextureAmount]);



			gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texels );

			// 纹理数量加1.
			iTextureAmount ++;
	    
		}
		else
		{
			// can't load the texture
			MessageBox( 0 , "Load Texture Erro!" , "Erro!" , MB_OK );
		}

		// 清理内存.
		if(texels)
			delete texels;
		texels = 0;
		if( fileName )
			delete fileName;

		// 读取新的纹理贴图文件名.
		sprintf_s( index ,20,"Texture%d",iTextureAmount);
		fileName = iniTextureFile.ReadText( index , NAME);
		
	}

}

// 获得纹理的名称.
GLuint TextureManager::GetTetureName(int textureID)
{
	if( textureID < iTextureAmount && textureID > 0)
		return textureName[ textureID ];
	else
		return textureName[DEFAULT_TEXTURE];		// 如果申请不存在的纹理,返回第0号.
}

// 使用指定纹理
bool TextureManager::UseTexture(int textureID)
{
	if( textureID < iTextureAmount && textureID > 0)
	{
		glBindTexture( GL_TEXTURE_2D , textureName[textureID]);


		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		return true;
	}
	else
	{
		glBindTexture( GL_TEXTURE_2D , textureName[DEFAULT_TEXTURE]);


		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		return false;		// 如果申请不存在的纹理,返回第0号.
	}
}