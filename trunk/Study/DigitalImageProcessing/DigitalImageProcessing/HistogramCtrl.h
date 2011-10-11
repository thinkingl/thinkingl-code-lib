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

	/** ����ֱ��ͼ����. */
	bool SetHistoramData( int maxDataValue, const THistogramData& data );

	/** ������ɫ. */

private:
	THistogramData m_histogramData;
	/** ����Value, ��ʾY�����ֵ. */
	int m_maxDataValue;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


