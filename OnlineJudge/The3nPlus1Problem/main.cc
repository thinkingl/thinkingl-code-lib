// 
// File:   main.cc
// Author: lizhixing
//
// Created on 2007年8月3日, 下午12:26
//

#include <stdlib.h>
#include <iostream>

using namespace std;

int GetCycleLength( int num )
{
    int counter = 1;
    while( num != 1 )
    {
        ++ counter;
        if( ( num & 1 ) == 1 )
        {
            // odd.
            num = 3*num +1;
        }
        else
        {
            num >>= 1;
        }
    }
    return counter;
}
//
// 
//
int main(int argc, char** argv) {
    
    int nMaxNum;
    int nMinNum;
    while( cin >> nMinNum >> nMaxNum )
    {
        cout << nMinNum << " " << nMaxNum << " ";
        int nMaxCycleLength = 0;
        for( int i=nMinNum+1; i<nMaxNum ; ++i )
        {
            int cycleLen = GetCycleLength( i );
            if( cycleLen > nMaxCycleLength )
            {
                nMaxCycleLength = cycleLen;
            }
        }
        
        cout << nMaxCycleLength << endl;
    }
    return (EXIT_SUCCESS);
}


