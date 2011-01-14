#pragma once

/** 随机选号. */

class CRadomPick
{
public:
	CRadomPick(void);
	~CRadomPick(void);

	/** 随机选择一个序号. */
	int GetRadomNum( int nBegin, int nEnd );

};

