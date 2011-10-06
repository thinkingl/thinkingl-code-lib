
// DigitalImageProcessingDoc.cpp : CDigitalImageProcessingDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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


// CDigitalImageProcessingDoc 构造/析构

CDigitalImageProcessingDoc::CDigitalImageProcessingDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDigitalImageProcessingDoc::~CDigitalImageProcessingDoc()
{
}

BOOL CDigitalImageProcessingDoc::OnNewDocument()
{
	// 不支持新建!
	return FALSE;

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDigitalImageProcessingDoc 序列化

void CDigitalImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDigitalImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CDigitalImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CDigitalImageProcessingDoc 诊断

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


// CDigitalImageProcessingDoc 命令


BOOL CDigitalImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	CImage img;
	HRESULT hr = img.Load( lpszPathName );
	if ( S_OK != hr )
	{
		AfxMessageBox( _T( "加载图片失败!" ) );
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
	// TODO: 在此添加专用代码和/或调用基类

	// 暂时不支持保存..
	return FALSE;

	return CDocument::OnSaveDocument(lpszPathName);
}
