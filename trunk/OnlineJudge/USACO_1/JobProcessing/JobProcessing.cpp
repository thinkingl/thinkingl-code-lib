/*
ID: thinkin6
PROG: job
LANG: C++
*/

/** USA CO 4.2.3 :
Job Processing
IOI'96
A factory is running a production line that requires two operations to be performed on each job: first operation "A" then operation "B". Only a certain number of machines are capable of performing each operation.


Figure 1 shows the organization of the production line that works as follows. A type "A" machine takes a job from the input container, performs operation "A" and puts the job into the intermediate container. A type "B" machine takes a job from the intermediate container, performs operation "B" and puts the job into the output container. All machines can work in parallel and independently of each other, and the size of each container is unlimited. The machines have different performance characteristics, a given machine requires a given processing time for its operation.
Give the earliest time operation "A" can be completed for all N jobs provided that the jobs are available at time 0. Compute the minimal amount of time that is necessary to perform both operations (successively, of course) on all N jobs.

PROGRAM NAME: job

INPUT FORMAT

Line 1:	 Three space-separated integers:
N, the number of jobs (1<=N<=1000).
M1, the number of type "A" machines (1<=M1<=30)
M2, the number of type "B" machines (1<=M2<=30)
Line 2..etc:	 M1 integers that are the job processing times of each type "A" machine (1..20) followed by M2 integers, the job processing times of each type "B" machine (1..20).
SAMPLE INPUT (file job.in)

5 2 3
1 1 3 1 4
OUTPUT FORMAT

A single line containing two integers: the minimum time to perform all "A" tasks and the minimum time to perform all "B" tasks (which require "A" tasks, of course).
SAMPLE OUTPUT (file job.out)

3 5
*/

/** 
*	
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

typedef std::vector< int > TTimeList;

const int IFINITE = 0xFFFFFFF;

/** ����,��ȡ�������ʱ��.
*	
*/
void GetOperationFinishTime( int nJobNum, 
	const TTimeList& tMachineAOpTime, const TTimeList& tMachineBOpTime,
	int& nOpAFinishTime, int& nOpBFinishTime )
{
	// ���ֻ�������Ŀ.
	int nMachineANum = tMachineAOpTime.size();
	int nMachineBNum = tMachineBOpTime.size();

	// ���ֻ�����״̬,��ŵ�����ɲ�����Ҫ��ȫ��ʱ��.��ʼ��û�й���,Ϊ0.
	TTimeList tMachineATotalTime( nMachineANum, 0 );
	TTimeList tMachineBTotalTime( nMachineBNum, 0 );

	// ����job container.
	int nInputContainer = nJobNum;
	int nIntermediateContainer = 0;
	int nOutContainer = 0;

	// ��jobһ�������������.
	for ( int i=0; i<nJobNum; ++i )
	{
		// Ѱ��������������job��A�����Ļ���.
		// ��������ɵ�ʱ��.
		int nMinFinishATime = IFINITE;
		// ����A�����Ļ������.
		int nOpAMachine = 0;
		// �������е�A����,Ѱ����������ɲ�����.
		for ( int a=0; a<nMachineANum; ++a )
		{
			int finishTime = tMachineATotalTime[a] + tMachineAOpTime[a];
			if ( finishTime < nMinFinishATime )
			{
				nOpAMachine = a;
				nMinFinishATime = finishTime;
			}
		}

		// ����̨��������A����.
		tMachineATotalTime[ nOpAMachine ] = nMinFinishATime;

		// ��̨���������A������,����B������,����B����.

		// Ѱ��һ������������jobB�����Ļ���.
		int nMinFinishBTime = IFINITE;
		int nOpBMachine = 0;
		for ( int b=0; b<nMachineBNum; ++b )
		{
			// һ̨����ֻ���� A���������֮��( nMinFinishATime ) ���ܽ���B����.
			int beginTime = max( tMachineBTotalTime[b], nMinFinishATime );
			int finishTime = beginTime + tMachineBOpTime[b];
			if ( finishTime < nMinFinishBTime )
			{
				nOpBMachine = b;
				nMinFinishBTime = finishTime;
			}
		}

		// ����̨�������B����.
		tMachineBTotalTime[ nOpBMachine ] = nMinFinishBTime;

	}

	// �������ɵ�A�����ҳ���.
	TTimeList::iterator itMaxFinishATime = max_element( tMachineATotalTime.begin(), tMachineATotalTime.end() );
	nOpAFinishTime = *itMaxFinishATime;

	TTimeList::iterator itMaxFinishBTime = max_element( tMachineBTotalTime.begin(), tMachineBTotalTime.end() );
	nOpBFinishTime = *itMaxFinishBTime;
}
int main()
{
	string strProblemName = "job";
	string strInFile = strProblemName + ".in";
	string strOutFile = strProblemName + ".out";
	ofstream fout ( strOutFile.c_str() );
	ifstream fin ( strInFile.c_str() );

	if( !fin )
	{
		cout << "open input file fail!" << endl;
		return 0;
	}

	int nJobNum, nMachineANum, nMachineBNum;
	fin >> nJobNum >> nMachineANum >> nMachineBNum;

	
	TTimeList tMachineAOpTime;
	for ( int i=0; i<nMachineANum; ++i )
	{
		int nTime;
		fin >> nTime;
		tMachineAOpTime.push_back( nTime );
	}
	TTimeList tMachineBOpTime;
	for ( int i=0; i<nMachineBNum; ++i )
	{
		int nTime;
		fin >> nTime;
		tMachineBOpTime.push_back( nTime );
	}

	// ģ��.
	int nOpATotalTime = 0;
	int nOpBTotalTime = 0;
	GetOperationFinishTime( nJobNum, tMachineAOpTime, tMachineBOpTime, nOpATotalTime, nOpBTotalTime );

	fout << nOpATotalTime << " " << nOpBTotalTime << endl;

#ifdef THINKINGL

	cout << "use clock: " << clock() << " / " << CLOCKS_PER_SEC << endl;

	cout << "-----------begin--dump--output--file----------------" << endl << endl;
	system( ( string( "type " ) + strOutFile ).c_str() );
	cout << endl;
	system( "pause" );
#endif

	return 0;
}