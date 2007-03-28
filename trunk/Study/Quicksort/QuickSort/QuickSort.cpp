// QuickSort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <BaseTsd.h>
#include <Windows.h>

void showTheArray( int * sortedArray , UINT_PTR totalCount, UINT_PTR samples = 1 )
{
	for ( UINT_PTR i=0; i<totalCount; i+=samples )
	{	
		std::cout << i << ":" << sortedArray[i] << "\t";
		if ( i%5 == 0 )
		{
			std::cout << std::endl;
		}
	}
}

INT_PTR Partition( int *sortedArray, INT_PTR begin, INT_PTR end, INT_PTR pivot )
{
	if ( end > begin )
	{
		int pivotValue = sortedArray[pivot];
		sortedArray[ pivot ] = sortedArray[ end ];
		sortedArray[ end ] = pivotValue;

		INT_PTR cursor = begin;
		for ( INT_PTR i= begin; i< end; i++ )
		{
			if ( sortedArray[i] <= pivotValue )
			{
				if ( i > cursor )
				{
					int temp = sortedArray[i];
					sortedArray[i] = sortedArray[cursor];
					sortedArray[cursor] = temp;
				}
				cursor ++;
			}
		}

		sortedArray[end] = sortedArray[cursor];
		sortedArray[cursor] = pivotValue;
		return cursor;
	}
	else
	{
		return -1;
	}
}

void QuickSortRecursive( int *sortedArray, INT_PTR begin, INT_PTR end )
{
	if ( end > begin )
	{
		int randNum = rand();
		randNum %= ( end - begin );
		INT_PTR newPivot = Partition( sortedArray, begin, end, begin + randNum );
		if ( newPivot != -1 )
		{
			QuickSortRecursive( sortedArray, begin, newPivot-1 );
			QuickSortRecursive( sortedArray, newPivot+1, end );
		}		
	}
}

void QuickSortLoop( int *sortedArray, INT_PTR arraySize )
{
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Please input the count of array will be sorted: " << std::endl;
	UINT_PTR count;

	//srand( ::GetTickCount() );

	while ( std::cin >> count )
	{
		//boost::array< int, count >* sortedArray = new boost::array< int, count>();
		int *sortedArray = new int[count];

		int number;
		for ( UINT_PTR i=0; i<count; i++ )
		{
			number = rand();
			sortedArray[i] = number;
		}

		showTheArray( sortedArray, count, count/500+1 );


		time_t beginTime = ::GetTickCount();

		QuickSortRecursive( sortedArray, 0, count-1 );

		time_t endTime = ::GetTickCount();

		std::cout << "\nThe last status: \n";

		showTheArray( sortedArray, count, count/500+1 );


		std::cout << "\nSpend Time:" << ( endTime - beginTime ) << std::endl;

		delete[] sortedArray;

		std::cout << "Please input the count of array will be sorted: " << std::endl;
	}
	return 0;
}

