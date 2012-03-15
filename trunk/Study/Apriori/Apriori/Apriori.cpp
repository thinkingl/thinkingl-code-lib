// 
// 实现Apriori算法
// 学号： 1110332040
// 姓名： 李之兴
// email： thinkingl@thinkingl.com
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

// 元素。
typedef std::string CItem;

// 元素集合。
typedef std::set< CItem > CItemSet;

// 事务
typedef CItemSet CTransaction;

// 事务集
typedef std::vector< CTransaction > CTransactionSet;

// 候选项集合。
// key: 项集合， value: 计数
typedef std::map< CItemSet, int > CCandidateItemTable;
typedef std::vector< CCandidateItemTable > CCandidateItemTableList;

// 频繁项目集合。
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

// 生成候选集
void CandidateGen( const CFreqItemSet& freqItemSet,  const CTransactionSet& transSet, CCandidateItemTable& candidateSet )
{
	candidateSet.clear();

	// 合并出候选项。
	// 书上的合并算法的想法很古怪， 用代码实现很麻烦， 我这里直接合并成一个候选项， 然后只保留 项数符合要求的候选项。
	// 其实书上的古怪想法并不能减少算法时间和空间复杂度。	
	for ( CFreqItemSet::const_iterator cIt1 = freqItemSet.begin(); cIt1 != freqItemSet.end(); ++cIt1 )
	{
		for ( CFreqItemSet::const_iterator cIt2 = freqItemSet.begin(); cIt2 != freqItemSet.end(); ++cIt2 )
		{
			size_t freqItemNum = cIt1->size();

			CItemSet candidate;
			set_union( cIt1->begin(), cIt1->end(), cIt2->begin(), cIt2->end(), std::inserter( candidate,  candidate.begin() ) );

			// 合并出的candidate项数应该比frequency项数多1.
			if ( candidate.size() == freqItemNum + 1 )
			{
				// 按照书上的说法， 这里要进行剪枝。
				// candidate 里所有的k-1子集 都必须在 频繁项目集合中。
				// 挨个删掉一个元素就是所有k-1子集
				bool isSubset = true;
				CItemSet subSet = candidate;
				for ( CItemSet::iterator itDel = candidate.begin(); itDel != candidate.end(); ++itDel )
				{
					subSet.erase( *itDel );	// 删除，获得子集。

					if ( freqItemSet.find( subSet ) == freqItemSet.end() )
					{
						// 没找到， 剪枝。
						isSubset = false;
						break;
					}

					subSet.insert( *itDel );	// 重新添加进来。
				}

				if ( isSubset )
				{
					candidateSet[ candidate ] = 0;
				}
				
			}
		}
	}
}

// Candidate Itemset 转换为 Frequency ItemSet, 要求必须大于最小支持度 minSup。
void Candidate2Freq( const CCandidateItemTable& candidateTable, float minSup,  int transactionCount, CFreqItemSet& freqSet, CCandidateItemTableList& allCanList )
{
	freqSet.clear();

	CCandidateItemTable validCandi;

	for ( CCandidateItemTable::const_iterator itCan = candidateTable.begin(); itCan != candidateTable.end(); ++itCan )
	{
		// 计算支持。
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

// Apriori 算法 主函数。
void Apriori( const CTransactionSet& transSet, float minSup, float minConf, CCandidateItemTableList& allCanList )
{
	// 根据初始的单项频繁项目集生成候选项集集合。
	CCandidateItemTable candidateSet;
	// 进行初始的Candidate计数。
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

	// 将数据库transaction集合转换为初始的单项频繁项目集。
	CFreqItemSet freqSet;
	Candidate2Freq( candidateSet, minSup, transSet.size(), freqSet, allCanList );
	
	for ( int i=2; !freqSet.empty(); ++i )
	{	
		// F(k-1) -> Ck 
		// 由当前级别的频繁项目集获取下一级的候选项集。
		CandidateGen( freqSet, transSet, candidateSet );

		// 遍历所有的数据事务， 对Candidate进行计数。
		for ( size_t transIndex=0; transIndex<transSet.size(); ++transIndex )
		{
			const CTransaction& trans = transSet[ transIndex ];

			// 看每个候选集项是否在数据中，如果在就计数。
			for ( CCandidateItemTable::iterator itCan = candidateSet.begin(); itCan != candidateSet.end(); ++itCan )
			{
				// 每个小项是否在数据中。
				bool isInclude = std::includes( trans.begin(), trans.end(), itCan->first.begin(), itCan->first.end() );
				if ( isInclude )
				{
					itCan->second++;
				}
			}
		}

		// 将满足条件的Candidate转换为Frequence。
		// 要满足支持计数大于设定值minSup。
		Candidate2Freq( candidateSet, minSup, transSet.size(), freqSet, allCanList );
	}
}



int main(int argc, char* argv[])
{
	cout << "Web mining homework!************" << endl;
	cout << "学号： 1110332040" << endl;
	cout << "姓名： 李之兴" << endl;
	cout << "email： thinkingl@thinkingl.com" << endl;
	cout << "http://www.thinkingl.com" << endl;
	cout << "用法： apriori [数据输入文件名]" << endl;
	cout << "[数据输入文件名]为空使用 默认文件名 apriori.txt" << endl;
	cout << endl;
	cout << "文件格式：" << endl;
	cout << "60  <------最小支持度60%" << endl;
	cout << "80  <------最小置信度80%" << endl;
	cout << "T100: { M O N K E Y }   <--------- 要挖掘的Item必须用 { } 括起来， 并且两边必须用 空格 间隔开。{} 外的字符都是打酱油的，只起修饰作用。" << endl;
	cout << "Item 可以是不带 空格或{}  的任意字符串。" << endl;
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
		cout << "打开输入文件失败。 文件名： " << inFileName;
	}

	ofstream outFile( "apriori_result.txt" );

	// 读取最小支持度和最小置信度。
	int minSup, minConf;
	inFile >> minSup >> minConf;


	// 读取事务集。
	// 简化， 用数据结构代表数据库， 用 STL 的set， 满足 Apriori 算法对字典序的要求。。
	CTransactionSet transSet;
	while( inFile )
	{
		CTransaction tran;
		// 读取一个事务。
		bool tranBegin = false;	// 是否开始了事务。
		while( inFile )
		{
			string item;
			inFile >> item;
			if ( tranBegin )
			{
				if ( item == "}" )
				{
					// 事务结束
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

	// 调用算法, 得到所有的频繁项目集。
	
	CCandidateItemTableList allCanTableList;
	Apriori( transSet, float(minSup)/100, float(minConf)/100, allCanTableList );

	cout << "所有的频繁项目集以及支持度信息：" << endl << endl;
	for ( size_t i=0; i<allCanTableList.size(); ++i )
	{
		for ( CCandidateItemTable::iterator itCan = allCanTableList[i].begin(); itCan!=allCanTableList[i].end(); ++itCan )
		{
			// 打印输出。
			cout << itCan->first << " cout: " << itCan->second << "\t Sup: " << float(itCan->second)/transSet.size() << endl;
		}
	}
	
	// 生成关联规则。

	system( "pause" );

	return 0;
}

