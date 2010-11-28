/*
ID: thinkin6
PROG: rockers
LANG: C++
*/

/** USA CO 3.4.5 :
Raucous Rockers
You just inherited the rights to N (1 <= N <= 20) previously unreleased songs recorded by the popular group Raucous Rockers. You plan to release a set of M (1 <= M <= 20) compact disks with a selection of these songs. Each disk can hold a maximum of T (1 <= T <= 20) minutes of music, and a song can not overlap from one disk to another.

Since you are a classical music fan and have no way to judge the artistic merits of these songs, you decide on the following criteria for making the selection:

The songs on the set of disks must appear in the order of the dates that they were written.
The total number of songs included will be maximized.
PROGRAM NAME: rockers

INPUT FORMAT

Line 1:	 Three integers: N, T, and M.
Line 2:	 N integers that are the lengths of the songs ordered by the date they were written.
SAMPLE INPUT (file rockers.in)

4 5 2
4 3 4 2
OUTPUT FORMAT

A single line with an integer that is the number of songs that will fit on M disks.
SAMPLE OUTPUT (file rockers.out)

3
*/

/** 
思路：
*	动态规划问题。
*	用n个碟片放m首歌曲的最大值，就是找出 用n-1个碟片放前一首歌曲的最大值+用1个碟片放后面歌曲的最大值， 
*	用n-1张碟片放前两首歌曲的最大值 + 用1个碟片放后面歌曲的最大数........用n-1个碟片放所有歌曲的最大值
*	找出它们中最大的那个。
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




int main()
{
	string strProblemName = "rockers";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	const int MaxSongNum = 20;
	const int MaxDiskNum = 20;
	const int MaxDiskLen = 20;

	int songNum, diskLength, diskNum;
	fin >> songNum >> diskLength >> diskNum;

	int arSongLen[ MaxSongNum+1 ] = { 0 };
	for ( int i=0; i<songNum; ++i )
	{
		fin >> arSongLen[i];
	}
	
	// 只有一张磁盘的情况。
	int arMaxSongOneDiskCanContain[MaxSongNum+1][MaxSongNum+1][MaxDiskLen+1];
	memset( arMaxSongOneDiskCanContain, 0, sizeof( arMaxSongOneDiskCanContain ) );
	for ( int startSong=0; startSong<songNum; ++startSong )
	{
		for ( int lastSong=startSong; lastSong<songNum; ++lastSong )
		{
			for ( int dl=0; dl<=diskLength; ++dl )
			{
				// 只有一首歌。
				if ( startSong == lastSong )
				{
					arMaxSongOneDiskCanContain[ startSong ][ lastSong ][ dl ] =
						( dl >= arSongLen[ startSong ] ) ? 1 : 0;
				}
				else
				{
					// 一种情况是磁盘不放最后一首歌。
					int maxSong1 = arMaxSongOneDiskCanContain[ startSong ][ lastSong-1 ][dl];

					// 另一种情况是磁盘放最后一首歌。
					int maxSong2 = 0;
					int diskLeft = dl - arSongLen[ lastSong ];
					if ( diskLeft >= 0 )
					{
						maxSong2 = arMaxSongOneDiskCanContain[ startSong ][ lastSong-1 ][ diskLeft ] + 1;
					}
					arMaxSongOneDiskCanContain[ startSong ][lastSong][dl] = max( maxSong1, maxSong2 );
				}

				cout << "start: " << startSong << " last " << lastSong << " disklen " << dl << " maxsong " 
					<< arMaxSongOneDiskCanContain[ startSong ][ lastSong ][ dl ] << endl;
				
			}			
			
		}
	}

	int arMaxSongDiskCanContain[MaxSongNum+1][MaxSongNum+1][MaxDiskNum+1];
	memset( arMaxSongDiskCanContain, 0, sizeof( arMaxSongDiskCanContain ) );

	for ( int startSong=0; startSong<songNum; ++startSong )
	{
		for ( int lastSong=startSong; lastSong<songNum; ++lastSong )
		{
			arMaxSongDiskCanContain[startSong][lastSong][1] = 
				arMaxSongOneDiskCanContain[startSong][lastSong][diskLength];
		}
	}


	for ( int dn=2; dn<=diskNum; ++dn )
	{
		for ( int startSong=0; startSong<songNum; ++startSong )
		{
			for ( int lastSong=startSong+1; lastSong<songNum; ++lastSong )
			{
				// 拆成两部分，前面一部分用dn-1个磁盘放，后面一部分用一个磁盘放。
				int maxSong = 0;
				for ( int splitPos = startSong; splitPos <= lastSong; ++splitPos )
				{
					int song = arMaxSongDiskCanContain[ startSong ][ splitPos ][ dn-1 ]
					+ arMaxSongDiskCanContain[ splitPos+1 ][ lastSong ][ 1 ];
					if ( song > maxSong )
					{
						maxSong = song;
					}
				}
				arMaxSongDiskCanContain[ startSong ][ lastSong ][ dn ] = maxSong;
			}
		}
	}
	
	fout << arMaxSongDiskCanContain[ 0 ][ songNum-1 ][ diskNum ] << endl;

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}