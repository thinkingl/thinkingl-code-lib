#include "dipcommon.h"

#ifdef WIN32
#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "MainFrm.h"
#include "DigitalImageProcessingDoc.h"
#include "DigitalImageProcessingView.h"
#endif

#ifdef WIN32
CDigitalImageProcessingView *GetActiveDIPView()
{
	// ʵ�������ڳ���ǰ���ĵ���ͼ�Ľṹ.
	CDigitalImageProcessingView * pActiveView = NULL;

	CMainFrame *pMain = dynamic_cast<CMainFrame*>( AfxGetMainWnd() );
	ASSERT( pMain );
	if ( pMain )
	{
		CFrameWnd *pFrame = pMain->GetActiveFrame();

		pActiveView = dynamic_cast<CDigitalImageProcessingView *>( pFrame->GetActiveView() );
	}
		
	return pActiveView;
}
CPropertiesWnd *GetPropertyWnd()
{
	CMainFrame *pMain = dynamic_cast<CMainFrame*>( AfxGetMainWnd() );
	ASSERT( pMain );
	if ( pMain )
	{
		return pMain->GetPropertyWnd();
	}
	else
	{
		return 0;
	}
}
#endif


