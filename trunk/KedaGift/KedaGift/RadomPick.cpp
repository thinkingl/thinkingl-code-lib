#include "StdAfx.h"
#include "RadomPick.h"


CRadomPick::CRadomPick(void)
{
}


CRadomPick::~CRadomPick(void)
{
}

int CRadomPick::GetRadomNum( int nBegin, int nEnd )
{
	// �Ƿ��ʼ�����������.
	static bool s_bSetRadom = false;
	if( !s_bSetRadom )
	{
		DWORD dwSeed = GetTickCount();
		srand( dwSeed );
		s_bSetRadom = true;
	}

	int nRand = rand();
	int nCount = nEnd - nBegin +1;
	int nRet = nBegin + ( nRand % nCount );
	return nRet;
}