/** ��׼��DES�����㷨
*	��ѧϰ���ʣ� �������������׶��� ��׷��Ч�ʡ�
*	���� �ұ�Ϊ��λ�� ���Ϊ��λ��
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

// DES �Ƿ�������㷨�� ÿ�δ���64bit, 8���ֽڡ�
const int DES_BIT_SIZE = 64;
const int DES_BUF_SIZE = 8;

/** Initial Permutation (IP) 
*	��ʼ�û��� IP
*	���й��ɵģ� ��˳����ң� ż��λ�����ߣ� ����λ���Ұ�ߡ�
*	�������ʵ��û������ѧ�ϵ����塣
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

// Inverse Initial Permutation (IP-1) ���ʼ�任�� �ǳ�ʼ�û�����任�� 
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

//  Expansion Permutation  ��չ�û��� ��32bit��չ��48bit��
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
// ��Ȼ�����ϵĲ�һ������
// 	45, 56, 35, 41, 51, 59, 34, 44,
// 	55, 49, 37, 52, 48, 53, 43, 60,
// 	38, 57, 50, 46, 54, 40, 33, 36
};

// Left Shifts table. ÿһ�ֵ�DES���������������Կ��
const char DES_TABLE_LS[] =
{	
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1
};

// ����һ����	��Ϊ������ << �Ļ����ܵ��ֽ����Ӱ�죬 ����ֱ�����û������
const char DES_TABLE_LS1[] =
{	 
	2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, 25,
	26, 27, 28, 1,  0,  0,  0,  0
};

// ����������
const char DES_TABLE_LS2[] =
{	
	3,  4,  5,  6,  7,  8,  9, 10,
	11, 12, 13, 14, 15, 16, 17, 18,
	19, 20, 21, 22, 23, 24, 25, 26,
	27, 28,  1,  2,  0,  0,  0,  0
};

// ���Ʊ�
// const char* DES_TABLE_LS_TABLE[] =
// {
// 	DES_TABLE_LS1, DES_TABLE_LS2
// };

// DES �ĺ��ģ� 8�� S-Box
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

// ��ʼ�û���
void IP( CDESBuf64& desBuf )
{
	Permutation( desBuf, DES_TABLE_IP );
}
// ���ʼ�û���
void FP( CDESBuf64& desBuf )
{
	Permutation( desBuf, DES_TABLE_FP );
}

// F Feistel Function
// ����S-Box�� ���32bit�ļ������ݡ�
void Feistel( const CDESBuf48& xoredBuf, CDESBuf32& cipherTextBuf )
{
	// ����XOR�Ľ����ѡ��S-Box�е�ֵ��
	// һ��8��S-Box���ֱ��Ӧ��XOR�е�8��6bit�� XOR ��ÿ6bit ȷ��һ��S-Box�е�ȡֵ��
	// 6bit�У� ��һbit�����һ��bit ������ɵ�����ȷ�� S-Box �е��кš��� 0-3 �� S-Boxһ�����У�
	// �м�4bit ȷ�� S-Box�е��кš��� 0-15�� S-Boxһ��16�� ��
	// �кź��кŶ�Ӧ�� S-Box �е���ֵ�� ת��Ϊ 4bit ���� �� 0- 15תΪ2���ƣ���
	// 8��S-Box�� ȡ�� 8 �� 4bit ���� ƴ��32bit �����ġ�
	cipherTextBuf.reset();
	for( int i=0; i<8; ++i )
	{
		int begin = i * 6;
		int end = begin + 5;
		int sboxIndex = xoredBuf[ begin ] * 2 + xoredBuf[ end ];
		int sboxValue = DES_TABLE_SBOX[ i ][ sboxIndex ];
		
		// �� S-Box��ȡ��������ֵת��Ϊ2���ƣ� �ߵ�λ˳������������ᡣ��
		for ( size_t k=0; k<4; ++k )
		{
			cipherTextBuf[ i*4 + k ] = ( (1<<(3-k))&sboxValue );
		}
		//cout << "cipher: " << cipherTextBuf << endl;
	}
}

// ��չ�û�����32bit���㵽48bit��
void PermutationExpansion( const CDESBuf32& buf32, CDESBuf48& buf48 )
{
	for ( size_t i=0; i<buf48.size(); ++i )
	{
		buf48[i] = buf32[ DES_TABLE_EP[i] - 1 ];
	}
}

// ÿһ�ֵ�DES��
void DESRound( int roundNum, CDESBuf32& left, CDESBuf32& right, const CDESBuf48& roundKey )
{	
	// ����չ�û��� ���Ұ����չ��48�ֽڡ� 
	CDESBuf48 eRight;
	PermutationExpansion( right, eRight );

	cout << "my left: " << roundNum << ": " <<  left << endl;
	cout << "my right: " << roundNum << ": " << eRight << endl;
		
	// ��������Կ���Ұ�����
	CDESBuf48 xored = roundKey ^ eRight;

	// ����F������ ��S-box��������Լ��ܡ�
	// ��ΪDES��ÿ�����Ҷ�Ҫ������ �����ֱ�ӽ����ˣ� ��û�б仯����벿�ݸ��Ƹ��ұߡ� ���ܺ�Ĳ��ָ���ߡ���
	right = left;
	Feistel( xored, left );
}

// PC1 �û��� ��64bit��key�зֳ�����ʹ�õ�56bit��
void KeyPC1( const CDESBuf64& key, CDESBuf28& leftKey, CDESBuf28& rightKey )
{
	for ( size_t i=0; i<leftKey.size(); ++i )
	{
		// �ұ�Ϊ��λ
		rightKey[i] = key[ DES_TABLE_PC1[i] - 1 ];

		// ���Ϊ��λ��
		leftKey[i] = key[ DES_TABLE_PC1[ i+leftKey.size() ] - 1 ];
	}
}

//  ��16�ֵ�Keyȫ��������� ��Ϊ�����ʱ��Ҫ����ʹ�á�
void KeyGeneration( const CDESBuf64& key, CDESRoundKeyList& keyList )
{
	keyList.clear();

	// ��ó�ʼ������Key��
	CDESBuf28 leftKey, rightKey;
	KeyPC1( key, leftKey, rightKey );

	for ( int i=0; i<16; ++i )
	{
		// ���������û���ѭ��������Կ��
// 		const char* desTableLS = DES_TABLE_LS_TABLE[ DES_TABLE_LS[ i ] - 1 ];
// 		Permutation( leftKey, desTableLS );
// 		Permutation( rightKey, desTableLS );
		leftKey <<= DES_TABLE_LS[ i ];
		rightKey <<= DES_TABLE_LS[ i ];

		// ��PC2 �û������ɱ��ֵ���Կ��
		CDESBuf48 roundKey;
		for ( size_t r=0; r<roundKey.size(); ++r )
		{
			int index = DES_TABLE_PC2[r] - 1;
			if ( index < rightKey.size() )
			{
				roundKey[r] = rightKey[ index ];
			}
			else
			{
				roundKey[r] = leftKey[ index - leftKey.size() ];
			}		
		}

		cout << "my key_" << i << ": " << roundKey << endl;
		keyList.push_back( roundKey );
	}
}



void DES64( const CDESBuf64& in, const CDESRoundKeyList& keyList, CDESBuf64& out )
{
	out = in;

	// ���г�ʼ�û���
	IP( out );
	
	// ��Ϊ���������֣� ��32bit��
	CDESBuf32 left, right, left2, right2;
	for( size_t i=0; i<left.size(); ++i )
	{
		right[i] = out[i];
		left[i] = out[i+left.size()];
	}

	cout << "my left: " << left << endl;
	cout << "my right: " << right << endl;

#if 0	// �������ǣ���������ֽ��� ����ѧ����
	left2 = CDESBuf32( static_cast<int>( desBuf.to_ullong() ) );
	right2 = CDESBuf32( static_cast<int>( ( desBuf >> 32 ).to_ullong() ));
	bool kkk = left2 == left;
	bool ww = right2 == right;
#endif

	// 16�ֵı任��
	for ( int i=0; i<16; ++i )
	{
		DESRound( i, left, right, keyList[i] );
	}

	// �ϲ����ҡ�
	for( size_t i=0; i<left.size(); ++i )
	{
		out[i] = right[i];
		out[ i+left.size() ] = left[i];
	}

	// ����ٽ���һ�����ʼ�û��� �ǳ�ʼ�û�����任��
	FP( out );
}


// DES ���ܣ� Ҫ�� ���ĳ��ȱ�����8�ı�����64bit���� ������ĵ� pEncBuf ���ȱ�����ڵ��� ԭ�ĳ��ȡ�
// key ������64bit ���ȡ�
bool DES( const void* pPlainText, int plainTextLen, const void* pKey, void *pCipherTextBuf, bool denc )
{
	if ( plainTextLen % 8 != 0 )
	{
		return false;
	}

	CDESBuf64 key( *reinterpret_cast<const u64*>( pKey ) );

	for ( int i=0; i*DES_BUF_SIZE<plainTextLen; ++i )
	{		
		CDESBuf64 tmpIn( *reinterpret_cast<const u64*>(pPlainText) );
		CDESBuf64 tmpOut;

		CDESRoundKeyList keyList;
		KeyGeneration( key, keyList );

		
		if ( denc )
		{
			// ����ǽ��ܣ� ��Ҫ����Կ˳��ߵ�������
			CDESRoundKeyList rKeyList;
			for ( CDESRoundKeyList::reverse_iterator rIt = keyList.rbegin(); rIt != keyList.rend(); ++rIt )
			{
				rKeyList.push_back( *rIt );
			}
			keyList = rKeyList;
		}
		
		DES64( tmpIn, keyList, tmpOut );
		u64 result = tmpOut.to_ullong();	// ת��64bit���֣� ���á���
		memcpy(  reinterpret_cast<unsigned char*>( pCipherTextBuf)  + i*DES_BUF_SIZE, &result, DES_BUF_SIZE );
	}
	return true;
}

// ���ַ������м��ܡ� �������8�ı����� �Զ���0.
// �����Կ���Ȳ���8�� ��ô�Զ���0 ��8. ����8�ĳ��Ȼᱻ���ԡ�
void DES( CDESBuf& plainText, CDESBuf& key, CDESBuf& cipherTextBuf, bool denc )
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
	DES( plainText.data(), plainText.size(), key.data(), cipherTextBuf.data(), denc );
}

void Hex2BitBuf( const string& hexStr, CDESBuf64& buf )
{
	for ( size_t i=0; i<hexStr.size(); i++ )
	{
		unsigned char v = hexStr[i] >= 'a' ? hexStr[i]-( 'a' - 10 ) : hexStr[i]-'0';
		CDESBuf4 tmp( v );
		for ( size_t k=0; k<tmp.size(); ++k )
		{
			buf[ i * 4 + k ] = tmp[3-k];
		}
	}
	
	cout << buf << endl;
}

void Hex2BitBuf( const string& hexStr, CDESBuf& buf )
{
	CDESBuf64 tmp;
	Hex2BitBuf( hexStr, tmp );
	buf.resize( 8 );
	u64 v = tmp.to_ullong();
	memcpy( buf.data(), &v, sizeof( v ) );
}

void des(char *out, const char *in, size_t len, const char *key, int action, int mode);
int main()
{
	// test.

	u64 kkkkk = 1;
	CDESBuf64 kkk( kkkkk );
	
	string plainText = "Hello";
	
	string key = "It's a sicret!";

	CDESBuf plainTextBuf;
	plainTextBuf.assign( plainText.begin(), plainText.end() );


	string hexPlain = "02468aceeca86420";
	string hexKey = "0f1571c947d9e869";

	CDESBuf keyBuf;
	//Hex2BitBuf( hexPlain, plainTextBuf );
	Hex2BitBuf( hexKey, keyBuf );

	plainTextBuf.resize( 8 );

	*( u64* )plainTextBuf.data() = 1;
	*( u64* )keyBuf.data() = 1;

	
	unsigned char szBuf[16] = {0};
	des( (char*)szBuf, (char*)plainTextBuf.data(), plainTextBuf.size(), (char*)keyBuf.data(), 0, 0 );

	

	CDESBuf cipherText;
	DES( plainTextBuf, keyBuf, cipherText, false );

//	char *encStr = cipherText.data();

	char szBack[16] = {0};
	des( szBack, (char*)szBuf, 8, (char*)keyBuf.data(), 1, 0 );

	CDESBuf DecPlain;
	DES( cipherText, keyBuf, DecPlain, true );

	system( "pause" );
	return 0;
}
