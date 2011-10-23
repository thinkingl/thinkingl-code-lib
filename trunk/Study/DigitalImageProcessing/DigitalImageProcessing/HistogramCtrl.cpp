// HistogramCtrl.cpp : ʵ���ļ�
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



// CHistogramCtrl ��Ϣ�������




void CHistogramCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	CRect rcClient;
	this->GetClientRect( rcClient );

	dc.Rectangle( rcClient );

	if ( m_histogramData.empty() )
	{
		return;
	}

	// �ҳ�ֱ��ͼ������ֵ.
	int maxHistogramDataValue = 0;
	for ( size_t i=0; i<m_histogramData.size(); ++i )
	{
		if( m_histogramData[i] > maxHistogramDataValue )
		{
			maxHistogramDataValue = m_histogramData[i];
		}
	}
	maxHistogramDataValue *= 1.5;

	CRect rcHistogram = rcClient;

	for ( int i=0; i<rcHistogram.Width(); ++i )
	{
		int beginX = i + rcHistogram.left;
		int beginY = rcHistogram.top;

		dc.MoveTo( beginX, beginY );

		int endX = beginX;
		
		// �������ط���Ӧ���������.
		size_t dataIndex = i * this->m_histogramData.size() / rcClient.Width();
		int endY = 0;
		if ( dataIndex >=0 && dataIndex < m_histogramData.size() )
		{
			// �������ֵ��Ӧ��ͼ�е����ص���.
			endY = rcHistogram.Height() - ( m_histogramData[ dataIndex ] * rcClient.Height() / maxHistogramDataValue );
		}
		else
		{
			ASSERT( FALSE );
		}

		dc.LineTo( endX, endY );
	}
}

bool CHistogramCtrl::SetHistoramData( const CDigitalImage::THistogramData& data )
{
	this->m_histogramData = data ;
	if ( this->GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return true;
}
