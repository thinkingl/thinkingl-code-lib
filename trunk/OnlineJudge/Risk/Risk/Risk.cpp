// Risk.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <set>

using namespace std;

typedef vector<int> Path;
typedef vector< vector<int> > Matrix;

class InputParser
{
public:
	bool ParseInputLine( vector<int>& output );
	bool ParseInputLine( string input, vector<int>& output );
protected:
private:
};

class SignOfNode
{
public:
	SignOfNode();
	~SignOfNode();
	
	int m_comeNode;
	int m_distance;
protected:
private:
};


SignOfNode::SignOfNode()
{
	m_distance = (unsigned int)( -1 )/4;
	m_comeNode = -1;
}

SignOfNode::~SignOfNode()
{

}

class GraphNoWeight
{
public:
	GraphNoWeight( );
	~GraphNoWeight();

	GraphNoWeight( int size );

	Path GetDijkstraMinPath( int begin, int end );

	void AddRelation( vector<int> relation );

	void Clear();

	void CompleteInit();

	void Reset();

protected:


private:
	vector< vector<int> > m_graph;

	vector<int> m_Pnodes;
	set<int> m_Tnodes;

	vector<SignOfNode> m_signArray;

	vector<bool> m_isPnode;
};

int main()
{
	InputParser input;
	vector<int> lineData;
	int curCase = 1;
	GraphNoWeight * worldMap = new GraphNoWeight();
	while ( input.ParseInputLine( lineData ) )
	{
		if ( lineData.size() == lineData[0] + 1 )
		{
			// 这是正常的数据..
			worldMap->AddRelation( lineData );
		}
		else
		{
			// 开始是题目了.
			worldMap->CompleteInit();
			cout << "Test Set #" << curCase << endl;

			for ( int i=0; i<lineData[0]; i++ )
			{
				int begin, end;
				cin>> begin >> end;

				int sdf = 23;
				// 得到结果.
				Path shortestPath = worldMap->GetDijkstraMinPath( begin-1, end-1 );



				// 输出.
				// 1 to 20: 7
				int distance = shortestPath.size() - 1;
				if ( distance < 0 )
				{
					distance = 0;
				}
				cout << begin << " to " << end << ":" << distance << endl;

//#ifdef _DEBUG
//				cout << "xiangxi: " << endl;
//				for ( int kkk =0; kkk<shortestPath.size(); kkk ++ )
//				{
//					cout << shortestPath[kkk] + 1 << "to: ";
//				}
//				cout << endl;
//#endif

				// reset.
				worldMap->Reset();

			}

			// next.
			curCase ++;
			//worldMap->Clear();
			delete worldMap;
			worldMap = new GraphNoWeight();
		}
	}

	return 0;
}

void GraphNoWeight::Reset()
{
	/*vector<int> m_Pnodes;
	set<int> m_Tnodes;

	vector<SignOfNode> m_signArray;

	vector<bool> m_isPnode;*/
	this->m_Pnodes.clear();
	this->m_Tnodes.clear();
	this->m_signArray.clear();
	this->m_isPnode.clear();

	this->CompleteInit();
}

void GraphNoWeight::Clear()
{
	this->m_graph.clear();
	this->Reset();
}
void GraphNoWeight::AddRelation(vector<int> relation )
{
	vector<int> newNode;
	for ( int i=0; i<relation.size()-1 ; i++ )
	{
		int value = relation[i+1] - 1;
//#ifdef _DEBUG
//		cout << "pushBack: " << value << endl; 
//#endif
		newNode.push_back( value );
	}
	this->m_graph.push_back( newNode );
}

bool InputParser::ParseInputLine(vector<int>& output )
{
	string input;

	while ( input.empty() && getline( cin, input ) )
	{

	}

	return ParseInputLine( input, output );
}
bool InputParser::ParseInputLine( string input, vector<int>& output )
{
	int temp;
	output.clear();
	stringstream parser;
	parser << input;
	while ( parser >> temp )
	{
		output.push_back( temp );
	}

	return ( !output.empty() );
}

GraphNoWeight::GraphNoWeight( int size )
{
	this->m_graph.resize( size );
	
	this->CompleteInit();
}

GraphNoWeight::GraphNoWeight( )
{
	
}

GraphNoWeight::~GraphNoWeight()
{

}

Path GraphNoWeight::GetDijkstraMinPath(int begin, int end )
{
	Path shortestPath;

	//if ( begin == end )
	//{
	//	return shortestPath;
	//}

	this->m_signArray[begin].m_distance = 0;
	this->m_signArray[begin].m_comeNode = begin;
	this->m_Pnodes.push_back( begin );
	this->m_isPnode[begin] = true;

	vector<int> newPnodes = m_Pnodes;

	while ( !newPnodes.empty() )
	{
		this->m_Tnodes.clear();
		//vector<bool> temp;
		//temp.resize( this->m_graph.size(), false );
		//this->m_isPnode.swap( temp );


		for ( int i=0; i<newPnodes.size(); i++ )
		{
			// search all the connect with others.
			int curNode = newPnodes[i];
			for ( int k=0; k<m_graph[curNode].size(); k++ )
			{
				int index = this->m_graph[curNode][k];
				if ( !this->m_isPnode[index] )
				{
					this->m_Tnodes.insert( index );

//#ifdef _DEBUG
//					cout << "new T node : " << index << "By : " << curNode << endl;
//					if ( curNode == 18 )
//					{
//						int sdfd = 444;
//					}
//#endif

					int newDistance = this->m_signArray[curNode].m_distance + 1;
					int oldDistance = this->m_signArray[index].m_distance;
					if ( this->m_signArray[index].m_distance > newDistance )
					{
						this->m_signArray[index].m_distance = newDistance;
						this->m_signArray[index].m_comeNode = curNode;
					}
				}
			}
		}

		newPnodes.clear();

		for ( set<int>::iterator iter = m_Tnodes.begin(); iter != m_Tnodes.end(); iter++ )
		{
			int oneNewPnode = *iter;
			newPnodes.push_back( oneNewPnode );
			this->m_isPnode[ oneNewPnode ] = true;

			// Check the aim node.
			if ( oneNewPnode == end )
			{
				vector<int> backward;
				for ( int index = end; index != begin; index = this->m_signArray[index].m_comeNode )
				{
					backward.push_back( index );
				}
				backward.push_back( begin );

				int size = backward.size();
				shortestPath.resize( size );
				for ( int i=0; i<backward.size(); i++ )
				{
					shortestPath[ size - i - 1] = backward[i];
				}

				return shortestPath;
			}
		}
	}
	return shortestPath;
}

void GraphNoWeight::CompleteInit()
{
	vector<int> lastEmpty;
	this->m_graph.push_back( lastEmpty );
	this->m_signArray.resize( m_graph.size() );
	this->m_isPnode.resize( m_graph.size(), false );

	for ( int i=0; i<this->m_graph.size(); i++ )
	{
		for( int k=0; k< m_graph[i].size(); k++ )
		{
			int value = m_graph[i][k];
			if ( value > i )
			{
//#ifdef _DEBUG
//				cout << "put: " << i << "to: " << value << endl;
//#endif
				m_graph[value].push_back( i );
			}
		}
	}
}