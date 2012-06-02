#include "Map.h"
#include "stdafx.h"
#include "include\iniset.h"

#define   MAP_HEAD  "Mapfile Powered by Sinbad."		// 文件头内容,长度不超过MAP_HEAD_LENGTH.
const int MAP_HEAD_LENGTH = 32;							// 地图文件的文件头大小.
const int MAP_NAME_LENGTH =	256;						// 地图名称的最大长度.
#define   MAP_SET_FILE  "ini\\map.ini"					// 地图配置文件的路径.				

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
	// 成员变量初始化
	m_mapAmount = 0;

	// 打开地图配置文件.
	CIniSet mapFile;
	mapFile.Open( MAP_SET_FILE );

	// 根据地图配置文件加载地图.
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

// 创建一个新的地图.
bool Map::CreateNewMap( int id  ,float x , float y , float z ,int mapWidth, int mapHeight)
{
	// 检测指定ID的地图是否已经存在.
	if ( FindMap( id ) != 0 )
	{
		MessageBox( NULL , "本ID的地图已经存在!" , "创建地图错误." , MB_OK);
		return false;
	}
	// 新地图的数据类型
	stMap *newMap;
	newMap = new stMap;

	newMap->mapId = id;
	newMap->width = mapWidth;
	newMap->height = mapHeight;
	newMap->xExcursion = x;
	newMap->yExcursion = y;
	newMap->zExcursion = z;

	// 申请内存.
	newMap->cell = new stCell[newMap->width * newMap->height];

	// 初始化新地图.
	stCell * curCell = newMap->cell;	// 当前格子.
	for ( int i = 0 ; i < newMap->width * newMap->height ; i ++ )
	{
		curCell->block = 0;
		curCell->GroundPic = 0;
		curCell->height = 0.0f ;
		curCell ++;
	}

	// 将新地图插入地图列表.
	if( PushMap( newMap) == 0 )
	{
		MessageBox( NULL, "地图无法插入列表!","地图创建错误.",MB_OK);
		return false;
	}
	return true;
}

// 载入地图.
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
	// 将地图加入列表.
	PushMap( map );
	
	return true;
#if 0
	// 打开地图文件.
	FILE *fp;

	errno_t err ;
	if( (err=fopen_s( &fp,FileName, "rb")) != NULL )
	{
		MessageBox(NULL,"Load map file error!Can't open the file.","加载地图错误.",MB_OK);
		return false;
	}

	// 创建新地图.
	stMap *map = new stMap;

	fseek( fp, MAP_HEAD_LENGTH, SEEK_SET);		// 跳过文件头
	fread( &map->mapId,4,1,fp);					// 读取地图ID.
//	fread( map->mapName,MAP_NAME_LENGTH,1,fp);	// 读取地图名称.
	fread( &map->width,4,1,fp);					// 读取地图宽度.
	fread( &map->height,4,1,fp);				// 读取地图长度.
	fread( &map->xExcursion,4,1,fp);			// 读取地图偏移量.x
	fread( &map->yExcursion,4,1,fp);			// 读取地图偏移量.y
	fread( &map->zExcursion,4,1,fp);			// 读取地图偏移量.z

	//// 写入地图文件各项值.
	//fwrite(MAP_HEAD, MAP_HEAD_LENGTH, 1, fp);	// 文件头占32字节.
	//fwrite( &map->mapId , 4,1,fp);				// 地图ID.
	//fwrite( map->mapName ,MAP_NAME_LENGTH,1,fp);// 地图名称.
	//fwrite( &map->width , 4,1,fp);				// 地图宽度.
	//fwrite( &map->height, 4,1,fp);				// 地图长度.
	//fwrite( &map->xExcursion, 4,1,fp);			// 地图偏移.
	//fwrite( &map->yExcursion, 4,1,fp);			// 地图偏移.
	//fwrite( &map->zExcursion, 4,1,fp);			// 地图偏移.

	// 申请格子内存
	map->cell = new stCell[ map->width * map->height ];
	// 读取地图网格数据.
	fread( map->cell , sizeof(stCell), (map->width)*(map->height),fp);

	// 关闭文件
	fclose( fp);

	// 将地图加入列表.
	PushMap( map );

	return true;
#endif
}

// 保存指定id的地图到文件.
bool Map::SaveMapToFile(int id ,char *FileName)
{
	// 寻找指定id的地图.
	stMap *map = FindMap( id );
	if( map == 0 )
	{
		MessageBox( NULL ,"找不到地图!地图未创建!","保存地图错误",MB_OK);
		return false;
	}

	// 打开地图文件.
	FILE *fp;
	errno_t err=fopen_s( &fp,FileName, "wb") ;
	if( err != NULL )
	{
		MessageBox(NULL,"Save map file error!","",MB_OK);
		return false;
	}

	// 写入地图文件各项值.
	fwrite(MAP_HEAD, MAP_HEAD_LENGTH, 1, fp);	// 文件头占32字节.
	fwrite( &map->mapId , 4,1,fp);				// 地图ID.
	fwrite( &map->width , 4,1,fp);				// 地图宽度.
	fwrite( &map->height, 4,1,fp);				// 地图长度.
	fwrite( &map->xExcursion, 4,1,fp);			// 地图偏移.
	fwrite( &map->yExcursion, 4,1,fp);			// 地图偏移.
	fwrite( &map->zExcursion, 4,1,fp);			// 地图偏移.

	fwrite( map->cell , sizeof(stCell), map->width*map->height,fp);// 所有地图格子.

	fclose( fp);
	return true;

}

// 找到指定ID的地图.
stMap * Map::FindMap(int id)
{
	for( int i = m_mapAmount -1 ; i>=0 ; i--)
	{
		if ( m_mapList[i]->mapId = id)
			return m_mapList[i];
	}
	return 0;
}

// 将地图加入列表.
bool Map::PushMap( stMap *newMap)
{
	if ( m_mapAmount >= MAX_MAP_AMOUNT )
	{
		MessageBox( NULL , "地图数量已经达到最大值!","地图存储错误!",MB_OK);
		return false;
	}
	m_mapList[m_mapAmount] = newMap;
	m_mapAmount ++;
	return true;
} 

// 获得地图的一个格子
bool Map::GetCell(int id, int x, int y, stCell *cell)
{
	stMap *map = FindMap( id );
	if ( map == 0 )
	{
		return 0;	// 没有找到地图.
	}
	if ( x >= map->width || y>= map->height )
	{
		return 0;	// 超过地图的范围.
	}
	*cell = map->cell[y* map->width + x];
	return true;
}

// 获得地图大小
bool Map::GetMapSize(int id , int *width , int *height )
{
	stMap *map = FindMap( id );
	if ( map == 0 )
	{
		return 0;	// 没有找到地图.
	}
	*width = map->width;
	*height = map->height;
	return true;
}

// 获得地图的偏移量
bool Map::GetMapExcursion(int id, float *x, float *y, float *z)
{
	stMap *map = FindMap(id );
	if (map == 0 )
	{
		return 0;	 // 没有找到地图.
	}
	*x = map->xExcursion;
	*y = map->yExcursion;
	*z = map->zExcursion;
	return true;
}