// CrashingBalloon.cpp : �������̨Ӧ�ó������ڵ㡣
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

// һ������Ϊ��λ�ĵ�����.��: 2,2,2,3,4,4,5.������� ++ ʱ��ֵ�仯Ϊ: 2,3,4,5.
class NumberIterator
{
public:
	NumberIterator(  vector<int> * numberVector, int nBeginPos = 0 );
	~NumberIterator();



	// ��һ����.
	NumberIterator& operator ++(int);
	// ��ȡ��ǰ��������ֵ.
	int GetData();
	// ��ȡ��ǰ��������λ��.
	int GetIndex();

	// �ж��Ƿ���ȡֵ.
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

	// �����˱ȽϷ���,��˭Ӯ.
	bool operator > ( CrashingBalloonUmpire& another );

	// ��ȡԭʼֵ.
	int GetScore();

private:
	// ԭʼֵ.
	int m_nScore;

	// �÷��Ƿ�Ϸ�.
	bool m_bIsValid;

	// �÷ֲ��ɵ���������.
	vector<int> m_primeVector;

	// ���еĿ��������.
	vector< vector<int> > m_all;

	// ���÷ֲ����������еĳ˻���ʽ..
	bool AnalyseScore( const int& nScore, vector<int> &primeVector );

	// ������������,Ѱ�����е����.
	void Search( vector<int> &primeVector, vector< vector<int> > &allPossibility );

	// ���ڵݹ���ֵ.��һ��������������.
	void Search( vector<int> &numberVector, vector< vector<int> > &allPossibility,const int nbeginPos );

	// ���һ��������ָ�������ָ��λ��֮ǰ�Ƿ���ֹ�.
	bool CheckExist( int aimNumber,const vector<int>& numberVector, int Pos );

	// ����һ������,���ظ���ȥ��,�����ź���.
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
	//for( NumberIterator i( &primeVector ); i.IsEnd() == false; i++ )	// ���������ׯ..
	//{
		Search( primeVector, allPossibility, 0 );

		// �Խ������,�����ظ���.
		int kklsf = 10;
	/*}*/
}

void CrashingBalloonUmpire::Search( vector<int> &numberVector, vector< vector<int> >& allPossibility, const int beginPos )
{
	// �ȿ��������ׯ����,�ܲ��ܲ���.(Ҳ���ǿ�����ǰ���û���ֹ�.)
	int curPosNumber = numberVector.at( beginPos );
	if( this->CheckExist( curPosNumber, numberVector, beginPos ) )
	{
		// ����..���ܲ���.
		// ����ǲ����Ѿ��������,û�л�����.
		if( beginPos == numberVector.size() -1 )
		{
			// �Ѿ��������,û�л�����..
			return;
		}
		else
		{
			// ���к������,���л���..
			NumberIterator numberIte( &numberVector, beginPos + 1 );
			for( ; numberIte.IsEnd() == false ; numberIte++ )
			{
				// ��beginPos �ϵ�������� �������е���������,�õ��µĶ���.
				int multiplyResult = numberVector.at(beginPos) * numberVector.at( numberIte.GetIndex() );
				if( multiplyResult <= 100 )
				{
					// ����һ���µĶ��пռ�,�������и�ֵ.
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

					// Ѱ������µĶ���.
					Search( *newVector, allPossibility, beginPos );

					// �����ڴ�.
					delete newVector;
				}
				
			}
		}


	}
	else
	{
		// �������ж�һ���ǲ��Ǽ�ֵ������..
		if( beginPos == numberVector.size() -1 )
		{
			// ��ջ..��Pass��.
			allPossibility.push_back( numberVector );
			return;
		}

		// ������,���Բ���.
		// ����,������һ����..		
		int newBeginPos = beginPos + 1;

		Search( numberVector, allPossibility, newBeginPos );

		// Ȼ���Ա��..
		NumberIterator numberIte( &numberVector, beginPos + 1 );
		for( ; numberIte.IsEnd() == false ; numberIte++ )
		{
			// ��beginPos �ϵ�������� �������е���������,�õ��µĶ���.
			int multiplyResult = numberVector.at(beginPos) * numberVector.at( numberIte.GetIndex() );
			if( multiplyResult <= 100 )
			{
				// ����һ���µĶ��пռ�,�������и�ֵ.
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

				// Ѱ������µĶ���.
				Search( *newVector, allPossibility, beginPos );

				// �����ڴ�.
				delete newVector;
			}
			
		}
	}


	//// ���ѵ�����.
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
	// ������.
	vector< vector<int> >::iterator ite ;
	for( ite = resultVector.begin(); ite != resultVector.end(); ite++ )
	{
		sort( (*ite).begin(), (*ite).end() );
	}

	// ȥ��.
	for( ite = resultVector.begin()+1; ite != resultVector.end(); ite ++ )
	{
		vector< vector<int> >::iterator compareCursor = resultVector.begin();
		for( ; compareCursor != ite; compareCursor ++ )
		{
			// �Ƚ��Ƿ����.
			if( (*ite) == (*compareCursor) )
			{
				// ���,�Ͱ�����ظ�����ɾ��.
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
			// ��this�Ľ����ȡ��һ������,Ȼ���another�е�ÿ�����ж��Ƚ�һ����û�й��õ���,�������������в����Ͻ��.
			// ����Ҳ�����,���Ǹ����Ͻ���Ķ���.
			bool bHasDifferentQueue = false;	// ��ʼĬ��û��û���ظ�����.

			// �����Լ���ÿһ��.
			for( size_t myResult = 0; myResult < this->m_all.size(); myResult++ )	
			{
				// ��Է���ÿһ�бȽ�.
				size_t anotherResult = 0;
				for(  ; anotherResult < anotherPlayer.m_all.size(); anotherResult ++ )	
				{	
					bool bIsDifferent = true;	// ������һ���ǲ�ͬ��.

					// ���Լ��Ƚϵ���һ����ȡ��һ��������Է���һ�бȽ�.
					for( size_t oneNumberIndex = 0; oneNumberIndex < this->m_all[myResult].size(); oneNumberIndex ++ ) 
					{
						// ��this�Ľ����ȡ��һ����,��another��������еĽ����Ҫ��һ��.
						int oneNumber = this->m_all[ myResult][oneNumberIndex];

						// �Է���һ���е�����.
						for( size_t anotherNumberIndex = 0; anotherNumberIndex < anotherPlayer.m_all.at( anotherResult ).size() ; anotherNumberIndex ++ )
						{
							if( anotherPlayer.m_all[anotherResult][ anotherNumberIndex ] == oneNumber )
							{
								// �����.���������another�ϵ���һ���ϲ�����.
								bIsDifferent = false;
								break;
							}

							// ��֦.��Ϊ���еĴ洢�������.
							if( anotherPlayer.m_all[anotherResult][anotherNumberIndex] > oneNumber )
							{
								break;
							}
						}

						// ����Ѿ���⵽��ͬ����,�Ͳ����ټ���������.
						if( !bIsDifferent )
							break;
					}	
					// ��������һ��,������һ�е����в���.
					if( bIsDifferent )
					{
						// ��һ����.����������û����ͬ��.
						bHasDifferentQueue = true;
						break;
					}
				}
				
				if( bHasDifferentQueue )	// Challenge fail.
				{
					// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22222222
					// Test:
					// ����ͬ�����
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

			cout << "��һ���Ϸ�,�ڶ����Ƿ�" << endl;
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

			cout << "��һ���Ƿ�,�ڶ����Ϸ�" << endl;
			// @@@@@@@@@@@@@@@@@@@@@@@2
			return false;
		}
		else
		{
			// @@@@@@@@@@@@@@@@@@@@2

			cout << "�������ǷǷ���" << endl;
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

	 // ����
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

