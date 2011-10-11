#pragma once

#include <vector>
// CHistogramCtrl

class CHistogramCtrl : public CStatic
{
	DECLARE_DYNAMIC(CHistogramCtrl)

public:
	CHistogramCtrl();
	virtual ~CHistogramCtrl();

	typedef std::vector< int > THistogramData;

	/** 设置直方图数据. */
	bool SetHistoramData( int maxDataValue, const THistogramData& data );

	/** 设置颜色. */

private:
	THistogramData m_histogramData;
	/** 最大的Value, 表示Y轴最大值. */
	int m_maxDataValue;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


