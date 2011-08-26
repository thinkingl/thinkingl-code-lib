// CorrectionPointsImportTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc < 3 )
	{
		cout << "缺少参数: 输入文本文件 输出文本文件 " << endl;
		return -1;
	}

	if ( argv[0] == NULL || argv[0][0] == 0 )
	{
		cout << "输入文件不能为空!" << endl;
		return -1;
	}

	if ( argv[1] == NULL || argv[1][0] == 0 )
	{
		cout << "输出文件不能为空!" << endl;
		return -1;
	}

	{
		// 格式: 地球经度 地球纬度 火星经度偏移量 火星纬度偏移量
		string inFileName = argv[1];
		string outFileName = argv[2];

		ifstream iFile( inFileName );
		ofstream oFile( outFileName );
 		iFile.precision( 20 );
 		oFile.precision( 20 );
		while ( iFile )
		{
			double longitudeEarth, lantitudeEarth, marsLongitudeOffset, marsLantitudeOffset;
			iFile >> longitudeEarth >> lantitudeEarth >> marsLongitudeOffset >> marsLantitudeOffset;

			// 将这条记录导入数据库文本文件..
			double longitudeMars = longitudeEarth + marsLongitudeOffset;
			double latitudeMars = lantitudeEarth + marsLantitudeOffset;

			oFile << longitudeEarth << "|" << lantitudeEarth << "|"
				<< longitudeMars << "|" << latitudeMars << endl;

		}
	}
	return 0;
}

