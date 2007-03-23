// InsertSort.cpp : 定义控制台应用程序的入口点。
//
// Goal point:
// Learn English;
// Try to use the boost lib.
// Test of insert sort algorithm.


#include "stdafx.h"
#include <boost\array.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>

enum{ count = 10000 ,
};

void showTheArray( boost::array<int, count>& sortedArray )
{
	for ( int i=0; i<count; i++ )
	{	
		std::cout << i << ":" << sortedArray[i] << "\t";

		if ( i%5 == 0 )
		{
			std::cout << "\n";
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "The size of sorted array is " << count;
	
	boost::array< int, count > sortedArray;

	std::cout << "The inited status of sorted array is: \n";

	srand( ::GetTickCount() );

	int number;
	for ( int i=0; i<count; i++ )
	{
		number = rand();
		sortedArray[i] = number;
	}

	//showTheArray( sortedArray );

	// begin to sort;
	time_t beginTime = ::GetTickCount();
	for ( int i=1; i<count; i++ )
	{
		int key = sortedArray[i];
		int k = i;
		while ( k > 0 && sortedArray[k-1] > key )
		{
			sortedArray[k] = sortedArray[k-1];
			k --;
		}
		sortedArray[k] = key;
	}
	time_t endTime = ::GetTickCount();

	std::cout << "\n The sorted array: \n";

	//showTheArray( sortedArray );

	std::cout << "\n Spend time : " << ( endTime - beginTime );

	
	int temp;
	std::cin >> temp; 

	return 0;
}

