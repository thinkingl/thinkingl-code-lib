/*
ID: thinkin6
PROG: shopping
LANG: C++
*/

/** USA CO 3.3.3 :
Shopping Offers
IOI'95 In a certain shop, each kind of product has an integer price. For example, the price of a flower is 2 zorkmids (z) and the price of a vase is 5z. In order to attract more customers, the shop introduces some special offers. 
A special offer consists of one or more product items together for a reduced price, also an integer. Examples: 
three flowers for 5z instead of 6z, or two vases together with one flower for 10z instead of 12z. Write a program that calculates the price a customer has to pay for a purchase, making optimal use of the special offers to make the price as low as possible. You are not allowed to add items, even if that would lower the price. 
For the prices and offers given above, the (lowest) price for three flowers and two vases is 14z: two vases and one flower for the reduced price of 10z and two flowers for the regular price of 4z. 
PROGRAM NAME: shopping
INPUT FORMAT
The input file has a set of offers followed by a purchase. Line 1: s, the number of special offers, (0 <= s <= 99). Line 2..s+1: Each line describes an offer using several integers. The first integer is n (1 <= n <= 5), the number of products that are offered. The subsequent n pairs of integers c and k indicate that k items (1 <= k <= 5) with product code c (1 <= c <= 999) are part of the offer. The last number p on the line stands for the reduced price (1 <= p <= 9999). The reduced price of an offer is less than the sum of the regular prices. Line s+2: The first line contains the number b (0 <= b <= 5) of different kinds of products to be purchased. Line s+3..s+b+2: Each of the subsequent b lines contains three values: c, k, and p. The value c is the (unique) product code (1 <= c <= 999). The value k indicates how many items of this product are to be purchased (1 <= k <= 5). The value p is the regular price per item (1 <= p <= 999). At most 5*5=25 items can be in the basket. 
SAMPLE INPUT (file shopping.in) 
2
1 7 3 5
2 7 1 8 2 10
2
7 3 2
8 2 5

OUTPUT FORMAT
A single line with one integer: the lowest possible price to be paid for the purchases. SAMPLE OUTPUT (file shopping.out)
14
*/

/** 
˼·��
*	��̬�滮.
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




#ifdef _WIN32
#include <time.h>
#endif

using namespace std;

#ifdef _WIN32
typedef __int64 n64;
typedef unsigned __int64 u64;
#else
typedef long long n64;
typedef unsigned long long u64;
#endif
typedef unsigned long u32;

#ifdef _WIN32
#define THINKINGL 1
#endif

std::map<int,int> a,b;

const int MAX_PRODUCT_TYPE = 5;

class CProductSet
{
public:
	struct TProductInfo
	{
		int m_nId;
		int m_nNum;
		int m_nPrice;

		TProductInfo(){ memset( this, 0 , sizeof( TProductInfo ) ); };

		bool operator <( const TProductInfo& another ) const
		{
			if ( m_nId == another.m_nId )
			{
				return m_nNum < another.m_nNum;
			}
			else
			{
				return m_nId < another.m_nId;
			}
		}
	};
public:
	CProductSet();
	void AddProduct( int nProductId, int nProductNum, int nPrice = 0 );
	/** �ײͼ�. */
	int GetSpecialPrice() const;
	/** �����ײͼ�. */
	void SetSpecialPrice( int nPrice );
	/** ֱ�����Ǯ. */
	int GetNormalPrice() const;
	/** �Ƿ�����Ч��. */
	bool IsValid()const;
	/** �Ƿ�û����Ʒ��. */
	bool IsNull()const;
	bool operator <( const CProductSet& another ) const;
	const CProductSet operator -( const CProductSet& another )const;
	CProductSet& operator -=( const CProductSet& another )const;
private:
	/** �ײͼ۸�. */
	int m_nSpecialPrice;
	/** ������Ʒ. */
	typedef std::vector< TProductInfo > TProductTable;
	TProductTable m_tProductTable;
};

int CProductSet::GetSpecialPrice()const
{
	return m_nSpecialPrice;
}

bool CProductSet::IsValid()const
{
//	int nTotal = 0;
	for ( TProductTable::const_iterator citer = m_tProductTable.begin(); citer != m_tProductTable.end(); ++citer  )
	{
		if ( citer->m_nNum < 0 )
		{
			return false;
		}
//		nTotal += citer->second.m_nNum;
	}
	return true;
}

bool CProductSet::IsNull()const
{
	for ( TProductTable::const_iterator citer = m_tProductTable.begin(); citer != m_tProductTable.end(); ++citer )
	{
		if ( citer->m_nNum > 0 )
		{
			return false;
		}
	}
	return true;
}

const CProductSet CProductSet::operator -( const CProductSet& another ) const
{
	CProductSet result = *this;
	for ( TProductTable::const_iterator citer = another.m_tProductTable.begin(); citer != another.m_tProductTable.end(); ++citer )
	{
		result.m_tProductTable[ citer->m_nId ].m_nNum -= citer->m_nNum;
	}

	return result;
}

int CProductSet::GetNormalPrice() const
{
	int nPrice = 0;
	for ( TProductTable::const_iterator citer = m_tProductTable.begin(); citer != m_tProductTable.end(); ++citer )
	{
		nPrice += citer->m_nNum * citer->m_nPrice;
	}
	return nPrice;
}

void CProductSet::SetSpecialPrice( int nPrice )
{
	m_nSpecialPrice = nPrice;
}

void CProductSet::AddProduct(int nProductId, int nProductNum, int nPrice /*= 0*/)
{
	TProductInfo tPro;
	tPro.m_nId = nProductId;
	tPro.m_nNum = nProductNum;
	tPro.m_nPrice = nPrice;
	this->m_tProductTable[ nProductId ] = tPro;
}

CProductSet::CProductSet()
{
	this->m_nSpecialPrice = 0;
	m_tProductTable.resize( MAX_PRODUCT_TYPE );
}

bool CProductSet::operator <( const CProductSet& another )const
{
	return this->m_tProductTable < another.m_tProductTable;
}


typedef std::vector< CProductSet > TProductSetList;
typedef std::pair<int,CProductSet> TSpecialOfferNum_ProductSetPair;

/** ȥ���õ�map. �ײͺ���Ʒ���<->Ǯ��. */
typedef std::map< TSpecialOfferNum_ProductSetPair, int > TMoneyMap;


/** ��ȡ�� nSpeOfferNum ���ײ�,���� tProductSet ��Щ����,��Ҫ�����ٵ�Ǯ��. 
*	���õݹ�ķ�ʽ,ʹ���Լ���ջ,��ʹ�ú������÷�ʽ.
*	�������ַ�ʽ���У��ڴ�ʹ��̫�࣬ʱ��Ҳ̫�ࡣ
*/
int GetMinmumMoney( int nSpeOfferNum, CProductSet tProductSet, 
				   TMoneyMap& tMoneyMap,  const TProductSetList& tSpecialOfferList )
{
	cout << "GetMinmumMoney1" << endl;

	TMoneyMap tFlagMap;

	TSpecialOfferNum_ProductSetPair tPairFinal( nSpeOfferNum, tProductSet );

	typedef std::stack< TSpecialOfferNum_ProductSetPair > TWorkStack;

	TWorkStack tWordStack;
	tWordStack.push( tPairFinal );

	cout << "GetMinmumMoney2" << endl;

	while ( !tWordStack.empty() )
	{
		// ȡ��һ��.
		TSpecialOfferNum_ProductSetPair tPair = tWordStack.top();
		

		// ����.
		// ��̬�滮.
		if( tFlagMap[ tPair ] )
		{
			// �Ѿ����˲�����.
		}
		else
		{
			// ���ջ���������.
			if( tPair.first == 0 )
			{
				tMoneyMap[ tPair ] = tPair.second.GetNormalPrice();
				tFlagMap[ tPair ] = 1;
				tWordStack.pop();// �Ѿ��������Ǯ�ĳ�ջ.
				continue;	// ��������Ĵ���.
			}


			// ���,��ջ.
			// ���һ���ײ͵�ʹ�����������,һ���ǲ���,�������һ���ײ�,��������. ��һ����ʹ�����һ���ײ�,��������һ��,ʣ�����Ʒ����Щ�ײ�.
			// �������ȡС��.
			TSpecialOfferNum_ProductSetPair tPairOne( tPair.first - 1, tPair.second );

			int nMoneySecond = 0xFFFFFFF;	// Ĭ��Ϊ���޴�.
			CProductSet productLeft = tPair.second - tSpecialOfferList[tPair.first-1];
			if ( productLeft.IsValid()  )
			{
				TSpecialOfferNum_ProductSetPair tPairTwo( tPair.first, productLeft );

				// ����Ѿ��������,������Ʒ�ǿյ�(��ʱ�ü۸�>0���ж�ȥ�ػ�ʧЧ!).
				if ( tFlagMap[ tPairTwo ] || productLeft.IsNull() )
				{
					nMoneySecond = tMoneyMap[ tPairTwo ] + tSpecialOfferList[ tPair.first-1 ].GetSpecialPrice();
				}
				else
				{
					// ��ջ.
					nMoneySecond = -1;
					tWordStack.push( tPairTwo );
				}

				
			}
			

			if ( tFlagMap[ tPairOne ] && nMoneySecond != -1 )
			{
				tMoneyMap[ tPair ] = min( tMoneyMap[ tPairOne ], nMoneySecond );

				tFlagMap[ tPair ] = 1;
	
				tWordStack.pop(); // ���Ѿ�������ĳ�ջ.
			}
			else
			{
				if ( tMoneyMap[tPairOne] == 0 )
				{
					tWordStack.push( tPairOne );
				}
			}
		}
	}

	return tMoneyMap[ tPairFinal ];

}



int main()
{
	string strProblemName = "shopping";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	// ȡ�Ż��ײ���Ŀ.
	int nOffersNum;
	fin >> nOffersNum;

	typedef std::map< int, int > TProductIdMap;
	TProductIdMap tIdMap;

	cout << "before special offerlist" << endl;
	/** �Ż��ײ�. */
	TProductSetList tSpecialOfferList;

	for( int i=0; i<nOffersNum; ++i )
	{
		CProductSet specialOffer;
		int nProductsNum;
		fin >> nProductsNum;
		for ( int k=0;k<nProductsNum;++k )
		{
			int nProductId;
			int nNum;
			fin >> nProductId >> nNum;
			if ( tIdMap.find( nProductId ) == tIdMap.end() )
			{
				int tmp = tIdMap.size();
				tIdMap[nProductId] = tmp;
			}
				
			nProductId = tIdMap[ nProductId ];
			specialOffer.AddProduct( nProductId, nNum );
		}
		int nPrice;
		fin >> nPrice;
		specialOffer.SetSpecialPrice( nPrice );

		tSpecialOfferList.push_back( specialOffer );
	}

	cout << "before all product" << endl;

	// ��ȡ���е���Ʒ.
	CProductSet tAllPruduct;

	int nAllProductNum;
	fin >> nAllProductNum;
	for ( int i=0; i<nAllProductNum; ++i )
	{
		int nProductId;
		int nProductNum;
		int nProductPrice;
		fin >> nProductId >> nProductNum >> nProductPrice;

		if ( tIdMap.find( nProductId ) == tIdMap.end() )
		{
			int tmp = tIdMap.size();
			tIdMap[nProductId] = tmp;
		}

		nProductId = tIdMap[ nProductId ];

		tAllPruduct.AddProduct( nProductId, nProductNum, nProductPrice );
	}

	TMoneyMap tMoneyMap;
	int nMinimumMoney = GetMinmumMoney( tSpecialOfferList.size(), tAllPruduct, tMoneyMap, tSpecialOfferList );

	fout << nMinimumMoney << endl;

	fin.close();
	fout.close();

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}