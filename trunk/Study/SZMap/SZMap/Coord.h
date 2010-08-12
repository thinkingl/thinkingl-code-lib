#pragma once
class CCoord
{
public:
	CCoord(void);
	virtual ~CCoord(void);

	CCoord( double latitude, double longitude, int nZLevel );
	CCoord( const CCoord& another );
	CCoord& operator = ( const CCoord& anoter );

	/** 获取设置纬度。 */
	double GetLatitude()const;
	void SetLatitude( double latitude );

	/** 获取设置经度 */
	double GetLongitude() const;
	void SetLongitude( double longtitude );

	/** 获取设置放大等级。 */
	int GetZLevel() const;
	void SetZLevel( int zlevel );

private:
	/** 纬度 */
	double m_dbLatitude;
	/** 经度*/
	double m_dbLongitude;
	/** 放大等级。 */
	int m_nZLevel;
};

