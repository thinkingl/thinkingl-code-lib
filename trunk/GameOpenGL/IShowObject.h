#pragma once

/** ��ʾ�ӿ�. */
class IShowObject
{
public:
	virtual void Show() = 0;
};

typedef std::vector< IShowObject* > CShowObjectPointList;