// 
// ʵ��Apriori�㷨
// ѧ�ţ� 1110332040
// ������ ��֮��
// email�� thinkingl@thinkingl.com
// blog: http://www.thinkingl.com
// 

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

// Ԫ�ء�
typedef std::string CItem;

// Ԫ�ؼ��ϡ�
typedef std::set< CItem > CItemSet;

// ����
typedef CItemSet CTransaction;

// ����
typedef std::vector< CTransaction > CTransactionSet;

// ��ѡ��ϡ�
// key: ��ϣ� value: ����
typedef std::map< CItemSet, int > CCandidateItemTable;
typedef std::vector< CCandidateItemTable > CCandidateItemTableList;

// Ƶ����Ŀ���ϡ�
typedef std::set<CItemSet> CFreqItemSet;

ostream& operator << ( ostream& oss, const CItemSet& itemset )
{
	for ( CItemSet::const_iterator cIt = itemset.begin(); cIt != itemset.end(); ++cIt )
	{
		oss << *cIt << " ";
	}
	oss << "\t";
	return oss;
}

// ���ɺ�ѡ��
void CandidateGen( const CFreqItemSet& freqItemSet,  const CTransactionSet& transSet, CCandidateItemTable& candidateSet )
{
	candidateSet.clear();

	// �ϲ�����ѡ�
	// ���ϵĺϲ��㷨���뷨�ܹŹ֣� �ô���ʵ�ֺ��鷳�� ������ֱ�Ӻϲ���һ����ѡ� Ȼ��ֻ���� ��������Ҫ��ĺ�ѡ�
	// ��ʵ���ϵĹŹ��뷨�����ܼ����㷨ʱ��Ϳռ临�Ӷȡ�	
	for ( CFreqItemSet::const_iterator cIt1 = freqItemSet.begin(); cIt1 != freqItemSet.end(); ++cIt1 )
	{
		for ( CFreqItemSet::const_iterator cIt2 = freqItemSet.begin(); cIt2 != freqItemSet.end(); ++cIt2 )
		{
			size_t freqItemNum = cIt1->size();

			CItemSet candidate;
			set_union( cIt1->begin(), cIt1->end(), cIt2->begin(), cIt2->end(), std::inserter( candidate,  candidate.begin() ) );

			// �ϲ�����candidate����Ӧ�ñ�frequency������1.
			if ( candidate.size() == freqItemNum + 1 )
			{
				// �������ϵ�˵���� ����Ҫ���м�֦��
				// candidate �����е�k-1�Ӽ� �������� Ƶ����Ŀ�����С�
				// ����ɾ��һ��Ԫ�ؾ�������k-1�Ӽ�
				bool isSubset = true;
				CItemSet subSet = candidate;
				for ( CItemSet::iterator itDel = candidate.begin(); itDel != candidate.end(); ++itDel )
				{
					subSet.erase( *itDel );	// ɾ��������Ӽ���

					if ( freqItemSet.find( subSet ) == freqItemSet.end() )
					{
						// û�ҵ��� ��֦��
						isSubset = false;
						break;
					}

					subSet.insert( *itDel );	// ������ӽ�����
				}

				if ( isSubset )
				{
					candidateSet[ candidate ] = 0;
				}
				
			}
		}
	}
}

// Candidate Itemset ת��Ϊ Frequency ItemSet, Ҫ����������С֧�ֶ� minSup��
void Candidate2Freq( const CCandidateItemTable& candidateTable, float minSup,  int transactionCount, CFreqItemSet& freqSet, CCandidateItemTableList& allCanList )
{
	freqSet.clear();

	CCandidateItemTable validCandi;

	for ( CCandidateItemTable::const_iterator itCan = candidateTable.begin(); itCan != candidateTable.end(); ++itCan )
	{
		// ����֧�֡�
		float sup = float(itCan->second) / transactionCount;
		if ( sup < minSup )
		{
			continue;
		}
		else
		{
			freqSet.insert( itCan->first );

			validCandi.insert( *itCan );
		}
	}

	allCanList.push_back( validCandi );
}

// Apriori �㷨 ��������
void Apriori( const CTransactionSet& transSet, float minSup, float minConf, CCandidateItemTableList& allCanList )
{
	// ���ݳ�ʼ�ĵ���Ƶ����Ŀ�����ɺ�ѡ����ϡ�
	CCandidateItemTable candidateSet;
	// ���г�ʼ��Candidate������
	for ( size_t i=0; i<transSet.size(); ++i )
	{
		const CTransaction& tran = transSet[i];
		for ( CTransaction::const_iterator cIt = tran.begin(); cIt != tran.end(); ++cIt )
		{
			CItemSet itemSet;
			itemSet.insert( *cIt );
			candidateSet[itemSet]++;
		}
	}

	// �����ݿ�transaction����ת��Ϊ��ʼ�ĵ���Ƶ����Ŀ����
	CFreqItemSet freqSet;
	Candidate2Freq( candidateSet, minSup, transSet.size(), freqSet, allCanList );
	
	for ( int i=2; !freqSet.empty(); ++i )
	{	
		// F(k-1) -> Ck 
		// �ɵ�ǰ�����Ƶ����Ŀ����ȡ��һ���ĺ�ѡ���
		CandidateGen( freqSet, transSet, candidateSet );

		// �������е��������� ��Candidate���м�����
		for ( size_t transIndex=0; transIndex<transSet.size(); ++transIndex )
		{
			const CTransaction& trans = transSet[ transIndex ];

			// ��ÿ����ѡ�����Ƿ��������У�����ھͼ�����
			for ( CCandidateItemTable::iterator itCan = candidateSet.begin(); itCan != candidateSet.end(); ++itCan )
			{
				// ÿ��С���Ƿ��������С�
				bool isInclude = std::includes( trans.begin(), trans.end(), itCan->first.begin(), itCan->first.end() );
				if ( isInclude )
				{
					itCan->second++;
				}
			}
		}

		// ������������Candidateת��ΪFrequence��
		// Ҫ����֧�ּ��������趨ֵminSup��
		Candidate2Freq( candidateSet, minSup, transSet.size(), freqSet, allCanList );
	}
}



int main(int argc, char* argv[])
{
	cout << "Web mining homework!************" << endl;
	cout << "ѧ�ţ� 1110332040" << endl;
	cout << "������ ��֮��" << endl;
	cout << "email�� thinkingl@thinkingl.com" << endl;
	cout << "http://www.thinkingl.com" << endl;
	cout << "�÷��� apriori [���������ļ���]" << endl;
	cout << "[���������ļ���]Ϊ��ʹ�� Ĭ���ļ��� apriori.txt" << endl;
	cout << endl;
	cout << "�ļ���ʽ��" << endl;
	cout << "60  <------��С֧�ֶ�60%" << endl;
	cout << "80  <------��С���Ŷ�80%" << endl;
	cout << "T100: { M O N K E Y }   <--------- Ҫ�ھ��Item������ { } �������� �������߱����� �ո� �������{} ����ַ����Ǵ��͵ģ�ֻ���������á�" << endl;
	cout << "Item �����ǲ��� �ո��{}  �������ַ�����" << endl;
	cout << endl << endl;

	string inFileName;
	if ( argc >= 2 && argv[1] != 0  )
	{
		inFileName = argv[1];	
	}
	
	if ( inFileName.empty() )
	{
		inFileName = "apriori.txt";
	}

	ifstream inFile( inFileName );
	if ( !inFile )
	{
		cout << "�������ļ�ʧ�ܡ� �ļ����� " << inFileName;
	}

	ofstream outFile( "apriori_result.txt" );

	// ��ȡ��С֧�ֶȺ���С���Ŷȡ�
	int minSup, minConf;
	inFile >> minSup >> minConf;


	// ��ȡ���񼯡�
	// �򻯣� �����ݽṹ�������ݿ⣬ �� STL ��set�� ���� Apriori �㷨���ֵ����Ҫ�󡣡�
	CTransactionSet transSet;
	while( inFile )
	{
		CTransaction tran;
		// ��ȡһ������
		bool tranBegin = false;	// �Ƿ�ʼ������
		while( inFile )
		{
			string item;
			inFile >> item;
			if ( tranBegin )
			{
				if ( item == "}" )
				{
					// �������
					break;
				}
				else
				{
					tran.insert( item );
				}
			}
			else
			{
				if ( item == "{" )
				{
					tranBegin = true;
				}
			}
		}

		if ( !tran.empty() )
		{
			transSet.push_back( tran );
		}		
	}

	// �����㷨, �õ����е�Ƶ����Ŀ����
	
	CCandidateItemTableList allCanTableList;
	Apriori( transSet, float(minSup)/100, float(minConf)/100, allCanTableList );

	cout << "���е�Ƶ����Ŀ���Լ�֧�ֶ���Ϣ��" << endl << endl;
	for ( size_t i=0; i<allCanTableList.size(); ++i )
	{
		for ( CCandidateItemTable::iterator itCan = allCanTableList[i].begin(); itCan!=allCanTableList[i].end(); ++itCan )
		{
			// ��ӡ�����
			cout << itCan->first << " cout: " << itCan->second << "\t Sup: " << float(itCan->second)/transSet.size() << endl;
		}
	}
	
	// ���ɹ�������

	system( "pause" );

	return 0;
}

