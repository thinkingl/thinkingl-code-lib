#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/** 
	测试Windows对于文件夹下的文件数目限制。
	我的机器是Vista 64bit ultimate
	i = 1107169 时一切正常。（就是机器有点慢）。
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