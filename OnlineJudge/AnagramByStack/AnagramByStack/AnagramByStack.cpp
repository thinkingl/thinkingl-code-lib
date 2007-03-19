// AnagramByStack.cpp : 定义控制台应用程序的入口点。
//

// 简要策略说明:
// 因为题目要求输出要是 "Dictionary order", 针对这个条件做了策略.
// 搜索是从一个 Push开始的.
// Push 是 i, Pop 是o, 所以每步搜索,只要先搜索Push,直接将结果输出后就是 "Dictionary order"了.
// 每步操作都是先 Push 下一个, 再 创建新 的一套数据, Pop 下一个..
// 注意,他们两个的数据不是一样的.两个函数的对应参数也是拷贝值传递形式的.

// 开始绝对是一个 Push, 结束绝对 会是一个Pop.

//#include "stdafx.h"


#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Push( const string& strSourceWord, const string& strTargetWord, int nSourcewordCursor, int nTargetwordCursor, vector<char> stack, vector<char> oneSequence );
void Pop( const string& strSourceWord, const string& strTargetWord, int nSourcewordCursor, int nTargetwordCursor, vector<char> stack, vector<char> oneSequence  );
void Output( const vector<char>& oneSequence );


// 寻找组合队列.
void SearchValidSequence(const string& strSourceWord, const string& strTargetWord )
{
	vector<char> oneSequence;
	vector<char> stack;
	Push( strSourceWord, strTargetWord, 0, 0, stack, oneSequence );
}

// 从输入中push一个.
// Push 会检查是否已经完成,但是不会输出,输出都是在Pop中做的.
void Push( const string& strSourceWord, const string& strTargetWord, int nSourcewordCursor, int nTargetwordCursor, vector<char> stack, vector<char> oneSequence )
{	

	// 源字符串已经没有可以输出的了.
	if( nSourcewordCursor >= strSourceWord.size() )
	{
		return ;
	}

	// 记录Push..
	stack.push_back( strSourceWord.at( nSourcewordCursor ) );
	oneSequence.push_back( 'i' );
	nSourcewordCursor ++;	// 这就是Push操作...!!!!!!

	// Push下一个.
	Push( strSourceWord, strTargetWord, nSourcewordCursor, nTargetwordCursor, stack, oneSequence );

	// Pop下一个.
	Pop( strSourceWord, strTargetWord, nSourcewordCursor, nTargetwordCursor, stack, oneSequence );

}

void Pop( const string& strSourceWord, const string& strTargetWord, int nSourcewordCursor, int nTargetwordCursor, vector<char> stack, vector<char> oneSequence  )
{
	// 目标串已经满了.
	if( nTargetwordCursor >= strTargetWord.size() )
	{
		// @@@@@@@@@@@@@
		// 目标满了,说明已经完成了 .输出这个队列.
		Output( oneSequence );

		return ;
	}

	// 不能对空堆栈Pop.
	if( stack.empty() )
	{
		return;
	}

	// 当前目标串需要的不是现在要Pop的字符.
	if( stack.at( stack.size()-1 ) != strTargetWord.at( nTargetwordCursor ) )
	{
		return ;
	}

	// 记录Pop操作..
	stack.pop_back();
	oneSequence.push_back( 'o' );

	nTargetwordCursor ++;

	// Push下一个.
	Push( strSourceWord, strTargetWord, nSourcewordCursor, nTargetwordCursor, stack, oneSequence );

	// Pop下一个.
	Pop( strSourceWord, strTargetWord, nSourcewordCursor, nTargetwordCursor, stack, oneSequence );
}

// 将一个组合队列输出.
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

