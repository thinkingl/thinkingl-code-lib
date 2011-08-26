// CorrectionPointsImportTool.cpp : �������̨Ӧ�ó������ڵ㡣
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
		cout << "ȱ�ٲ���: �����ı��ļ� ����ı��ļ� " << endl;
		return -1;
	}

	if ( argv[0] == NULL || argv[0][0] == 0 )
	{
		cout << "�����ļ�����Ϊ��!" << endl;
		return -1;
	}

	if ( argv[1] == NULL || argv[1][0] == 0 )
	{
		cout << "����ļ�����Ϊ��!" << endl;
		return -1;
	}

	{
		// ��ʽ: ���򾭶� ����γ�� ���Ǿ���ƫ���� ����γ��ƫ����
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

			// ��������¼�������ݿ��ı��ļ�..
			double longitudeMars = longitudeEarth + marsLongitudeOffset;
			double latitudeMars = lantitudeEarth + marsLantitudeOffset;

			oFile << longitudeEarth << "|" << lantitudeEarth << "|"
				<< longitudeMars << "|" << latitudeMars << endl;

		}
	}
	return 0;
}

