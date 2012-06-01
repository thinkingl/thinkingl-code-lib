#include "scene.h"
#using <mscorlib.dll>

#include "map.h"
#include "TextureManager.h"

#include "ModelManager.h"


const float CELL_BORDER = 0.5f;		// 地图的网格在显示的时候的大小.

Scene::Scene(void)
{
	SceneDisplayList = 0;
}

Scene::~Scene(void)
{
}

int Scene::DrawTheScene()
{


	if(SceneDisplayList)
	{
		glCallList( SceneDisplayList);	//显示地图.
	}
	else
	{
		MessageBox( NULL , "未加载场景或场景加载发生错误.","Erro!", MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
	}

	//ModelManager::GetInstance()->Show( 0 );

	return 0;
}

int Scene::LoadScene(void)
{
		// stuff for lighting
	GLfloat lAmbient[] = {0.3f,0.3f,0.3f,1.0f};
	GLfloat lDiffuse[] = {0.3f,0.3f,0.3f,1.0f};
	GLfloat lSpecular[] = {1.0f,1.0f,1.0f,1.0f};

	GLfloat lPosition[] = {0.0f,100.0f,0.0f,1.0f};

	// materials
	GLfloat mSpecular[] = {1.0f,1.0f,1.0f,1.0f};
	// the smaller the larger the specular area is
	GLfloat mShininess[] = {64.0};

	// EMISSION
	GLfloat lEmission[] = { 0.2f , 0.2f , 0.2f , 1.0f};

	////colors
	//GLfloat cBlack[] = {0.0f,0.0f,0.0f,1.0f};
	//GLfloat cOrange[] = {1.0f,0.5f,0.5f,1.0f}; 
	//GLfloat cWhite[] = {1.0f,1.0f,1.0f,1.0f}; 
	//GLfloat cGrey[] = {0.1f,0.1f,0.1f,1.0f};
	//GLfloat cLightGrey[] = {0.9f,0.9f,0.9f,1.0f};


	SceneDisplayList = glGenLists( 1 );		// 申请一个显示列表的名称.

	float xExcursion,yExcursion,zExcursion;	// 地图偏移量.
	int width , height ;					// 地图宽度和长度.
	stCell cell;
	
	int id = 1;
	// 获得地图大小和地图偏移.
	Map::GetInstance() ->GetMapSize( id , &width , &height );
	Map::GetInstance() ->GetMapExcursion( id ,&xExcursion , &yExcursion, &zExcursion );
	



	glNewList ( SceneDisplayList , GL_COMPILE );	// 开始创建新的显示列表.
	glPushMatrix();
	glTranslatef( xExcursion , yExcursion , zExcursion );


	glTexEnvf    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);///////////////////
	// 设置材质
	glMaterialfv( GL_FRONT , GL_AMBIENT , lAmbient );
	glMaterialfv( GL_FRONT , GL_DIFFUSE , lDiffuse );
	glMaterialfv( GL_FRONT , GL_SPECULAR, lSpecular );
	glMaterialfv( GL_FRONT , GL_SHININESS, mShininess );
//	glMaterialfv( GL_FRONT , GL_EMISSION , lEmission );

	glNormal3f( 0.0f , 1.0f ,0.0f );				//	设置法线为向上.
	for (int i = 0 ; i<height ; i ++ )	
	{	
		for(int j = 0 ; j< width; j++ )
		{
			Map::GetInstance() ->GetCell( id, j,i , &cell);
			TextureManager::GetInstance() ->UseTexture( cell.GroundPic );

			//  画地图.
			glBegin( GL_TRIANGLE_STRIP );
			glTexCoord2f( 0.0f ,1.0f );					// left top of the cell
			glVertex3f( j*CELL_BORDER ,	cell.height *CELL_BORDER,i *CELL_BORDER  );

			glTexCoord2f( 0.0f ,0.0f );					// left bottom of the cell.
			glVertex3f( j*CELL_BORDER ,	cell.height *CELL_BORDER, ( i+1 ) *CELL_BORDER);

			glTexCoord2f( 1.0f ,1.0f );					// right top of the cell.
			glVertex3f( (j+1)*CELL_BORDER,	cell.height *CELL_BORDER, i*CELL_BORDER);

			glTexCoord2f( 1.0f ,0.0f );					// right bottom of the cell.
			glVertex3f( (j+1)*CELL_BORDER,	cell.height *CELL_BORDER, (i+1)*CELL_BORDER);

			glEnd();
		}

	}

	// 画天空
	DrawSky( 1, 3,6,3,6);

	////开启雾效果 
	//GLfloat color[4] = {0.70f, 0.76f, 0.74f, 0.10f};
	//glEnable(GL_FOG);
	//glFogfv(GL_FOG_COLOR, color);
	//glFogf(GL_FOG_START, 5.0f);
	//glFogf(GL_FOG_END, 100.00f);
	//glFogi(GL_FOG_MODE, GL_LINEAR);

	glPopMatrix();
	glEndList();
	return SceneDisplayList;
}

int Scene::LoadScene( char * fileName )
{

	return 0;
}

//  画天空
void Scene::DrawSky(int mapId ,int a,int wi,int he,int le)
{
	// stuff for lighting
	GLfloat lAmbient[] = {0.3f,0.3f,0.3f,1.0f};
	GLfloat lDiffuse[] = {0.8f,0.8f,0.8f,1.0f};
	GLfloat lSpecular[] = {1.0f,1.0f,1.0f,1.0f};

	// materials
	GLfloat mSpecular[] = {1.0f,1.0f,1.0f,1.0f};
	// the smaller the larger the specular area is
	GLfloat mShininess[] = {64.0};

	// EMISSION
	GLfloat lEmission[] = { 0.8f , 0.8f , 0.8f , 1.0f};

	GLfloat width= 1000.0f;
	GLfloat height = 1000.0f ;

	// 保存当前状态
//	glPushMatrix();

//	glTranslatef( 0.0f ,0.0f ,0.0f );
	// 设置材质
	glMaterialfv( GL_FRONT , GL_AMBIENT , lAmbient );
	glMaterialfv( GL_FRONT , GL_DIFFUSE , lDiffuse );
	glMaterialfv( GL_FRONT , GL_SPECULAR, lSpecular );
	glMaterialfv( GL_FRONT , GL_SHININESS, mShininess );
	glMaterialfv( GL_FRONT , GL_EMISSION , lEmission );

	// 确定天空盒子的顶点坐标.
	float  skyWidth , skyHeight , skyLength;
	int mapWidth,mapHeight ;
	float xOffset,yOffset,zOffset;
	Map::GetInstance()->GetMapSize( mapId , &mapWidth , &mapHeight );
	Map::GetInstance()->GetMapExcursion( mapId, &xOffset , &yOffset , &zOffset);
	skyWidth = mapWidth * wi * CELL_BORDER;
	skyHeight = mapWidth * he *CELL_BORDER;
	skyLength = mapHeight * le * CELL_BORDER;
	float x = xOffset + mapWidth *CELL_BORDER/2 - skyWidth /2;
	float y = yOffset - skyHeight * CELL_BORDER/a;
	float z = zOffset + mapHeight * CELL_BORDER/2 - skyLength/2;
	
	//float width =m_nTerrainGridWidth*wi;
	//float height=m_nTerrainGridWidth*he;
	//float length=m_nTerrainGridLength*le;
	//float x = m_nTerrainGridWidth  -width /2;
	//float y = m_nTerrainGridWidth/a-height/2-100.0f;
	//float z = -m_nTerrainGridLength -length/2;

	//glColor3f( 1.0f, 1.0f, 1.0f );
	// front
	TextureManager::GetInstance()->UseTexture( 6 );
	glNormal3f( 0.0f ,0.0f , 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+skyWidth,y,		 z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+skyWidth,y+skyHeight,z); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+skyHeight,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();

	// back
	TextureManager::GetInstance()->UseTexture( 7 );
	glNormal3f( 0.0f ,0.0f , -1.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z+skyLength);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+skyHeight,z+skyLength);
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+skyWidth,y+skyHeight,z+skyLength); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+skyWidth,y,		 z+skyLength);
	glEnd();

	// top
	TextureManager::GetInstance()->UseTexture( 8 );
	glNormal3f( 0.0f ,-1.0f ,0.0f );
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+skyWidth,y+skyHeight,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+skyWidth,y+skyHeight,z+skyLength); 
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y+skyHeight,z+skyLength);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+skyHeight,z);
	glEnd();

	// left
	TextureManager::GetInstance()->UseTexture( 9 );
	glNormal3f( 1.0f , 0.0f ,0.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+skyHeight,z);	
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+skyHeight,z+skyLength); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+skyLength);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z);		
	glEnd();

	// right 
	TextureManager::GetInstance()->UseTexture( 10 );
	glNormal3f( -1.0f , 0.0f ,0.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+skyWidth,y,		 z);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+skyWidth,y,		 z+skyLength);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+skyWidth,y+skyHeight,z+skyLength); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+skyWidth,y+skyHeight,z);
	glEnd();
	///////////////////////////////////////////////////////////////////////////////

	// 恢复状态
//	glPopMatrix();
	
}