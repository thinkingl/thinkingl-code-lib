#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/** 
	����Windows�����ļ����µ��ļ���Ŀ���ơ�
	�ҵĻ�����Vista 64bit ultimate
	i = 1107169 ʱһ�������������ǻ����е�������
*/

int main()
{
	char * strRootFolder = ( "F:\\download\\maxfile\\" );

	int i = 1;
	while( i )
	{
		stringstream ssFilePath;
		ssFilePath << strRootFolder << i;
		ofstream of;
		of.open( ssFilePath.str().c_str() );
		if ( !of )
		{
			cout << ( "Create File Fail!!" ) << ssFilePath.str() << endl;

			system( "pause" );
		}
		else
		{
			of << i;
			of.close();
		}		

		i ++;
	}

	return 0;
}