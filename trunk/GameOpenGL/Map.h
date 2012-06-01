#pragma once


const int MAX_MAPFILE_PATH = 255;	// ��ͼ�ļ���·�����ļ�������󳤶�.
const int MAX_MAP_AMOUNT = 100;		// ��֧�ֵĵ�ͼ�������Ŀ.
// ��ͼ����
struct stCell
{
	bool block;			// �Ƿ���ͨ��.
	float  height;		// �߶�.
	unsigned GroundPic;	// ����������

};

// ��ͼ���ݽṹ
struct stMap
{
	stCell *cell;				// ���ڴ洢��ͼ�Ļ���������.

	int mapId;					// ��ͼ����.
	int width,height;			// ��ͼ�Ĵ�С.
	float xExcursion,yExcursion,zExcursion;// ��ͼ���½�����������������.����ͼ��ȫ�ֵ�ƫ����.
//	char *mapName;				// ��ͼ����.
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

	// ��ʼ����ͼ.
	void Init();

	// ����һ���µĵ�ͼ.
	bool CreateNewMap( int id,float x, float y , float z,int width=100 , int height=100 );

	// �����ͼ
	bool LoadMapFromFile( char *FileName );

	// �����ͼ
	bool SaveMapToFile( int id , char * FileName );

	// ���һ������
	bool GetCell( int id , int x, int y , stCell *cell);
	// ���һ����ͼ�Ĵ�С
	bool GetMapSize( int id ,int *width , int *height );
	// ���һ����ͼ��ƫ��
	bool GetMapExcursion( int id , float *x, float *y , float *z );

private:
	//------------------------------------------
	//       ��ͼ��ָ������.
	stMap	*m_mapList[MAX_MAP_AMOUNT];	// ��ͼ���б�.
	int m_mapAmount;					// ��ͼ������.
	bool PushMap( stMap * map);			// ����ͼ�����б�.

	stMap * FindMap( int id );		// �ҵ�ָ��ID�ĵ�ͼ.

	//int mapId;			// ��ͼ����.
	//
	//int width,height;	// ��ͼ�Ĵ�С.
	//float sx,sy;		// ��ͼ���½�����������������.

	//char mapFileName[MAX_MAPFILE_PATH];	// ��ǰ��ͼ�ļ�
	//char mapName[MAX_MAPFILE_PATH];		// ��ͼ����.
	//stCell *cell;
};

