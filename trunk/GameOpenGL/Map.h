#pragma once


const int MAX_MAPFILE_PATH = 255;	// 地图文件的路径和文件名的最大长度.
const int MAX_MAP_AMOUNT = 100;		// 能支持的地图的最大数目.
// 地图格子
struct stCell
{
	bool block;			// 是否能通过.
	float  height;		// 高度.
	unsigned GroundPic;	// 地面纹理编号

};

// 地图数据结构
struct stMap
{
	stCell *cell;				// 用于存储地图的缓冲区坐标.

	int mapId;					// 地图代号.
	int width,height;			// 地图的大小.
	float xExcursion,yExcursion,zExcursion;// 地图左下角在所处场景的坐标.即地图在全局的偏移量.
//	char *mapName;				// 地图名称.
};
class Map
{
public:
	Map(void);
public:
	~Map(void);
public:
	static Map  *GetInstance();
	static void FreeInstance();
private:
	static Map *m_mapInstance;
public:

	// 初始化地图.
	void Init();

	// 创建一个新的地图.
	bool CreateNewMap( int id,float x, float y , float z,int width=100 , int height=100 );

	// 载入地图
	bool LoadMapFromFile( char *FileName );

	// 保存地图
	bool SaveMapToFile( int id , char * FileName );

	// 获得一个格子
	bool GetCell( int id , int x, int y , stCell *cell);
	// 获得一个地图的大小
	bool GetMapSize( int id ,int *width , int *height );
	// 获得一个地图的偏移
	bool GetMapExcursion( int id , float *x, float *y , float *z );

private:
	//------------------------------------------
	//       地图的指针数组.
	stMap	*m_mapList[MAX_MAP_AMOUNT];	// 地图的列表.
	int m_mapAmount;					// 地图的数量.
	bool PushMap( stMap * map);			// 将地图加入列表.

	stMap * FindMap( int id );		// 找到指定ID的地图.

	//int mapId;			// 地图代号.
	//
	//int width,height;	// 地图的大小.
	//float sx,sy;		// 地图左下角在所处场景的坐标.

	//char mapFileName[MAX_MAPFILE_PATH];	// 当前地图文件
	//char mapName[MAX_MAPFILE_PATH];		// 地图名字.
	//stCell *cell;
};

