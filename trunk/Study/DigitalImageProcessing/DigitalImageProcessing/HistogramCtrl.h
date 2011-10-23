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

	

	/** ����ֱ��ͼ����. */
	bool SetHistoramData( const CDigitalImage::THistogramData& data );

	/** ������ɫ. */

private:
	CDigitalImage::THistogramData m_histogramData;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


