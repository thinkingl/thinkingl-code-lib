
// DigitalImageProcessingDoc.cpp : CDigitalImageProcessingDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DigitalImageProcessing.h"
#endif

#include "DigitalImageProcessingDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDigitalImageProcessingDoc

IMPLEMENT_DYNCREATE(CDigitalImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CDigitalImageProcessingDoc, CDocument)
END_MESSAGE_MAP()


// CDigitalImageProcessingDoc ����/����

CDigitalImageProcessingDoc::CDigitalImageProcessingDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CDigitalImageProcessingDoc::~CDigitalImageProcessingDoc()
{
}

BOOL CDigitalImageProcessingDoc::OnNewDocument()
{
	// ��֧���½�!
	return FALSE;

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDigitalImageProcessingDoc ���л�

void CDigitalImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CDigitalImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CDigitalImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CDigitalImageProcessingDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDigitalImageProcessingDoc ���

#ifdef _DEBUG
void CDigitalImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDigitalImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDigitalImageProcessingDoc ����


BOOL CDigitalImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  �ڴ������ר�õĴ�������
	CImage img;
	HRESULT hr = img.Load( lpszPathName );
	if ( S_OK != hr )
	{
		AfxMessageBox( _T( "����ͼƬʧ��!" ) );
		return FALSE;
	}
	
	this->m_image.Load( lpszPathName );

	return TRUE;
}

CDigitalImage *CDigitalImageProcessingDoc::CloneImage() const
{
	return this->m_image.Clone();
}


BOOL CDigitalImageProcessingDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: �ڴ����ר�ô����/����û���

	// ��ʱ��֧�ֱ���..
	return FALSE;

	return CDocument::OnSaveDocument(lpszPathName);
}
