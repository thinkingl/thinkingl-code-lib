// CrashingBalloon.cpp : 定义控制台应用程序的入口点。
//

// Base plan:
// Analyse the number to find all the valid combonition.


//#include "stdafx.h"
//#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
//#include <winbase.h>

using namespace std;

// 一个以数为单位的迭代器.如: 2,2,2,3,4,4,5.则迭代器 ++ 时的值变化为: 2,3,4,5.
class NumberIterator
{
public:
	NumberIterator(  vector<int> * numberVector, int nBeginPos = 0 );
	~NumberIterator();



	// 下一个数.
	NumberIterator& operator ++(int);
	// 获取当前迭代器的值.
	int GetData();
	// 获取当前迭代器的位置.
	int GetIndex();

	// 判断是否还能取值.
	bool IsEnd();

private:
	 unsigned int m_curIndex;
	 vector<int> * m_pNumberVector;
};

NumberIterator::NumberIterator(  std::vector<int> *numberVector, int nBeginPos )
{
	m_pNumberVector = numberVector;
	m_curIndex = nBeginPos;
}

NumberIterator::~NumberIterator()
{
}

NumberIterator& NumberIterator::operator ++(int)
{
	size_t vectorSize = m_pNumberVector->size();

	if( m_curIndex < vectorSize -1 )
	{
		int oldValue = m_pNumberVector->at( m_curIndex );
		m_curIndex ++;
		while( m_curIndex < vectorSize )
		{			
			int newValue = m_pNumberVector->at( m_curIndex );
			if( oldValue != newValue )
			{
				break;
			}
			else
			{
				m_curIndex ++;
			}			
		}
	}
	else
	{
		m_curIndex = vectorSize;
	}
	


	return *this;
}

int NumberIterator::GetData()
{
	return m_pNumberVector->at( m_curIndex );
}

int NumberIterator::GetIndex()
{
	return this->m_curIndex;
}

bool NumberIterator::IsEnd()
{
	return m_curIndex == this->m_pNumberVector->size() ;
}



class CrashingBalloonUmpire
{
public:
	CrashingBalloonUmpire( int nScore);
	~CrashingBalloonUmpire();

	// 两个人比较分数,看谁赢.
	bool operator > ( CrashingBalloonUmpire& another );

	// 获取原始值.
	int GetScore();

private:
	// 原始值.
	int m_nScore;

	// 得分是否合法.
	bool m_bIsValid;

	// 得分拆解成的素数向量.
	vector<int> m_primeVector;

	// 所有的可能性组合.
	vector< vector<int> > m_all;

	// 将得分拆解成素数队列的乘积形式..
	bool AnalyseScore( const int& nScore, vector<int> &primeVector );

	// 根据素数数组,寻找所有的组合.
	void Search( vector<int> &primeVector, vector< vector<int> > &allPossibility );

	// 用于递归求值.不一定是素数数组了.
	void Search( vector<int> &numberVector, vector< vector<int> > &allPossibility,const int nbeginPos );

	// 检查一个数在在指定数组的指定位置之前是否出现过.
	bool CheckExist( int aimNumber,const vector<int>& numberVector, int Pos );

	// 清理一个数组,将重复的去掉,并且排好序.
	void Filtrate( vector< vector<int> >& resultVector );

	

};

CrashingBalloonUmpire::CrashingBalloonUmpire( int nScore )
{
	m_nScore = nScore;
	m_bIsValid = this->AnalyseScore( nScore, m_primeVector );

	if( m_bIsValid )
	{
		this->Search( m_primeVector, this->m_all );
		if( this->m_all.size() > 0 )
		{
			this->Filtrate( this->m_all );
		}
		else
		{
			this->m_bIsValid = false;
		}
		
	}
}

CrashingBalloonUmpire::~CrashingBalloonUmpire()
{
}

bool CrashingBalloonUmpire::AnalyseScore( const int& nScore, vector<int> &primeVector )
{
	// Parse the score to prime number.
	int nScoreTemp= nScore;
	int maxTryNum = min( 100, nScore+1 );
	for( int i=2;( i< maxTryNum )&&( nScoreTemp != 1) ; i++ )
	{
		while( nScoreTemp%i == 0 )
		{
			nScoreTemp = nScoreTemp/i;

			// Record i.
			primeVector.push_back( i );
		}
	}

	if( nScoreTemp != 1 )
	{
		return false;
	}
	return true;
}

void CrashingBalloonUmpire::Search( vector<int> &primeVector, vector< vector<int> >& allPossibility )
{
	//for( NumberIterator i( &primeVector ); i.IsEnd() == false; i++ )	// 大家轮流做庄..
	//{
		Search( primeVector, allPossibility, 0 );

		// 对结果排序,过滤重复的.
		int kklsf = 10;
	/*}*/
}

void CrashingBalloonUmpire::Search( vector<int> &numberVector, vector< vector<int> >& allPossibility, const int beginPos )
{
	// 先看看这个坐庄的数,能不能不变.(也就是看看在前面出没出现过.)
	int curPosNumber = numberVector.at( beginPos );
	if( this->CheckExist( curPosNumber, numberVector, beginPos ) )
	{
		// 存在..不能不变.
		// 检查是不是已经到了最后,没有机会了.
		if( beginPos == numberVector.size() -1 )
		{
			// 已经到了最后,没有机会了..
			return;
		}
		else
		{
			// 还有后面的数,还有机会..
			NumberIterator numberIte( &numberVector, beginPos + 1 );
			for( ; numberIte.IsEnd() == false ; numberIte++ )
			{
				// 用beginPos 上的这个数与 迭代器中的这个数相乘,得到新的队列.
				int multiplyResult = numberVector.at(beginPos) * numberVector.at( numberIte.GetIndex() );
				if( multiplyResult <= 100 )
				{
					// 申请一个新的队列空间,并给队列赋值.
					vector<int> *newVector = new vector<int>;
					for( size_t i=0 ; i<numberVector.size(); i++ )
					{
						if( i== beginPos )
						{
							newVector->push_back( multiplyResult );
						}
						else if( i != numberIte.GetIndex() )
						{
							newVector->push_back( numberVector.at( i ) );
						}
					}

					// 寻找这个新的队列.
					Search( *newVector, allPossibility, beginPos );

					// 清理内存.
					delete newVector;
				}
				
			}
		}


	}
	else
	{
		// 在这里判断一下是不是坚持到了最后..
		if( beginPos == numberVector.size() -1 )
		{
			// 入栈..你Pass了.
			allPossibility.push_back( numberVector );
			return;
		}

		// 不存在,可以不变.
		// 不变,尝试下一个数..		
		int newBeginPos = beginPos + 1;

		Search( numberVector, allPossibility, newBeginPos );

		// 然后尝试变的..
		NumberIterator numberIte( &numberVector, beginPos + 1 );
		for( ; numberIte.IsEnd() == false ; numberIte++ )
		{
			// 用beginPos 上的这个数与 迭代器中的这个数相乘,得到新的队列.
			int multiplyResult = numberVector.at(beginPos) * numberVector.at( numberIte.GetIndex() );
			if( multiplyResult <= 100 )
			{
				// 申请一个新的队列空间,并给队列赋值.
				vector<int> *newVector = new vector<int>;
				for( size_t i=0 ; i<numberVector.size(); i++ )
				{
					if( i== beginPos )
					{
						newVector->push_back( multiplyResult );
					}
					else if( i != numberIte.GetIndex() )
					{
						newVector->push_back( numberVector.at( i ) );
					}
				}

				// 寻找这个新的队列.
				Search( *newVector, allPossibility, beginPos );

				// 清理内存.
				delete newVector;
			}
			
		}
	}


	//// 解脱的条件.
	//if( numberVector.size() == 1 )
	//{
	//	return;
	//}

}

bool CrashingBalloonUmpire::CheckExist( int aimNumber, const vector<int>& numberVector, int pos )
{
	bool result = false;
	for( int i=0; i<pos; i++ )
	{
		if( numberVector.at(i) == aimNumber )
		{
			result = true;
			break;
		}
	}
	return result;
}

void CrashingBalloonUmpire::Filtrate(std::vector< vector<int> > &resultVector)
{
	// 先排序.
	vector< vector<int> >::iterator ite ;
	for( ite = resultVector.begin(); ite != resultVector.end(); ite++ )
	{
		sort( (*ite).begin(), (*ite).end() );
	}

	// 去重.
	for( ite = resultVector.begin()+1; ite != resultVector.end(); ite ++ )
	{
		vector< vector<int> >::iterator compareCursor = resultVector.begin();
		for( ; compareCursor != ite; compareCursor ++ )
		{
			// 比较是否相等.
			if( (*ite) == (*compareCursor) )
			{
				// 相等,就把这个重复的项删调.
				ite = resultVector.erase( ite );
				ite --;
				break;
			}			
		}
	}
}

// Get the winner.
bool CrashingBalloonUmpire::operator > ( CrashingBalloonUmpire& anotherPlayer )
{
	if( this->m_bIsValid )
	{
		if( anotherPlayer.m_bIsValid )
		{
			// all tell true.
			// 从this的结果中取出一个队列,然后和another中的每个队列都比较一下有没有共用的数,如果有则这个队列不符合结果.
			// 如果找不出来,就是个符合结果的队列.
			bool bHasDifferentQueue = false;	// 开始默认没有没有重复的行.

			// 遍历自己的每一行.
			for( size_t myResult = 0; myResult < this->m_all.size(); myResult++ )	
			{
				// 与对方的每一行比较.
				size_t anotherResult = 0;
				for(  ; anotherResult < anotherPlayer.m_all.size(); anotherResult ++ )	
				{	
					bool bIsDifferent = true;	// 假设这一行是不同的.

					// 从自己比较的这一行中取出一个数字与对方的一行比较.
					for( size_t oneNumberIndex = 0; oneNumberIndex < this->m_all[myResult].size(); oneNumberIndex ++ ) 
					{
						// 在this的结果中取出一个数,和another结果中所有的结果都要比一遍.
						int oneNumber = this->m_all[ myResult][oneNumberIndex];

						// 对方的一行中的数字.
						for( size_t anotherNumberIndex = 0; anotherNumberIndex < anotherPlayer.m_all.at( anotherResult ).size() ; anotherNumberIndex ++ )
						{
							if( anotherPlayer.m_all[anotherResult][ anotherNumberIndex ] == oneNumber )
							{
								// 相等了.这个队列在another上的这一行上不行了.
								bIsDifferent = false;
								break;
							}

							// 剪枝.因为队列的存储是排序的.
							if( anotherPlayer.m_all[anotherResult][anotherNumberIndex] > oneNumber )
							{
								break;
							}
						}

						// 如果已经检测到相同的项,就不用再继续处理了.
						if( !bIsDifferent )
							break;
					}	
					// 遍历完这一行,看看这一行到底行不行.
					if( bIsDifferent )
					{
						// 这一行行.和其他的行没有相同的.
						bHasDifferentQueue = true;
						break;
					}
				}
				
				if( bHasDifferentQueue )	// Challenge fail.
				{
					// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22222222
					// Test:
					// 将不同的输出
					cout << "This Queue:";
					for( int i=0; i<this->m_all[myResult].size(); i++ )
					{
						cout << this->m_all[myResult][i];
						cout << "\t";
					}
					cout << endl;

					for( int i=0; i<anotherPlayer.m_all[anotherResult].size(); i++ )
					{
						cout << anotherPlayer.m_all[anotherResult][i];
						cout << "\t";
					}
					cout << endl;

					//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2
					return this->m_nScore > anotherPlayer.m_nScore;
				}				
			}			

			// Challenge success.who get low score win.

			// @@@@@@@@@@@@@@@@@@@@@@@@22
			cout << "Can't find a zuhe.." << endl;
			// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			return this->m_nScore < anotherPlayer.m_nScore;
		}
		else
		{
			// @@@@@@@@@@@@@@@@@@@@2

			cout << "第一个合法,第二个非法" << endl;
			// @@@@@@@@@@@@@@@@@@@@@@@2
			return true;
		}
	}
	else
	{
		// I have invalid score.
		if( anotherPlayer.m_bIsValid )
		{
			// @@@@@@@@@@@@@@@@@@@@2

			cout << "第一个非法,第二个合法" << endl;
			// @@@@@@@@@@@@@@@@@@@@@@@2
			return false;
		}
		else
		{
			// @@@@@@@@@@@@@@@@@@@@2

			cout << "两个都是非法的" << endl;
			// @@@@@@@@@@@@@@@@@@@@@@@2
			return this->m_nScore > anotherPlayer.m_nScore;
		}
	}
	return false;
}


// Search a number in ordered number queue by dichotomy.
// if success ,return the index of the number. Return -1 when the number is not in the queue.
int DichotomySearch( const vector<int> & orderedQueue, const int &aimNumber )
{
	int begin = 0;
	int end = static_cast<int>(orderedQueue.size());
	int index =  (begin+end)/2;

	bool bIsSearched = false;
	
	while(  end != begin )
	{
		int tempNumber = orderedQueue.at( index );
		if( tempNumber == aimNumber)
		{
			bIsSearched = true;
			break;
		}

		if( tempNumber > aimNumber)
		{
			end = (end + begin)/2 -1;
		}
		else if( tempNumber < aimNumber)
		{
			begin = ( end + begin )/2 +1;
		}

		index = (begin + end)/2;
	}
	if( bIsSearched )
	{
		return index;
	}
	else
	{
		return -1;
	}
}

int CrashingBalloonUmpire::GetScore()
{
	return this->m_nScore;
}

struct Game
{
	int highScore;
	int lowScore;
};

int main()
{
//	vector< Game > input;
	int winnerScore, loserScore;
//	int time = 0;
	//while( cin >> winnerScore >> loserScore )
	//{	
	//	//cout << winnerScore+loserScore << endl;
	//	CrashingBalloonUmpire * highScore = new CrashingBalloonUmpire( winnerScore ); 
	//	CrashingBalloonUmpire *lowScore = new CrashingBalloonUmpire( loserScore ); 
	//	if( (*highScore) > (*lowScore) )
	//	{
	//		cout << highScore->GetScore() << endl;
	//	}
	//	else
	//	{
	//		cout << lowScore->GetScore() << endl;
	//	}

	//	delete highScore;
	//	delete lowScore;
	//}

	 // 测试
	for( winnerScore = 2304; true; winnerScore ++ )
	{
		for( loserScore = 2; loserScore < winnerScore; loserScore ++ )
		{
			if( loserScore != winnerScore )
			{
				CrashingBalloonUmpire * highScore = new CrashingBalloonUmpire( winnerScore ); 
				CrashingBalloonUmpire *lowScore = new CrashingBalloonUmpire( loserScore ); 

				/*if( ( (*highScore) > (*lowScore) ) != (winnerScore > loserScore) )
				{*/
				cout << "One : " << winnerScore << "\t" << loserScore << endl;
				/*}*/

				if( (*highScore) > (*lowScore) )
				{
					cout << highScore->GetScore() << endl;
				}
				else
				{
					cout << lowScore->GetScore() << endl;
				}

				delete highScore;
				delete lowScore;
			}
		}
	}
	
	//for( size_t i=0 ; i<input.size(); i++ )
	//{
	//	CrashingBalloonUmpire highScore( (input.at(i)).highScore ); 
	//	CrashingBalloonUmpire lowScore( ((input.at(i))).lowScore ); 
	//	if( highScore > lowScore )
	//	{
	//		cout << (input.at(i)).highScore << endl;
	//	}
	//	else
	//	{
	//		cout << (input.at(i)).lowScore << endl;
	//	}
	//}
		
	//while(true)
	//{
	//	string kkkk;
	//	cin >> kkkk;
	//}
	//

//	cin >> winnerScore;
	return 0;
}

