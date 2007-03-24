// MergerSort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <boost/array.hpp>
#include <Windows.h>

enum{ count = 100000000,
};

void showTheArray( boost::array<int, count>& sortedArray , int samples = 1 )
{
	for ( int i=0; i<count; i+=samples )
	{	
		std::cout << i << ":" << sortedArray[i] << "\t";

		if ( i%5 == 0 )
		{
			std::cout << "\n";
		}
	}
}

void showTheArray( int * sortedArray , int samples = 1 )
{
	for ( int i=0; i<count; i+=samples )
	{	
		std::cout << i << ":" << sortedArray[i] << "\t";

		if ( i%5 == 0 )
		{
			std::cout << "\n";
		}
	}
}

void MergerArray( boost::array<int, count>& sortedArray, int beginPoint, int mergerPoint, int endPoint )
{
	for ( ; mergerPoint<= endPoint; mergerPoint ++ )
	{
		int key = sortedArray[ mergerPoint ];
		int k = mergerPoint;
		while ( k>beginPoint && sortedArray[k-1] > key )
		{
			sortedArray[ k ] = sortedArray[ k-1 ];
			k --;
		}
		sortedArray[ k ] = key;
	}
}

void MergerArray( int *sortedArray, int beginPoint, int mergerPoint, int endPoint )
{
	for ( ; mergerPoint<= endPoint; mergerPoint ++ )
	{
		int key = sortedArray[ mergerPoint ];
		int k = mergerPoint;
		while ( k>beginPoint && sortedArray[k-1] > key )
		{
			sortedArray[ k ] = sortedArray[ k-1 ];
			k --;
		}
		sortedArray[ k ] = key;
	}
}

void MergerArrayOptimize( boost::array<int, count>& sortedArray, int beginPoint, int mergerPoint, int endPoint )
{
	int leftSize = mergerPoint - beginPoint;
	int rightSize = endPoint - mergerPoint + 1 ;
	int* leftArray = new int[ leftSize ];
	int* rightArray = new int[ rightSize ];
	for ( int i=beginPoint; i<mergerPoint; i++ )
	{
		leftArray[ i - beginPoint ] = sortedArray[i];
	}
	for ( int i=mergerPoint; i<= endPoint; i++ )
	{
		rightArray[ i - mergerPoint ] = sortedArray[ i ];
	}

	int leftCursor = 0;
	int rightCursor = 0;
	for ( int i = beginPoint; i<= endPoint; i++ )
	{
	
		if ( rightCursor < rightSize && ( leftCursor >= leftSize || leftArray[leftCursor] > rightArray[rightCursor] ) )
		{
			sortedArray[i] = rightArray[ rightCursor ];
			rightCursor ++;
		}
		else
		{
			sortedArray[i] = leftArray[ leftCursor ];
			leftCursor ++;
		}		 
	}

	delete[] leftArray;
	delete[] rightArray;
}

void MergerArrayOptimize( int * sortedArray, int beginPoint, int mergerPoint, int endPoint )
{
	int leftSize = mergerPoint - beginPoint;
	int rightSize = endPoint - mergerPoint + 1 ;
	int* leftArray = new int[ leftSize ];
	int* rightArray = new int[ rightSize ];
	for ( int i=beginPoint; i<mergerPoint; i++ )
	{
		leftArray[ i - beginPoint ] = sortedArray[i];
	}
	for ( int i=mergerPoint; i<= endPoint; i++ )
	{
		rightArray[ i - mergerPoint ] = sortedArray[ i ];
	}

	int leftCursor = 0;
	int rightCursor = 0;
	for ( int i = beginPoint; i<= endPoint; i++ )
	{

		if ( rightCursor < rightSize && ( leftCursor >= leftSize || leftArray[leftCursor] > rightArray[rightCursor] ) )
		{
			sortedArray[i] = rightArray[ rightCursor ];
			rightCursor ++;
		}
		else
		{
			sortedArray[i] = leftArray[ leftCursor ];
			leftCursor ++;
		}		 
	}

	delete[] leftArray;
	delete[] rightArray;
}

void MergerSortRecursive( boost::array<int, count>& sortedArray, int beginPoint, int endPoint )
{
	if ( beginPoint == endPoint - 1 )
	{
		if ( sortedArray[beginPoint] > sortedArray[endPoint] )
		{
			int key = sortedArray[beginPoint];
			sortedArray[beginPoint] = sortedArray[endPoint];
			sortedArray[endPoint] = key;
		}
	}
	else
	{
		if ( endPoint > beginPoint )
		{
			int middlePoint = ( beginPoint + endPoint )/2;
			MergerSortRecursive( sortedArray, beginPoint, middlePoint );
			MergerSortRecursive( sortedArray, middlePoint+1, endPoint );
			//MergerArray( sortedArray, beginPoint, middlePoint+1, endPoint );
			MergerArrayOptimize( sortedArray, beginPoint, middlePoint+1, endPoint );
		}
	}
}

void MergerSortLoop(  boost::array<int, count>& sortedArray )
{
	int mergerSize = 1;
//	int middlePoint = (count+1)/2;

	while ( mergerSize < count )
	{
		for ( int i=0; i < count - mergerSize; i += mergerSize*2)
		{
			MergerArrayOptimize( sortedArray, i, i+mergerSize, (i+mergerSize*2)<count? (i+mergerSize*2-1):count-1 );
		}

		mergerSize *= 2;
	}	
}

void MergerSortLoop(  int* sortedArray )
{
	int mergerSize = 1;
	//	int middlePoint = (count+1)/2;

	while ( mergerSize < count )
	{
		for ( int i=0; i < count - mergerSize; i += mergerSize*2)
		{
			MergerArrayOptimize( sortedArray, i, i+mergerSize, (i+mergerSize*2)<count? (i+mergerSize*2-1):count-1 );
		}

		mergerSize *= 2;
	}	
}



int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "The size of sorted array is " << count;

	//boost::array< int, count > sortedArray;
	//boost::array< int, count > *sortedArray = &Array;
	boost::array< int, count > *sortedArray = new boost::array< int, count>();
	//int *Array = new int[count] ;
	//int **sortedArray = &Array;

	std::cout << "The inited status of sorted array is: \n";

	srand( ::GetTickCount() );
	//srand( 0 );

	int number;
	for ( int i=0; i<count; i++ )
	{
		number = rand();
		(*sortedArray)[i] = number;
	}

	std::cout << "\nThe Init status: \n";

	//showTheArray( sortedArray );

	time_t beginTime = ::GetTickCount();

	//MergerSortRecursive( sortedArray, 0, count-1 );
	MergerSortLoop( *sortedArray );

	time_t endTime = ::GetTickCount();

	std::cout << "\nThe last status: \n";

	showTheArray( *sortedArray, count/500+1 );


	std::cout << "\nSpend Time:" << ( endTime - beginTime ) << std::endl;

	delete sortedArray;

	system( "pause" );
	return 0;
}

