//********************************************
//	IniSet  by thinkingl
//  对配置文件进行的操作.

//  创建于2006 .1 . 6 
//  借鉴了 Sword2 的源代码.
//********************************************
#ifndef _GP_INI_
#define _GP_INI_

#pragma once

#define ERROR_DATA -99999999

#define _RELEASE(A)			try{ if( (A) != NULL ) { (A) -> Release(); (A) = NULL; } } catch(...) { PutDebugString("error ->Release!"); }
#define _DELETE(X)			try{ if( (X) != NULL ) { delete (X); (X) = NULL; } } catch(...) { PutDebugString("error delete!"); }
#define _DELETE_ARRAY(X)	try{ if( (X) != NULL ) { delete[] (X); (X) = NULL; } } catch(...) { PutDebugString("error delete[]!"); }
#define _FREE(X)			try{ if( (X) != NULL ) { free(X); (X)=NULL; } } catch(...) { PutDebugString("error free()!"); }

//  定义各个字符串缓冲区的最大长度.
int const MAX_LENGTH = 260;

//配置文件类
class CIniSet{
private:
	char FileName[MAX_LENGTH];	//文件名
	int DataLen;			//文件长度
	char *Data;				//文件内容

	int IndexNum;			//索引数目（[]的数目）
	int *IndexList;			//索引点位置列表

	int Point;				//当前指针
	int Line, Word;			//当前行列

public:
	CIniSet();
	CIniSet(char *);		//初始化打开配置文件
	~CIniSet();				//释放内存
	char *GetData();		//返回文件内容
	int GetLines(int);			//返回文件的行数

	bool Open( char *);		//打开配置文件
	bool Save(char *filename=0);		//保存配置文件

private:
	void InitIndex();			//初始化索引
	int FindIndex(char *);		//返回标题位置
	int FindData(int, char *);	//返回数据位置
	int GotoNextLine(int); 		//提行
	char *ReadDataName(int &);	//在指定位置读一数据名称
	char *ReadText(int);		//在指定位置读字符串

	bool AddIndex(char *);		//加入一个索引
	bool AddData(int, char *, char *);	//在当前位置加入一个数据
	bool ModityData(int, char *, char *); //在当前位置修改一个数据的值

	// ***************************
	// 获取文件长度
	int GetFileLength(char *);
	// **************************
	// 显示调试信息
	void PutDebugString(char *);
	// **************************
	// 显示调试信息
	void ShowMessage(char *msg,...);

public:
	int ReadInt(char *, char *);	// 读一个整数
	char *ReadText(char *, char *);	// 读一个字符串
	int ReadInt(char *, int );		// 在指定的行读一整数
	char *ReadText(char *, int);	// 在指定的行读一字符串
	char *ReadData(char *, int);	// 在指定行读一字符名称

	bool WriteInt(char *, char *, int);		// 写一个整数
	bool WriteText(char *, char *, char *);	// 写一个字符串

	int GetContinueDataNum(char *);			// 返回连续的行数（从INDEX到第一个空行）

};

#endif