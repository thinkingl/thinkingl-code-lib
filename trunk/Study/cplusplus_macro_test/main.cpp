
#include <iostream>

using namespace std;

extern "C"
{
	void test__cplusplusInExternC()
	{
#ifdef __cplusplus
		printf( "__cplusplus has defined in externC function, value: %ld\n", __cplusplus );
#else
		printf( "%s", "__cplusplus not defined  in externC function!" );
#endif
	}

	void test__cplusplusInC();
};

int main()
{
#ifdef __cplusplus
	cout << "__cplusplus has defined, value: " << __cplusplus << endl;
#else
	cout << "__cplusplus not defined!"
#endif

	test__cplusplusInExternC();

	test__cplusplusInC();
	system( "pause" );
	return 0;
}