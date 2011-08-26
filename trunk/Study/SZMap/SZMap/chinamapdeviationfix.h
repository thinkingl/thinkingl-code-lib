#pragma once

#include "correctionpointdatabase.h"

class CChinaMapDeviationFix
{
public:
	CChinaMapDeviationFix();
	~CChinaMapDeviationFix();

	/** 火星坐标转地球坐标. */
	void MarsToEarth( double marsLongitude, double marsLatitude, 
		double& earthLongitude, double& earthLatitude );

	void EarthToMars( double earthLongitude, double earthLatitude, 
		double& marsLongitude, double& marsLatitude );

private:
	/** 已知 (x1,y1) 和 ( x2,y2) ,求 (x,?)中的y */
	double  LinearInterpolate( double x1, double y1, double x2, double y2, double x );
private:
	CCorrectionPointDatabase m_cpDatabase;
};

// end of file


