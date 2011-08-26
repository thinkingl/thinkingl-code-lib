#pragma once

#include "correctionpointdatabase.h"

class CChinaMapDeviationFix
{
public:
	CChinaMapDeviationFix();
	~CChinaMapDeviationFix();

	/** ��������ת��������. */
	void MarsToEarth( double marsLongitude, double marsLatitude, 
		double& earthLongitude, double& earthLatitude );

	void EarthToMars( double earthLongitude, double earthLatitude, 
		double& marsLongitude, double& marsLatitude );

private:
	/** ��֪ (x1,y1) �� ( x2,y2) ,�� (x,?)�е�y */
	double  LinearInterpolate( double x1, double y1, double x2, double y2, double x );
private:
	CCorrectionPointDatabase m_cpDatabase;
};

// end of file


