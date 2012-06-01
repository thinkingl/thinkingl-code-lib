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

// ��ʼ��.
void TextureManager::Init()
{
	iTextureAmount = 0;
	// ��ȡ���������ļ�.
	CIniSet iniTextureFile = CIniSet( TEXTURE_FILE );

	// �������������ļ��б�
	char index[20] ;
	char *fileName ;

	sprintf_s( index ,20,"Texture%d",iTextureAmount);
	fileName = iniTextureFile.ReadText( index , NAME);
	while( fileName )
	{
		// ��ȡͼƬ�ļ�.
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

			// ����������1.
			iTextureAmount ++;
	    
		}
		else
		{
			// can't load the texture
			MessageBox( 0 , "Load Texture Erro!" , "Erro!" , MB_OK );
		}

		// �����ڴ�.
		if(texels)
			delete texels;
		texels = 0;
		if( fileName )
			delete fileName;

		// ��ȡ�µ�������ͼ�ļ���.
		sprintf_s( index ,20,"Texture%d",iTextureAmount);
		fileName = iniTextureFile.ReadText( index , NAME);
		
	}

}

// ������������.
GLuint TextureManager::GetTetureName(int textureID)
{
	if( textureID < iTextureAmount && textureID > 0)
		return textureName[ textureID ];
	else
		return textureName[DEFAULT_TEXTURE];		// ������벻���ڵ�����,���ص�0��.
}

// ʹ��ָ������
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
		return false;		// ������벻���ڵ�����,���ص�0��.
	}
}