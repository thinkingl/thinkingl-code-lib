// PictureStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KedaGift.h"
#include "PictureStatic.h"

#include "kedagiftcommon.h"


// CPictureStatic

IMPLEMENT_DYNAMIC(CPictureStatic, CStatic)

CPictureStatic::CPictureStatic()
{
	m_pImageShow = NULL;
}

CPictureStatic::~CPictureStatic()
{
}


BEGIN_MESSAGE_MAP(CPictureStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPictureStatic ��Ϣ�������




void CPictureStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	CRect rcClient;
	this->GetClientRect( rcClient );
	if ( this->m_pImageShow )
	{
		this->m_pImageShow->Draw( dc, rcClient );
	}
}

void CPictureStatic::SetPicture( LPCTSTR strImage )
{
	this->m_pImageShow = GetImage( strImage );
}