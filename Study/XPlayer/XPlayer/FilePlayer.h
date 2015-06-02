#pragma once

/**
*	ÎÄ¼þ²¥·ÅÆ÷.
*	+by lizhixing@2015-06-02
*/

class CFilePlayer
{
public:
	CFilePlayer();
	virtual ~CFilePlayer();

	bool Play( LPCTSTR filePath );
	bool Stop();

};

