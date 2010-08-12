#pragma once
class CCoord
{
public:
	CCoord(void);
	virtual ~CCoord(void);

	CCoord( double latitude, double longitude, int nZLevel );
	CCoord( const CCoord& another );
	CCoord& operator = ( const CCoord& anoter );

	/** ��ȡ����γ�ȡ� */
	double GetLatitude()const;
	void SetLatitude( double latitude );

	/** ��ȡ���þ��� */
	double GetLongitude() const;
	void SetLongitude( double longtitude );

	/** ��ȡ���÷Ŵ�ȼ��� */
	int GetZLevel() const;
	void SetZLevel( int zlevel );

private:
	/** γ�� */
	double m_dbLatitude;
	/** ����*/
	double m_dbLongitude;
	/** �Ŵ�ȼ��� */
	int m_nZLevel;
};

