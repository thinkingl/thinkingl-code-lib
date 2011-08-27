#include "chinamapdeviationfix.h"

#include "szmapconfig.h"
#include "Log.h"

// 搞到的数据的精度是 0.025 个经纬度.
const double CORRECTION_POINTS_PRECISION = 0.025;

CChinaMapDeviationFix::CChinaMapDeviationFix()
{
	tstring datasePath = CSZMapConfig::GetDatabasePath();
	m_cpDatabase.OpenDatabase( datasePath );
}

CChinaMapDeviationFix::~CChinaMapDeviationFix()
{
	m_cpDatabase.Close();
}

void CChinaMapDeviationFix::MarsToEarth( double marsLongitude, double marsLatitude, double& earthLongitude, double& earthLatitude )
{
	CCorrectionPointDatabase::TRecordList pointsAround = 
		m_cpDatabase.GetMarsPointsAround( marsLongitude, marsLatitude, CORRECTION_POINTS_PRECISION );

	// 应该得到至少4个点.
	if ( pointsAround.size() < 4 )
	{
		ASSERT( FALSE );
		Log() << "MarsToEarth 周围没有找到足够的校正点!" << endl;
		return;
	}
	ASSERT( pointsAround.size() == 4 );

	// 确定各个校正点的位置.
	int lefttopIndex = -1;
	int leftbottomIndex = -1;
	int righttopIndex = -1;
	int rightbottomIndex = -1;
	for ( int i=0; i<pointsAround.size(); ++i )
	{
		if ( pointsAround[i].m_marsLongitude > marsLongitude )	// 中国是东经. 最西(左)端为东经73, 最东(右)为东经135度.
		{
			// 说明 校正点在 我这个点的 右(东)边
			if ( pointsAround[i].m_marsLatitude > marsLatitude )
			{
				// 说明校正点在 我这个点的 上(北)面.
				righttopIndex = i;
			}
			else
			{
				rightbottomIndex = i;
			}
		}
		else
		{
			if ( pointsAround[i].m_marsLatitude > marsLatitude )
			{
				// 说明校正点在 我这个点的 上(北)面.
				lefttopIndex = i;
			}
			else
			{
				leftbottomIndex = i;
			}
		}
	}

	ASSERT( lefttopIndex != -1 );
	ASSERT( leftbottomIndex != -1 );
	ASSERT( righttopIndex != -1 );
	ASSERT( rightbottomIndex != -1 );

	// 增强的双线性插值确定当前点的地球坐标.
	// 双线性插值是三次线性插值. 只适用于校正点为矩形的情况下.
	// 而我们的校正点的地球坐标不是矩形的, 所以不适用普通的双线性插值算法, 这里扩展了一下.
	// 第一次算出的临时线性插值点的纬度由一次线性插值算出.
	ASSERT( pointsAround.size() == 4 );
//	ASSERT( pointsAround[ lefttopIndex ].m_marsLongitude == pointsAround[ leftbottomIndex ].m_marsLongitude );
//	ASSERT( pointsAround[ lefttopIndex ].m_marsLatitude == pointsAround[ righttopIndex ].m_marsLatitude );
//	ASSERT( pointsAround[ rightbottomIndex].m_marsLongitude == pointsAround[ righttopIndex ].m_marsLongitude );
//	ASSERT( pointsAround[ rightbottomIndex].m_marsLatitude == pointsAround[ leftbottomIndex ].m_marsLatitude );
	
	//////////////////////////////////////////////////////////////////////////
	// 用双线性插值求地球坐标经度.
	// 先在经度方向上线性插值.
	double topEarthLongitude = this->LinearInterpolate( 
		pointsAround[lefttopIndex].m_marsLongitude, pointsAround[lefttopIndex].m_earthLongitude,
		pointsAround[righttopIndex].m_marsLongitude, pointsAround[righttopIndex].m_earthLongitude,
		marsLongitude );
	double topMarsLatitude = this->LinearInterpolate(
		pointsAround[lefttopIndex].m_marsLongitude, pointsAround[lefttopIndex].m_marsLatitude,
		pointsAround[righttopIndex].m_marsLongitude, pointsAround[righttopIndex].m_marsLatitude,
		marsLongitude );

	double bottomEarthLongitude = this->LinearInterpolate( 
		pointsAround[leftbottomIndex].m_marsLongitude, pointsAround[leftbottomIndex].m_earthLongitude,
		pointsAround[rightbottomIndex].m_marsLongitude, pointsAround[rightbottomIndex].m_earthLongitude,
		marsLongitude );
	double bottomMarsLatitude = this->LinearInterpolate(
		pointsAround[leftbottomIndex].m_marsLongitude, pointsAround[leftbottomIndex].m_marsLatitude,
		pointsAround[rightbottomIndex].m_marsLongitude, pointsAround[rightbottomIndex].m_marsLatitude,
		marsLongitude );
	
	// 在纬度上对刚刚得出的两个插值点进行插值.
	earthLongitude = this->LinearInterpolate( 
		topMarsLatitude, topEarthLongitude, bottomMarsLatitude, bottomEarthLongitude, marsLatitude );

	//////////////////////////////////////////////////////////////////////////
	// 用双线性插值求地球坐标纬度.
	// 先在经度方向上线性插值.
	double topEarthLatitude = this->LinearInterpolate(
		pointsAround[lefttopIndex].m_marsLongitude, pointsAround[ lefttopIndex ].m_earthLatitude,
		pointsAround[righttopIndex].m_marsLongitude, pointsAround[righttopIndex].m_earthLatitude,
		marsLongitude );

	double bottomEarthLatitude = this->LinearInterpolate(
		pointsAround[leftbottomIndex].m_marsLongitude, pointsAround[leftbottomIndex].m_earthLatitude,
		pointsAround[rightbottomIndex].m_marsLongitude, pointsAround[rightbottomIndex].m_earthLatitude,
		marsLongitude );

	// 在纬度上对上面两个插值点再次进行插值.
	earthLatitude = this->LinearInterpolate( 
		topMarsLatitude, topEarthLatitude, bottomMarsLatitude, bottomEarthLatitude, marsLatitude );

}

double CChinaMapDeviationFix::LinearInterpolate( double x1, double y1, double x2, double y2, double x )
{
	return ( y1 + ( x - x1 ) * (y2-y1) / (x2-x1) );
}