// AnagramByStack.cpp : �������̨Ӧ�ó������ڵ㡣
//

// ��Ҫ����˵��:
// ��Ϊ��ĿҪ�����Ҫ�� "Dictionary order", �������������˲���.
// �����Ǵ�һ�� Push��ʼ��.
// Push �� i, Pop ��o, ����ÿ������,ֻҪ������Push,ֱ�ӽ������������ "Dictionary order"��.
// ÿ������������ Push ��һ��, �� ������ ��һ������, Pop ��һ��..
// ע��,�������������ݲ���һ����.���������Ķ�Ӧ����Ҳ�ǿ���ֵ������ʽ��.

// ��ʼ������һ�� Push, �������� ����һ��Pop.

//#include "stdafx.h"


#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Push( const string& strSourceWord, const string& strTargetWord, int nSourcewordCursor, int nTargetwordCursor, vector<char> stack, vector<char> oneSequence );
void Pop( const string& strSourceWord, const string& strTargetWord, int nSourcewordCursor, int nTargetwordCursor, vector<char> stack, vector<char> oneSequence  );
void Output( const vector<char>& oneSequence );


// Ѱ����϶���.
void SearchValidSequence(const string& strSourceWord, const string& strTargetWord )
{
	vector<char> oneSequence;
	vector<char> stack;
	Push( strSourceWord, strTargetWord, 0, 0, stack, oneSequence );
}

// ��������pushһ��.
// Push �����Ƿ��Ѿ����,���ǲ������,���������Pop������.
void Push( const string& strSourceWord, const string& strTargetWord, int nSourcewordCursor, int nTargetwordCursor, vector<char> stack, vector<char> oneSequence )
{	

	// Դ�ַ����Ѿ�û�п����������.
	if( nSourcewordCursor >= strSourceWord.size() )
	{
		return ;
	}

	// ��¼Push..
	stack.push_back( strSourceWord.at( nSourcewordCursor ) );
	oneSequence.push_back( 'i' );
	nSourcewordCursor ++;	// �����Push����...!!!!!!

	// Push��һ��.
	Push( strSourceWord, strTargetWord, nSourcewordCursor, nTargetwordCursor, stack, oneSequence );

	// Pop��һ��.
	Pop( strSourceWord, strTargetWord, nSourcewordCursor, nTargetwordCursor, stack, oneSequence );

}

void Pop( const string& strSourceWord, const string& strTargetWord, int nSourcewordCursor, int nTargetwordCursor, vector<char> stack, vector<char> oneSequence  )
{
	// Ŀ�괮�Ѿ�����.
	if( nTargetwordCursor >= strTargetWord.size() )
	{
		// @@@@@@@@@@@@@
		// Ŀ������,˵���Ѿ������ .����������.
		Output( oneSequence );

		return ;
	}

	// ���ܶԿն�ջPop.
	if( stack.empty() )
	{
		return;
	}

	// ��ǰĿ�괮��Ҫ�Ĳ�������ҪPop���ַ�.
	if( stack.at( stack.size()-1 ) != strTargetWord.at( nTargetwordCursor ) )
	{
		return ;
	}

	// ��¼Pop����..
	stack.pop_back();
	oneSequence.push_back( 'o' );

	nTargetwordCursor ++;

	// Push��һ��.
	Push( strSourceWord, strTargetWord, nSourcewordCursor, nTargetwordCursor, stack, oneSequence );

	// Pop��һ��.
	Pop( strSourceWord, strTargetWord, nSourcewordCursor, nTargetwordCursor, stack, oneSequence );
}

// ��һ����϶������.
void Output( const vector<char>& oneSequence )
{

	for( int i=0; i< oneSequence.size(); i++ )
	{
		cout << oneSequence[i] << " ";
	}
	cout << endl;
}

int main()
{
	string strSourceWord, strTargetWord;

	while( cin >> strSourceWord >> strTargetWord )
	{
		//
		

		cout << "[" << endl;
		
		SearchValidSequence( strSourceWord, strTargetWord );

		cout << "]" << endl;
		
	}
	


	return 0;
}

