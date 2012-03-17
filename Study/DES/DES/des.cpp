/** 标准的DES加密算法
*	是学习性质， 力求代码最清楚易懂， 不追求效率。
*	+by thinkingl@20120317
*	thinkingl@thinkingl.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <math.h>
#include <set>
#include <sstream>
#include <algorithm>
#include <memory.h>
#include <complex>
#include <queue>
#include <stack>
#include <bitset>

using namespace std;

#ifdef _WIN32
typedef __int64 n64;
typedef unsigned __int64 u64;
#else
typedef long long n64;
typedef unsigned long long u64;
#endif
typedef unsigned long u32;

// DES 是分组加密算法， 每次处理64bit, 8个字节。
const int DES_BIT_SIZE = 64;
const int DES_BUF_SIZE = 8;

/** 初始置换表 IP
*	是有规律的， 将顺序打乱， 偶数位在左半边， 奇数位在右半边。
*	这个表其实并没有密码学上的意义。
*/
const char DES_TABLE_IP[]    
= { 
	58, 50, 42, 34, 26, 18, 10,  2, 
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6, 
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1, 
	59, 51, 43, 35, 27, 19, 11,  3, 
	61, 53, 45, 37, 29, 21, 13,  5, 
	63, 55, 47, 39, 31, 23, 15,  7 
};

// 逆初始变换表， 是初始置换的逆变换。 
const char DES_TABLE_IPI[] =
{	
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

typedef std::bitset<DES_BIT_SIZE> CDESBuf64;

void Permutation( CDESBuf64& desBuf, const char* permutationTable )
{
	const CDESBuf64 in = desBuf;
	for ( size_t i=0; i<in.size(); ++i )
	{
		desBuf[ permutationTable[i]-1 ] = in[i];
	}
}

// 初始置换。
void IP( CDESBuf64& desBuf )
{
	Permutation( desBuf, DES_TABLE_IP );
}
// 逆初始置换。
void IPI( CDESBuf64& desBuf )
{
	Permutation( desBuf, DES_TABLE_IPI );
}

// 每一轮的DES。
void DESRound( CDESBuf64& desBuf )
{

}

void DES64( const CDESBuf64& in, CDESBuf64& out )
{
	out = in;

	// 进行初始置换。
	IP( out );

	// 16轮的变换。
	for ( int i=0; i<16; ++i )
	{
		DESRound( out );
	}

	// 最后再进行一次逆初始置换。 是初始置换的逆变换。
	IPI( out );
}


// DES 加密， 要求 明文长度必须是8的倍数（64bit）， 存放密文的 pEncBuf 长度必须大于等于 原文长度。
bool DES( const void* pPlanBuf, int planLength, void *pEncBuf )
{
	if ( planLength % 8 != 0 )
	{
		return false;
	}

	for ( int i=0; i*DES_BUF_SIZE<planLength; ++i )
	{
		const unsigned char* planBufIn = reinterpret_cast<const unsigned char*>( pPlanBuf ) + i*DES_BUF_SIZE;
		CDESBuf64 tmpIn( *reinterpret_cast<const u64*>(planBufIn) );
		CDESBuf64 tmpOut;

		DES64( tmpIn, tmpOut );
		u64 result = tmpOut.to_ullong();
		memcpy(  reinterpret_cast<unsigned char*>( pEncBuf)  + i*DES_BUF_SIZE, &result, DES_BUF_SIZE );
	}
	return true;
}

// 对字符串进行加密。 如果不是8的倍数， 自动补0.
void DES( std::string& planTxt, std::vector<char>& encBuf )
{
	if ( planTxt.size() % DES_BUF_SIZE != 0 )
	{
		size_t newSize = ( planTxt.size() / DES_BUF_SIZE + 1 ) * DES_BUF_SIZE;
		planTxt.resize( newSize, '\0' );
	}

	encBuf.clear();
	encBuf.resize( planTxt.size() );
	DES( planTxt.c_str(), planTxt.size(), encBuf.data() );
}

int main()
{
	// test.
	
	string planTxt = "Hello DES!!";

	vector<char> encBuf;
	DES( planTxt, encBuf );

	char *encStr = encBuf.data();

	system( "pause" );
	return 0;
}
