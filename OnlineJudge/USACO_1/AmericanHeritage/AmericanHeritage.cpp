/*
ID: thinkin6
PROG: heritage
LANG: C++
*/

/** USA CO 3.4.3 :
American Heritage
Farmer John takes the heritage of his cows very seriously. He is not, however, a truly fine bookkeeper. He keeps his cow genealogies as binary trees and, instead of writing them in graphic form, he records them in the more linear `tree in-order' and `tree pre-order' notations.

Your job is to create the `tree post-order' notation of a cow's heritage after being given the in-order and pre-order notations. Each cow name is encoded as a unique letter. (You may already know that you can frequently reconstruct a tree from any two of the ordered traversals.) Obviously, the trees will have no more than 26 nodes.

Here is a graphical representation of the tree used in the sample input and output:

C
/   \
/     \
B       G
/ \     /
A   D   H
/ \
E   F
The in-order traversal of this tree prints the left sub-tree, the root, and the right sub-tree.

The pre-order traversal of this tree prints the root, the left sub-tree, and the right sub-tree.

The post-order traversal of this tree print the left sub-tree, the right sub-tree, and the root.

PROGRAM NAME: heritage

INPUT FORMAT

Line 1:	The in-order representation of a tree.
Line 2:	 The pre-order representation of that same tree.
SAMPLE INPUT (file heritage.in)

ABEDFCHG
CBADEFGH
OUTPUT FORMAT

A single line with the post-order representation of the tree.
SAMPLE OUTPUT (file heritage.out)

AEFDBHGC 
*/

/** 
思路：pre-order 树的第一项是树的根节点。
*	根节点把 in-order 分成两部分，分别就是左右两颗子树。
*	根据 in-order 的两颗子树的序列，我们又可以去把 pre-order 树拆成左右两颗子树。
*	这样递归拆下去，就可以得到原本的那棵树。
*	然后把这棵树按照post-order 的顺序遍历一遍就行了。
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

/** 树 */
template< class _Type >
class CTree
{
public:
	CTree();
	~CTree();
	_Type Value()const;
	void Value( const _Type& v );
	CTree<_Type> *LeftChild();
	void LeftChild( CTree<_Type>* pc );
	CTree<_Type> *RightChild();
	void RightChild( CTree<_Type>* pc );
private:
	_Type m_nodeValue;
	CTree *m_pLeftChild;
	CTree *m_pRightChild;

};

template< class _Type >
CTree<_Type>::CTree()
{
	m_nodeValue = 0;
	m_pLeftChild = m_pRightChild = 0;
};

template< class _Type >
CTree<_Type>::~CTree()
{
	if ( m_pLeftChild )
	{
		delete m_pLeftChild;
		m_pLeftChild = 0;
	}
	if ( m_pRightChild )
	{
		delete m_pRightChild;
		m_pRightChild = 0;
	}
};

template< class _Type >
_Type CTree<_Type>::Value()const
{
	return m_nodeValue;
}

template< class _Type >
void CTree<_Type>::Value( const _Type& v )
{
	m_nodeValue = v;
}

template< class _Type >
CTree<_Type> * CTree<_Type>::LeftChild()
{
	return m_pLeftChild;
}

template< class _Type >
CTree<_Type> * CTree<_Type>::RightChild()
{
	return m_pRightChild;
}

template< class _Type >
void CTree<_Type>::LeftChild( CTree<_Type> * pt )
{
	m_pLeftChild = pt;
}

template< class _Type >
void CTree<_Type>::RightChild( CTree<_Type> * pt )
{
	m_pRightChild = pt;
}

typedef CTree<char> TCharTree;
// 根据两种顺序建树。
void ParseAndBuildTree( const string& inOrderSquence, const string& preOrderSequence, 
	TCharTree* pTree )
{
	if ( inOrderSquence.empty() || preOrderSequence.empty() )
	{
		return;
	}
	if ( inOrderSquence.size() != preOrderSequence.size() || 0 == pTree )
	{
		return;
	}

	// pre-order 遍历的顺序是 root > left > right
	// 所以第一个就是 root.
	char cRoot = preOrderSequence[0];
	pTree->Value( cRoot ); 

	// root 把 left > root > right 循序的 in-order 树拆成左右两颗子树。
	int nRootInorder = inOrderSquence.find( cRoot );

	// 从根这里拆成左右两棵树，递归分析。
	string leftInOrder = inOrderSquence.substr( 0, nRootInorder );
	string leftPreOrder = preOrderSequence.substr( 1, nRootInorder );
	if ( !leftInOrder.empty() )
	{
		pTree->LeftChild( new TCharTree() );
		ParseAndBuildTree( leftInOrder, leftPreOrder, pTree->LeftChild() );
	}
	
	// 右边子树
	string rightInorder = inOrderSquence.substr( nRootInorder + 1 );
	string rightPreOrder = preOrderSequence.substr( nRootInorder + 1 );
	if ( !rightInorder.empty() )
	{
		pTree->RightChild( new TCharTree() );
		ParseAndBuildTree( rightInorder, rightPreOrder, pTree->RightChild() );
	}

}

// post-order 遍历树。 left > right > root
void PostOrderTravelTree( string& travalSequence, TCharTree * pTree )
{
	// The post-order traversal of this tree print the left sub-tree, the right sub-tree, and the root.
	TCharTree *pLeftChild = pTree->LeftChild();
	if ( pLeftChild )
	{
		PostOrderTravelTree( travalSequence, pLeftChild );
	}

	TCharTree *pRightChild = pTree->RightChild();
	if ( pRightChild )
	{
		PostOrderTravelTree( travalSequence, pRightChild );
	}

	// root.
	travalSequence.push_back( pTree->Value() );
}

int main()
{
	string strProblemName = "heritage";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	string inOrderSequence, preOrderSequence;

	fin >> inOrderSequence;
	fin >> preOrderSequence;

	

	TCharTree cowTree;
	ParseAndBuildTree( inOrderSequence, preOrderSequence, &cowTree );

	string postOrderSequence;
	PostOrderTravelTree( postOrderSequence, &cowTree );

	fout << postOrderSequence << endl;

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