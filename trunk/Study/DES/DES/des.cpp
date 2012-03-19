/** 标准的DES加密算法
*	是学习性质， 力求代码最清楚易懂， 符合标准，正确无误， 不追求效率。
*	定义 右边为低位， 左边为高位， 和计算机中的数字二进制保持一致。
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

/** Initial Permutation (IP) 
*	初始置换表 IP
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

// Inverse Initial Permutation (IP-1) 逆初始变换表， 是初始置换的逆变换。 
const char DES_TABLE_FP[] =
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

//  Expansion Permutation  扩展置换表， 将32bit扩展到48bit。
const char DES_TABLE_EP[] =
{	
	32,  1,  2,  3,  4,  5, 4,  5,
	6,  7,  8,  9,  8,  9, 10, 11,
	12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21,
	22, 23, 24, 25, 24, 25, 26, 27,
	28, 29,	28, 29, 30, 31, 32,  1
};

// Permuted Choice One 
const char DES_TABLE_PC1[] =
{
	57, 49, 41, 33, 25, 17,  9, 
	1,	58, 50, 42, 34, 26, 18, 
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3,	60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7,	62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 
	21, 13,  5,	28, 20, 12,  4
};

// Permuted Choice Two 
const char DES_TABLE_PC2[] =
{	
	14, 17, 11, 24,  1,  5,  3, 28,
	15,  6, 21, 10, 23, 19, 12,  4,
	26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40,
	51, 45, 33, 48, 44, 49, 39, 56,
	34, 53, 46, 42, 50, 36, 29, 32
// 竟然和书上的不一样！！
// 	45, 56, 35, 41, 51, 59, 34, 44,
// 	55, 49, 37, 52, 48, 53, 43, 60,
// 	38, 57, 50, 46, 54, 40, 33, 36
};

// Left Shifts table. 每一轮的DES都根据这个左移密钥。
const char DES_TABLE_LS[] =
{	
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1
};

// 左移一步。	因为发现用 << 的话会受到字节序的影响， 所以直接用置换解决。
const char DES_TABLE_LS1[] =
{	 
	2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, 25,
	26, 27, 28, 1,  0,  0,  0,  0
};

// 左移两步。
const char DES_TABLE_LS2[] =
{	
	3,  4,  5,  6,  7,  8,  9, 10,
	11, 12, 13, 14, 15, 16, 17, 18,
	19, 20, 21, 22, 23, 24, 25, 26,
	27, 28,  1,  2,  0,  0,  0,  0
};

// 左移表。
// const char* DES_TABLE_LS_TABLE[] =
// {
// 	DES_TABLE_LS1, DES_TABLE_LS2
// };

// DES 的核心， 8个 S-Box
const char DES_TABLE_SBOX_1[] =
{	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};

const char DES_TABLE_SBOX_2[] =
{	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

const char DES_TABLE_SBOX_3[] =
{	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

const char DES_TABLE_SBOX_4[] =
{	 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

const char DES_TABLE_SBOX_5[] =
{	 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

const char DES_TABLE_SBOX_6[] =
{	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

const char DES_TABLE_SBOX_7[] =
{	 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

const char DES_TABLE_SBOX_8[] =
{	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11};

const char *DES_TABLE_SBOX[] = { DES_TABLE_SBOX_1, DES_TABLE_SBOX_2, DES_TABLE_SBOX_3, DES_TABLE_SBOX_4, DES_TABLE_SBOX_5, DES_TABLE_SBOX_6, DES_TABLE_SBOX_7, DES_TABLE_SBOX_8};

// S盒子选择取值做完后的置换P。
const char DES_TABLE_P[] =
{	
	16,  7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26,  5, 18, 31, 10,
	2,  8, 24, 14, 32, 27,  3,  9,
	19, 13, 30,  6, 22, 11,  4, 25
};

typedef std::vector< unsigned char > CDESBuf;

typedef std::bitset<DES_BIT_SIZE> CDESBuf64;
typedef std::bitset<DES_BIT_SIZE/2> CDESBuf32;
typedef std::bitset<48> CDESBuf48;
typedef std::bitset<28> CDESBuf28;
typedef std::bitset<6> CDESBuf6;
typedef std::bitset<4> CDESBuf4;
typedef std::bitset<2> CDESBuf2;

typedef std::vector< CDESBuf48 > CDESRoundKeyList;

template< typename _BitSetBuf >
void Permutation( _BitSetBuf& desBuf, const char* permutationTable )
{
	const _BitSetBuf tmpBuf = desBuf;
	desBuf.reset();
	for ( size_t i=0; i< tmpBuf.size(); ++i )
	{
		desBuf[ i ] = tmpBuf[ permutationTable[i]-1 ];
	}
}

// 初始置换。
void IP( CDESBuf64& desBuf )
{
	Permutation( desBuf, DES_TABLE_IP );
}
// 逆初始置换。
void FP( CDESBuf64& desBuf )
{
	Permutation( desBuf, DES_TABLE_FP );
}

// F Feistel Function
// 根据S-Box， 输出32bit的加密数据。
// void Feistel( const CDESBuf48& xoredBuf, CDESBuf32& cipherTextBuf )
// {
// 	
// }

// 扩展置换。将32bit补足到48bit。
void PermutationExpansion( const CDESBuf32& buf32, CDESBuf48& buf48 )
{
	for ( size_t i=0; i<buf48.size(); ++i )
	{
		buf48[i] = buf32[ DES_TABLE_EP[i] - 1 ];
	}
}

void SBoxChoice( const CDESBuf48& xoredKey, CDESBuf32& sboxOut )
{
	// 根据XOR的结果来选择S-Box中的值。
	// 一共8个S-Box，分别对应于XOR中的8个6bit。 XOR 中每6bit 确定一个S-Box中的取值。
	// 6bit中， 第一bit和最后一个bit 联合组成的数字确认 S-Box 中的行号。（ 0-3 ， S-Box一共四行）
	// 中间4bit 确定 S-Box中的列号。（ 0-15， S-Box一共16列 ）
	// 行号和列号对应的 S-Box 中的数值， 转换为 4bit 数据 （ 0- 15转为2进制）。
	// 8个S-Box， 取出 8 个 4bit 来， 拼成32bit 的密文。
	for( int i=0; i<8; ++i )
	{
		int begin = i * 6;
		int end = begin + 5;
		int sboxIndex = xoredKey[ begin ] * 2 + xoredKey[ end ];
		int sboxValue = DES_TABLE_SBOX[ i ][ sboxIndex ];

		// 将 S-Box中取出来的数值转换为2进制， 高低位顺序在这里很讨厌。。
		for ( size_t k=0; k<4; ++k )
		{
			sboxOut[ i*4 + k ] = ( 0 != ( (1<<(3-k))&sboxValue ) );
		}
		//cout << "cipher: " << cipherTextBuf << endl;
	}
}

// 每一轮的DES。
void DESRound( int roundNum, CDESBuf32& left, CDESBuf32& right, const CDESBuf48& roundKey )
{	
	// 用扩展置换， 将右半边扩展到48字节。 
	CDESBuf48 eRight;
	PermutationExpansion( right, eRight );

// 	cout << endl << "******************round " << roundNum << " *****************************" << endl;
// 	cout << "my left: " <<  left << endl;
// 	cout << "my right: " << eRight << endl;
		
	// 将本轮密钥和右半边异或。
	CDESBuf48 xored = roundKey ^ eRight;

	// 用S-box引入非线性加密。
	CDESBuf32 exp;
	SBoxChoice( xored, exp );

//	cout << "my exp: " << exp << endl;

	// 进行P置换。
	Permutation( exp, DES_TABLE_P );

//	cout << "exp after P: " <<exp << endl;

	// 和左边进行异或（XOR）结果作为下一轮的右边值。
	exp ^= left;

	// 因为DES中每次左右都要互换。
	left = right;
	right = exp;	

// 	cout << "left result : " << left << endl;
// 	cout << "right result: " << right << endl;
// 
// 	cout << "******************************round end**********" << endl;
}

// PC1 置换。 从64bit的key中分出真正使用的56bit。
void KeyPC1( const CDESBuf64& key, CDESBuf28& leftKey, CDESBuf28& rightKey )
{
	for ( size_t i=0; i<leftKey.size(); ++i )
	{
		// 右边为低位
		rightKey[i] = key[ DES_TABLE_PC1[i] - 1 ];

		// 左边为高位。
		leftKey[i] = key[ DES_TABLE_PC1[ i+leftKey.size() ] - 1 ];
	}
}

//  把16轮的Key全部求出来。 因为解码的时候要倒序使用。
void KeyGeneration( const CDESBuf64& key, int desRound, CDESRoundKeyList& keyList )
{
	keyList.clear();

	// 求得初始的左右Key。
	CDESBuf28 leftKey, rightKey;
	KeyPC1( key, leftKey, rightKey );

	for ( int i=0; i< desRound; ++i )
	{
		// 根据左移置换表循环左移密钥。
// 		const char* desTableLS = DES_TABLE_LS_TABLE[ DES_TABLE_LS[ i ] - 1 ];
// 		Permutation( leftKey, desTableLS );
// 		Permutation( rightKey, desTableLS );
		leftKey <<= DES_TABLE_LS[ i ];
		rightKey <<= DES_TABLE_LS[ i ];

		// 用PC2 置换表生成本轮的密钥。
		CDESBuf48 roundKey;
		for ( size_t r=0; r<roundKey.size(); ++r )
		{
			int index = DES_TABLE_PC2[r] - 1;
			if ( (size_t)index < rightKey.size() )
			{
				roundKey[r] = rightKey[ index ];
			}
			else
			{
				roundKey[r] = leftKey[ index - leftKey.size() ];
			}		
		}

//		cout << "my key_" << i << ": " << roundKey << endl;
		keyList.push_back( roundKey );
	}
}


/** 加密一个64bit的block
*	int desRound : 为了调试和学习， 控制DES的加密轮数。因为轮数低了容易追踪。
*/
void DES64( const CDESBuf64& in, const CDESRoundKeyList& keyList, int desRound, CDESBuf64& out )
{
	out = in;

	// 进行初始置换。
	IP( out );
	
	// 分为左右两部分， 各32bit。
	CDESBuf32 left, right, left2, right2;
	for( size_t i=0; i<left.size(); ++i )
	{
		right[i] = out[i];
		left[i] = out[i+left.size()];
	}

// 	cout << "my left: " << left << endl;
// 	cout << "my right: " << right << endl;

#if 0	// 这样拆分牵扯到主机字节序， 不科学！。
	left2 = CDESBuf32( static_cast<int>( desBuf.to_ullong() ) );
	right2 = CDESBuf32( static_cast<int>( ( desBuf >> 32 ).to_ullong() ));
	bool kkk = left2 == left;
	bool ww = right2 == right;
#endif

	// 16轮的变换。
	for ( int i=0; i<desRound; ++i )
	{
		DESRound( i, left, right, keyList[i] );
	}

	// 最后左右呼唤一次， 这样加密和解密的过程就一样了。
	CDESBuf32 swapBuf = left;
	left = right;
	right = swapBuf;

	// 合并左右。
	for( size_t i=0; i<left.size(); ++i )
	{
		out[i] = right[i];
		out[ i+left.size() ] = left[i];
	}

	// 最后再进行一次逆初始置换。 是初始置换的逆变换。
	FP( out );
}


// DES 加密， 要求 明文长度必须是8的倍数（64bit）， 存放密文的 pEncBuf 长度必须大于等于 原文长度。
// key 必须是64bit 长度。
bool DES( const void* pPlainText, int plainTextLen, const void* pKey,  int desRound, void *pCipherTextBuf, bool denc )
{
	if ( plainTextLen % 8 != 0 )
	{
		return false;
	}

	CDESBuf64 key( *reinterpret_cast<const u64*>( pKey ) );

	const u64 *p64Block = reinterpret_cast<const u64*>(pPlainText);
	for ( int i=0; i*DES_BUF_SIZE<plainTextLen; ++i )
	{		
		
		CDESBuf64 tmpIn( p64Block[i] );
		CDESBuf64 tmpOut;

		CDESRoundKeyList keyList;
		KeyGeneration( key, desRound, keyList );

		
		if ( denc )
		{
			// 如果是解密， 需要将密钥顺序颠倒过来。
			CDESRoundKeyList rKeyList;
			for ( CDESRoundKeyList::reverse_iterator rIt = keyList.rbegin(); rIt != keyList.rend(); ++rIt )
			{
				rKeyList.push_back( *rIt );
			}
			keyList = rKeyList;
		}
		
		DES64( tmpIn, keyList, desRound, tmpOut );
		u64 result = tmpOut.to_ullong();	// 转成64bit数字， 正好。。
		memcpy(  reinterpret_cast<unsigned char*>( pCipherTextBuf)  + i*DES_BUF_SIZE, &result, DES_BUF_SIZE );
	}
	return true;
}

// 对字符串进行加密。 如果不是8的倍数， 自动补0.
// 如果密钥长度不是8， 那么自动补0 到8. 超过8的长度会被忽略。
void DES( CDESBuf& plainText, CDESBuf& key, int desRound, CDESBuf& cipherTextBuf, bool denc )
{
	if ( plainText.size() % DES_BUF_SIZE != 0 )
	{
		size_t newSize = ( plainText.size() / DES_BUF_SIZE + 1 ) * DES_BUF_SIZE;
		plainText.resize( newSize, '\0' );
	}

	if ( key.size() < 8 )
	{
		key.resize( 8 );
	}

	cipherTextBuf.clear();
	cipherTextBuf.resize( plainText.size() );
	DES( plainText.data(), plainText.size(), key.data(), desRound, cipherTextBuf.data(), denc );
}


int main()
{
	// test.

	u64 kkkkk = 1;
	CDESBuf64 kkk( kkkkk );
	
	string plainText = "Hello DES!!!!!";
	
	string key = "It's a sicret!";

	CDESBuf plainTextBuf;
	plainTextBuf.assign( plainText.begin(), plainText.end() );

	CDESBuf keyTextBuf;
	keyTextBuf.assign( key.begin(), key.end() );

	CDESBuf cipherText;
	DES( plainTextBuf, keyTextBuf, 16, cipherText, false );

//	char *encStr = cipherText.data();

	char szBack[16] = {0};
	//des( szBack, (char*)szBuf, 8, (char*)keyBuf.data(), 1, 0 );

	CDESBuf DecPlain;
	DES( cipherText, keyTextBuf, 16, DecPlain, true );
	string newPlain = reinterpret_cast<char*>( DecPlain.data() );

	bool isOk = ( newPlain == plainText );
	if ( !isOk )
	{
		cout << "靠， 还不行！" << endl;
	}

	system( "pause" );
	return 0;
}
