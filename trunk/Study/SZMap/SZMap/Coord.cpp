#include "StdAfx.h"
#include "Coord.h"


CCoord::CCoord(void)
{
	this->m_dbLatitude = 0.0;
	this->m_dbLongitude = 0.0;
	this->m_nZLevel = 0;
}


CCoord::~CCoord(void)
{
}

CCoord::CCoord( double latitude, double longitude, int zlevel )
{
	this->m_dbLatitude = latitude;
	this->m_dbLongitude = longitude;
	this->m_nZLevel = zlevel;
}

CCoord::CCoord( const CCoord& another )
{
	this->operator=( another );
}

CCoord& CCoord::operator=( const CCoord& another )
{
	if( this == &another )
	{
		return *this;
	}
	else
	{
		this->m_dbLatitude = another.m_dbLatitude;
		this->m_dbLongitude = another.m_dbLongitude;
		this->m_nZLevel = another.m_nZLevel;
	}
	return *this;
}

double CCoord::GetLatitude() const
{
	return this->m_dbLatitude;
}

void CCoord::SetLatitude( double latitude )
{
	this->m_dbLatitude = latitude;
}

double CCoord::GetLongitude() const
{
	return this->m_dbLongitude;
}

void CCoord::SetLongitude( double longitude )
{
	this->m_dbLongitude = longitude;
}

int CCoord::GetZLevel() const
{
	return this->m_nZLevel;
}

void CCoord::SetZLevel( int zlevel )
{
	this->m_nZLevel = zlevel;
}


