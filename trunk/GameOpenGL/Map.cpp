#include "Map.h"
#include "stdafx.h"
#include "include\iniset.h"

#define   MAP_HEAD  "Mapfile Powered by Sinbad."		// �ļ�ͷ����,���Ȳ�����MAP_HEAD_LENGTH.
const int MAP_HEAD_LENGTH = 32;							// ��ͼ�ļ����ļ�ͷ��С.
const int MAP_NAME_LENGTH =	256;						// ��ͼ���Ƶ���󳤶�.
#define   MAP_SET_FILE  "ini\\map.ini"					// ��ͼ�����ļ���·��.				

Map *Map::m_mapInstance = 0;

Map::Map(void)
{
	Init();
}

Map::~Map(void)
{
}

Map *Map::GetInstance()
{
	if(m_mapInstance == 0)
        m_mapInstance = new Map;
    return m_mapInstance;
}

void Map::FreeInstance()
{
	if(m_mapInstance != 0)
		delete m_mapInstance;
    m_mapInstance = 0;
}

void Map::Init()
{
	// ��Ա������ʼ��
	m_mapAmount = 0;

	// �򿪵�ͼ�����ļ�.
	CIniSet mapFile;
	mapFile.Open( MAP_SET_FILE );

	// ���ݵ�ͼ�����ļ����ص�ͼ.
	int iterator = 0;
	char index[32] ;
	char *mapFileName;
	sprintf_s( index , 32 , "Map%d" , iterator );
	mapFileName = mapFile.ReadText( index , "File" );
	while ( mapFileName )
	{
		LoadMapFromFile( mapFileName );
		iterator ++;

		delete mapFileName;
		sprintf_s( index , 32 , "Map%d" , iterator );
		mapFileName = mapFile.ReadText( index ,"File");
	}
}

// ����һ���µĵ�ͼ.
bool Map::CreateNewMap( int id  ,float x , float y , float z ,int mapWidth, int mapHeight)
{
	// ���ָ��ID�ĵ�ͼ�Ƿ��Ѿ�����.
	if ( FindMap( id ) != 0 )
	{
		MessageBox( NULL , "��ID�ĵ�ͼ�Ѿ�����!" , "������ͼ����." , MB_OK);
		return false;
	}
	// �µ�ͼ����������
	stMap *newMap;
	newMap = new stMap;

	newMap->mapId = id;
	newMap->width = mapWidth;
	newMap->height = mapHeight;
	newMap->xExcursion = x;
	newMap->yExcursion = y;
	newMap->zExcursion = z;

	// �����ڴ�.
	newMap->cell = new stCell[newMap->width * newMap->height];

	// ��ʼ���µ�ͼ.
	stCell * curCell = newMap->cell;	// ��ǰ����.
	for ( int i = 0 ; i < newMap->width * newMap->height ; i ++ )
	{
		curCell->block = 0;
		curCell->GroundPic = 0;
		curCell->height = 0.0f ;
		curCell ++;
	}

	// ���µ�ͼ�����ͼ�б�.
	if( PushMap( newMap) == 0 )
	{
		MessageBox( NULL, "��ͼ�޷������б�!","��ͼ��������.",MB_OK);
		return false;
	}
	return true;
}

// �����ͼ.
bool Map::LoadMapFromFile( char *FileName)
{
	stMap *map = new stMap;
	map->mapId = 1;
	map->width = 200;
	map->height = 200;
	map->xExcursion = -100;
	map->yExcursion = 0;
	map->zExcursion = -100;


	map->cell = new stCell[ map->width * map->height ];
	for ( int w=0; w<map->width; ++w )
	{
		for ( int h=0; h<map->height; ++h )
		{
			map->cell[ w*map->height + h ].block = false;
			map->cell[ w*map->height + h ].height = 0;
			map->cell[ w*map->height + h ].GroundPic = rand() % 5;
		}
	}
	// ����ͼ�����б�.
	PushMap( map );
	
	return true;
#if 0
	// �򿪵�ͼ�ļ�.
	FILE *fp;

	errno_t err ;
	if( (err=fopen_s( &fp,FileName, "rb")) != NULL )
	{
		MessageBox(NULL,"Load map file error!Can't open the file.","���ص�ͼ����.",MB_OK);
		return false;
	}

	// �����µ�ͼ.
	stMap *map = new stMap;

	fseek( fp, MAP_HEAD_LENGTH, SEEK_SET);		// �����ļ�ͷ
	fread( &map->mapId,4,1,fp);					// ��ȡ��ͼID.
//	fread( map->mapName,MAP_NAME_LENGTH,1,fp);	// ��ȡ��ͼ����.
	fread( &map->width,4,1,fp);					// ��ȡ��ͼ���.
	fread( &map->height,4,1,fp);				// ��ȡ��ͼ����.
	fread( &map->xExcursion,4,1,fp);			// ��ȡ��ͼƫ����.x
	fread( &map->yExcursion,4,1,fp);			// ��ȡ��ͼƫ����.y
	fread( &map->zExcursion,4,1,fp);			// ��ȡ��ͼƫ����.z

	//// д���ͼ�ļ�����ֵ.
	//fwrite(MAP_HEAD, MAP_HEAD_LENGTH, 1, fp);	// �ļ�ͷռ32�ֽ�.
	//fwrite( &map->mapId , 4,1,fp);				// ��ͼID.
	//fwrite( map->mapName ,MAP_NAME_LENGTH,1,fp);// ��ͼ����.
	//fwrite( &map->width , 4,1,fp);				// ��ͼ���.
	//fwrite( &map->height, 4,1,fp);				// ��ͼ����.
	//fwrite( &map->xExcursion, 4,1,fp);			// ��ͼƫ��.
	//fwrite( &map->yExcursion, 4,1,fp);			// ��ͼƫ��.
	//fwrite( &map->zExcursion, 4,1,fp);			// ��ͼƫ��.

	// ��������ڴ�
	map->cell = new stCell[ map->width * map->height ];
	// ��ȡ��ͼ��������.
	fread( map->cell , sizeof(stCell), (map->width)*(map->height),fp);

	// �ر��ļ�
	fclose( fp);

	// ����ͼ�����б�.
	PushMap( map );

	return true;
#endif
}

// ����ָ��id�ĵ�ͼ���ļ�.
bool Map::SaveMapToFile(int id ,char *FileName)
{
	// Ѱ��ָ��id�ĵ�ͼ.
	stMap *map = FindMap( id );
	if( map == 0 )
	{
		MessageBox( NULL ,"�Ҳ�����ͼ!��ͼδ����!","�����ͼ����",MB_OK);
		return false;
	}

	// �򿪵�ͼ�ļ�.
	FILE *fp;
	errno_t err=fopen_s( &fp,FileName, "wb") ;
	if( err != NULL )
	{
		MessageBox(NULL,"Save map file error!","",MB_OK);
		return false;
	}

	// д���ͼ�ļ�����ֵ.
	fwrite(MAP_HEAD, MAP_HEAD_LENGTH, 1, fp);	// �ļ�ͷռ32�ֽ�.
	fwrite( &map->mapId , 4,1,fp);				// ��ͼID.
	fwrite( &map->width , 4,1,fp);				// ��ͼ���.
	fwrite( &map->height, 4,1,fp);				// ��ͼ����.
	fwrite( &map->xExcursion, 4,1,fp);			// ��ͼƫ��.
	fwrite( &map->yExcursion, 4,1,fp);			// ��ͼƫ��.
	fwrite( &map->zExcursion, 4,1,fp);			// ��ͼƫ��.

	fwrite( map->cell , sizeof(stCell), map->width*map->height,fp);// ���е�ͼ����.

	fclose( fp);
	return true;

}

// �ҵ�ָ��ID�ĵ�ͼ.
stMap * Map::FindMap(int id)
{
	for( int i = m_mapAmount -1 ; i>=0 ; i--)
	{
		if ( m_mapList[i]->mapId = id)
			return m_mapList[i];
	}
	return 0;
}

// ����ͼ�����б�.
bool Map::PushMap( stMap *newMap)
{
	if ( m_mapAmount >= MAX_MAP_AMOUNT )
	{
		MessageBox( NULL , "��ͼ�����Ѿ��ﵽ���ֵ!","��ͼ�洢����!",MB_OK);
		return false;
	}
	m_mapList[m_mapAmount] = newMap;
	m_mapAmount ++;
	return true;
} 

// ��õ�ͼ��һ������
bool Map::GetCell(int id, int x, int y, stCell *cell)
{
	stMap *map = FindMap( id );
	if ( map == 0 )
	{
		return 0;	// û���ҵ���ͼ.
	}
	if ( x >= map->width || y>= map->height )
	{
		return 0;	// ������ͼ�ķ�Χ.
	}
	*cell = map->cell[y* map->width + x];
	return true;
}

// ��õ�ͼ��С
bool Map::GetMapSize(int id , int *width , int *height )
{
	stMap *map = FindMap( id );
	if ( map == 0 )
	{
		return 0;	// û���ҵ���ͼ.
	}
	*width = map->width;
	*height = map->height;
	return true;
}

// ��õ�ͼ��ƫ����
bool Map::GetMapExcursion(int id, float *x, float *y, float *z)
{
	stMap *map = FindMap(id );
	if (map == 0 )
	{
		return 0;	 // û���ҵ���ͼ.
	}
	*x = map->xExcursion;
	*y = map->yExcursion;
	*z = map->zExcursion;
	return true;
}