
#include <stdio.h>

void test__cplusplusInC()
{
#ifdef __cplusplus
	printf( "__cplusplus has defined in C function, value: %ld\n", __cplusplus );
#else
	printf( "%s", "__cplusplus not defined  in C function!" );
#endif
};