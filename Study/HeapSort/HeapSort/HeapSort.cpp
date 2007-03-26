// HeapSort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <BaseTsd.h>
#include <Windows.h>
//#include <boost/array.hpp>

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

void MaxHeapify( int *sortedArray, UINT_PTR heapSize, INT_PTR nodeIndex )
{
	INT_PTR largestIndex = nodeIndex;
	UINT_PTR leftChildIndex = 2*nodeIndex+1;
	UINT_PTR rightChildIndex = leftChildIndex+1;
	if ( leftChildIndex < heapSize && sortedArray[largestIndex] < sortedArray[ leftChildIndex ] )
	{
		largestIndex = leftChildIndex;
	}

	if ( rightChildIndex < heapSize && sortedArray[largestIndex] < sortedArray[ rightChildIndex ] )
	{
		largestIndex = rightChildIndex;
	}
	if ( largestIndex != nodeIndex )
	{
		int temp = sortedArray[nodeIndex];
		sortedArray[nodeIndex] = sortedArray[largestIndex];
		sortedArray[largestIndex] = temp;

		if ( largestIndex < heapSize/2 )
		{
			MaxHeapify( sortedArray, heapSize, largestIndex );
		}
	}



}

void BuildMaxHeap( int *sortedArray, UINT_PTR heapSize )
{
	for ( INT_PTR i=heapSize/2-1; i>=0; i-- )
	{
		MaxHeapify( sortedArray, heapSize, i );
	}
}

void HeapSortRecursive( int * sortedArray, UINT_PTR heapSize )
{
	int temp = sortedArray[0];
	sortedArray[0] = sortedArray[heapSize-1];
	sortedArray[heapSize-1] = temp;
	heapSize --;
	if ( heapSize > 1 )
	{
		HeapSortRecursive( sortedArray, heapSize );
	}
}

void HeapSortLoop( int *sortedArray, UINT_PTR arraySize )
{	
	for ( UINT_PTR i=arraySize; i>1; i-- )
	{
		MaxHeapify( sortedArray, i, 0 );
		int temp = sortedArray[0];
		sortedArray[0] = sortedArray[i-1];
		sortedArray[i-1] = temp;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Please input the count of array will be sorted: " << std::endl;
	UINT_PTR count;

	while ( std::cin >> count )
	{
		//boost::array< int, count >* sortedArray = new boost::array< int, count>();
		int *sortedArray = new int[count];

		srand( ::GetTickCount() );

		int number;
		for ( UINT_PTR i=0; i<count; i++ )
		{
			number = rand();
			sortedArray[i] = number;
		}

		showTheArray( sortedArray, count, count/500+1 );


		time_t beginTime = ::GetTickCount();

		BuildMaxHeap( sortedArray, count );

		//MergerSortRecursive( sortedArray, 0, count-1 );
		//HeapSortRecursive( sortedArray, count );
		HeapSortLoop( sortedArray, count );

		time_t endTime = ::GetTickCount();

		std::cout << "\nThe last status: \n";

		showTheArray( sortedArray, count, count/500+1 );


		std::cout << "\nSpend Time:" << ( endTime - beginTime ) << std::endl;

		delete[] sortedArray;

		std::cout << "Please input the count of array will be sorted: " << std::endl;
	}

	system( "pause" );

	return 0;
}

