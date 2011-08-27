#include "chinamapdeviationfix.h"

#include "szmapconfig.h"
#include "Log.h"

// �㵽�����ݵľ����� 0.025 ����γ��.
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

	// Ӧ�õõ�����4����.
	if ( pointsAround.size() < 4 )
	{
		ASSERT( FALSE );
		Log() << "MarsToEarth ��Χû���ҵ��㹻��У����!" << endl;
		return;
	}
	ASSERT( pointsAround.size() == 4 );

	// ȷ������У�����λ��.
	int lefttopIndex = -1;
	int leftbottomIndex = -1;
	int righttopIndex = -1;
	int rightbottomIndex = -1;
	for ( int i=0; i<pointsAround.size(); ++i )
	{
		if ( pointsAround[i].m_marsLongitude > marsLongitude )	// �й��Ƕ���. ����(��)��Ϊ����73, �(��)Ϊ����135��.
		{
			// ˵�� У������ �������� ��(��)��
			if ( pointsAround[i].m_marsLatitude > marsLatitude )
			{
				// ˵��У������ �������� ��(��)��.
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
				// ˵��У������ �������� ��(��)��.
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

	// ��ǿ��˫���Բ�ֵȷ����ǰ��ĵ�������.
	// ˫���Բ�ֵ���������Բ�ֵ. ֻ������У����Ϊ���ε������.
	// �����ǵ�У����ĵ������겻�Ǿ��ε�, ���Բ�������ͨ��˫���Բ�ֵ�㷨, ������չ��һ��.
	// ��һ���������ʱ���Բ�ֵ���γ����һ�����Բ�ֵ���.
	ASSERT( pointsAround.size() == 4 );
//	ASSERT( pointsAround[ lefttopIndex ].m_marsLongitude == pointsAround[ leftbottomIndex ].m_marsLongitude );
//	ASSERT( pointsAround[ lefttopIndex ].m_marsLatitude == pointsAround[ righttopIndex ].m_marsLatitude );
//	ASSERT( pointsAround[ rightbottomIndex].m_marsLongitude == pointsAround[ righttopIndex ].m_marsLongitude );
//	ASSERT( pointsAround[ rightbottomIndex].m_marsLatitude == pointsAround[ leftbottomIndex ].m_marsLatitude );
	
	//////////////////////////////////////////////////////////////////////////
	// ��˫���Բ�ֵ��������꾭��.
	// ���ھ��ȷ��������Բ�ֵ.
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
	
	// ��γ���϶Ըոյó���������ֵ����в�ֵ.
	earthLongitude = this->LinearInterpolate( 
		topMarsLatitude, topEarthLongitude, bottomMarsLatitude, bottomEarthLongitude, marsLatitude );

	//////////////////////////////////////////////////////////////////////////
	// ��˫���Բ�ֵ���������γ��.
	// ���ھ��ȷ��������Բ�ֵ.
	double topEarthLatitude = this->LinearInterpolate(
		pointsAround[lefttopIndex].m_marsLongitude, pointsAround[ lefttopIndex ].m_earthLatitude,
		pointsAround[righttopIndex].m_marsLongitude, pointsAround[righttopIndex].m_earthLatitude,
		marsLongitude );

	double bottomEarthLatitude = this->LinearInterpolate(
		pointsAround[leftbottomIndex].m_marsLongitude, pointsAround[leftbottomIndex].m_earthLatitude,
		pointsAround[rightbottomIndex].m_marsLongitude, pointsAround[rightbottomIndex].m_earthLatitude,
		marsLongitude );

	// ��γ���϶�����������ֵ���ٴν��в�ֵ.
	earthLatitude = this->LinearInterpolate( 
		topMarsLatitude, topEarthLatitude, bottomMarsLatitude, bottomEarthLatitude, marsLatitude );

}

double CChinaMapDeviationFix::LinearInterpolate( double x1, double y1, double x2, double y2, double x )
{
	return ( y1 + ( x - x1 ) * (y2-y1) / (x2-x1) );
}