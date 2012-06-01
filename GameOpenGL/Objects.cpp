#include ".\objects.h"
#using <mscorlib.dll>

#include "TextureManager.h"

Objects::Objects(void)
{
	xrot = 1.0f ;
	turn1 = turn2 = 0.0f;
	ZDEEP = -7.0f ;
	filter = 0;
	for ( int i = 0 ; i < 50 ; i++)
	{
		stars[i].angle = 0.0f ;
		stars[i].dist = (float)i/50.0f * 10.0f;
		stars[i].b = rand()%256;
		stars[i].g = rand()%256;
		stars[i].r = rand()%256;
	}

	
}

Objects::~Objects(void)
{
}

//AUX_RGBImageRec * Objects::LoadBMP(char * FileName)
//{
//	FILE *File = NULL;		//Make sure a filename was given.
//	if(!FileName)
//	{
//		return 0;
//	}
//	File = fopen( FileName ,"r");		//Check to see if the file exists.
//	if(File)
//	{
//		fclose(File);
//		return auxDIBImageLoad(FileName);		//Load the image and return a pointer.
//	}
//	return NULL;
//}

//int Objects::LoadGLTextures(void)
//{
//	int Status = FALSE;		//To keep track of whether or not we were able to load the bitmap.
//	AUX_RGBImageRec *TextureImage[1];		//Creat storage space for the texture image.
//	memset(TextureImage , 0 , sizeof(void *) *1);		//Set the point to NULL.
//	TextureImage[0] = LoadBMP( "data/coolboy.bmp");//coolBoy.bmp");
//	if ( TextureImage[0] )
//	{
//		Status = TRUE;
//		glGenTextures( 3 , &texture[0]);
//		glBindTexture( GL_TEXTURE_2D , texture[0] );
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//		glTexImage2D(GL_TEXTURE_2D ,0 , 3 , TextureImage[0]->sizeX , TextureImage[0]->sizeY ,0,GL_RGB,GL_UNSIGNED_BYTE,TextureImage[0]->data);
//		
//		glBindTexture( GL_TEXTURE_2D , texture[1] );
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//		glTexImage2D(GL_TEXTURE_2D , 0 , 3 , TextureImage[0]->sizeX , TextureImage[0]->sizeY , 0 ,GL_RGB,GL_UNSIGNED_BYTE,TextureImage[0]->data);
//
//		glBindTexture(GL_TEXTURE_2D , texture[2]);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST);
//		gluBuild2DMipmaps(GL_TEXTURE_2D ,3,TextureImage[0]->sizeX,TextureImage[0]->sizeY,GL_RGB,GL_UNSIGNED_BYTE,TextureImage[0]->data);
//		
//	}
//	if (TextureImage[0])
//	{
//		if(TextureImage[0]->data)
//		{
//			free (TextureImage[0]->data);
//		}
//		free(TextureImage[0]);
//	}
//	return Status;
//}


void Objects::Box1(void)
{
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//The code is added by my own hands for study.!!Good luck!
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	glTranslatef( -2.0f , 2.0f ,ZDEEP );
	glRotatef( turn2 , 1.0f , 1.0f , 1.0f );
	
//	glBindTexture(GL_TEXTURE_2D,texture[filter]);
	TextureManager::GetInstance()->UseTexture( 5 );

	glBegin(GL_QUADS);

			// Right face
		glNormal3f( 1.0f ,0.0f , 0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad

		// Front Face
		glNormal3f( 0.0f ,0.0f ,1.0f);

		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad

		// Back Face
		glNormal3f( 0.0f , 0.0f , -1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
		// Top Face
		glNormal3f( 0.0f , 1.0f , 0.0f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
		// Bottom Face
		glNormal3f( 0.0f , -1.0f , 0.0f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad

		// Left Face
		glNormal3f( -1.0f , 0.0f , 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad


	glEnd();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f , 0.0f , ZDEEP);

	//TextureManager::GetInstance()->UseTexture(5);
	//glBegin (GL_QUADS);
	//		//background big picture.
	//	glNormal3f( 0.0f , 0.0f ,1.0f );
	//	glTexCoord2f( 1.0f ,1.0f );	glVertex3f( -1000.0f , 1000.0f , -2000.0f);
	//	glTexCoord2f( 1.0f , 0.0f );	glVertex3f( -1000.0f , -1000.0f , -2000.0f );
	//	glTexCoord2f( 0.0f ,0.0f );	glVertex3f( 1000.0f , -1000.0f , -2000.0f);
	//	glTexCoord2f( 0.0f ,1.0f ); glVertex3f( 1000.0f , 1000.0f , -2000.0f );
	//glEnd();

//	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	/*
	glPushMatrix();
//	glDisable(GL_LIGHTING);
	glTranslatef( 2.0 , 0.0 , ZDEEP );
	glRotatef( turn1 , 1.0f , 0.0f , 0.0f );
	glBegin( GL_TRIANGLE_STRIP );
		glColor4ub( 128 ,0,0,255);
		glVertex3f( 0.0f , 0.0f , 1.2f );
		glColor3f(0 , 124 , 0);
		glVertex3f( 0.0f , 1.0f , -1.0f );
		glColor3f( 0 , 0 , 124 );
		glVertex3f( 0.0f , -1.0f , -1.0f );
		glColor3f ( 1.0f , 1.0f , 0.0f);
		glVertex3f( 1.2f , 0.0f , 0.0f );
		glVertex3f( 0.0f , 0.0f , 1.2f );
		glColor3f ( 0.5f ,0.5f , 0.2f );
		glVertex3f( 0.0f , 1.0f , -1.0f );
	glEnd();
	glPopMatrix();
*/

	
//	glEnable(GL_LIGHTING);
	
    turn1 += xrot;
	turn2 -= xrot;

/*	for ( int loop = 0 ; loop < 50 ; loop ++)
	{
		glPushMatrix();
		glTranslatef( 0.0f , 0.0f , -20.0f);//ÔÚÆÁÄ»Àï20f´¦»­ÐÇÐÇ.
		glRotatef( stars[loop].angle , 0.0f , 0.0f ,1.0f );
		glTranslatef( stars[loop].dist , 0.0f ,0.0f );
		glRotatef( -stars[loop].angle , 0.0f ,0.0f ,1.0f );
		
		glColor4ub(stars[loop].r,stars[loop].g,stars[loop].b,255);
		glBegin(GL_QUADS);
			glNormal3f( 0.0f ,0.0f ,1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
		
		stars[loop].angle += (float)loop/50;
		stars[loop].dist -= 0.01f;
		if(stars[loop].dist < 0.0f)
		{
			stars[loop].dist = 10.0f;
		}
		glPopMatrix();
	}

*/
}



void Objects::test(void)
{

	//	glTranslatef( -2.0f , 0.0f ,-1.0f );
		glBegin(GL_QUADS);
			glNormal3f( 0.0f ,0.0f ,1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
}
