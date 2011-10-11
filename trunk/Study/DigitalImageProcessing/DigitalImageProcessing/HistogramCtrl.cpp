// HistogramCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "HistogramCtrl.h"


// CHistogramCtrl

IMPLEMENT_DYNAMIC(CHistogramCtrl, CStatic)

CHistogramCtrl::CHistogramCtrl()
{

}

CHistogramCtrl::~CHistogramCtrl()
{
}


BEGIN_MESSAGE_MAP(CHistogramCtrl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CHistogramCtrl 消息处理程序




void CHistogramCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	CRect rcClient;
	this->GetClientRect( rcClient );

	dc.Rectangle( rcClient );

	CRect rcHistogram = rcClient;

	for ( int i=0; i<rcHistogram.Width(); ++i )
	{
		int beginX = i + rcHistogram.left;
		int beginY = rcHistogram.top;

		dc.MoveTo( beginX, beginY );

		int endX = beginX;
		
		// 算出这个地方对应的数据序号.
		int dataIndex = i * this->m_histogramData.size() / rcClient.Width();
		int endY = 0;
		if ( dataIndex >=0 && dataIndex < m_histogramData.size() )
		{
			// 算出数据值对应的图中的像素点数.
			endY = rcHistogram.Height() - ( m_histogramData[ dataIndex ] * rcClient.Height() / m_maxDataValue );
		}
		else
		{
			ASSERT( FALSE );
		}

		dc.LineTo( endX, endY );
	}
}

bool CHistogramCtrl::SetHistoramData( int maxDataValue, const THistogramData& data )
{
	this->m_maxDataValue = maxDataValue;
	this->m_histogramData = data ;
	return true;
}
