#pragma once

#include <vector>
// CHistogramCtrl

#include "DigitalImage.h"

class CHistogramCtrl : public CStatic
{
	DECLARE_DYNAMIC(CHistogramCtrl)

public:
	CHistogramCtrl();
	virtual ~CHistogramCtrl();

	

	/** 设置直方图数据. */
	bool SetHistoramData( const CDigitalImage::THistogramData& data );

	/** 设置颜色. */

private:
	CDigitalImage::THistogramData m_histogramData;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


