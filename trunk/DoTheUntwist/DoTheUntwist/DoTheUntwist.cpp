// DoTheUntwist.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Cryptography
{
public:
	Cryptography( int key, string text, bool bEncrypt= false )
	{		
		m_key = key;
		if( bEncrypt )
		{
			m_plaintext = text;

			this->Translate( m_plaintext, this->m_plainNum, true );


		}
		else
		{
			m_ciphertext = text;

			this->Translate( m_ciphertext, m_cipherNum, true );

			this->Decrypt( this->m_plainNum, this->m_cipherNum, this->m_key );

			this->Translate( m_plaintext, m_plainNum, false );
		}

		

	}

	~Cryptography()
	{
	}

	// ����..
	// ciphercode[i] = (plaincode[ki mod n] - i) mod 28.
	bool Decrypt( vector<int>& plainNum,const vector<int>& cipherNum, int key )
	{
		// Ϊԭ��׼�����ڴ�.
		plainNum.resize( cipherNum.size() );
		for( int ciphertextIndex = 0; ciphertextIndex < cipherNum.size(); ciphertextIndex ++ )
		{
			int maxNum = 27 - ciphertextIndex;
			int minNum = 0 - ciphertextIndex;
			
			int inBracket ;
			this->Unmode( cipherNum[ciphertextIndex] , 28, inBracket, minNum, maxNum);

			// ����ֵ..
			plainNum[ key*ciphertextIndex % cipherNum.size() ] = inBracket + ciphertextIndex ;

		}
		return true;
	}

	bool OutputPlantext()
	{
		if( m_plaintext.size() > 0 )
		{
			cout << m_plaintext << endl;
			return true;
		}
		else
		{
			return false;
		}
	}

private:

	// ��һ������һ����Χ�ڵĵ�һ����ģ..
	bool Unmode( int residue, int divisor, int& nResult, int minNum, int maxNum )
	{


		int minNumResidue = minNum%divisor ;
		int minus = residue - minNumResidue ;

		nResult = minNum + minus;
		if( nResult < minNum )
		{
			nResult +=  divisor;
		}
		else if( nResult > maxNum )
		{
			nResult -= divisor;
		}

		if( nResult <= maxNum && nResult >= minNum )
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	// ����.
	bool Translate( string& text, vector<int>& numQueue, bool bStrToNum )
	{
		if( bStrToNum )
		{
			numQueue.empty();

			int nNum;
			for( int i=0; i<text.size(); i++ )
			{
				char cText = text.at(i);
				if( cText == '_' )
				{
					nNum = 0;
				}
				else if( cText == '.' )
				{
					nNum = 27;
				}
				else
				{
					nNum = cText - 96;
				}
				numQueue.push_back( nNum );
			}
		}
		else
		{
			char curText;
			text.empty();
			for( int i=0; i<numQueue.size(); i++ )
			{
				int curNum = numQueue[i];
				if( curNum == 0 )
				{
					curText = '_';
				}
				else if( curNum == 27 )
				{
					curText = '.';
				}
				else
				{
					curText = curNum + 96;
				}

				text.push_back( curText );
			}
		}

		return true;
	}

private:
	int m_key;
	string m_plaintext;
	string m_ciphertext;

	vector<int> m_plainNum;
	vector<int> m_cipherNum;

};



int main()
{
	
	int key;
	string ciphertext;

	while( cin >> key  )
	{
		if( key == 0 )
		{
			break;
		}
		cin >> ciphertext;

		Cryptography cleverMan( key, ciphertext );

		cleverMan.OutputPlantext();
	}
	return 0;
}
