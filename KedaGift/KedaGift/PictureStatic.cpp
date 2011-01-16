// PictureStatic.cpp : 实现文件
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



// CPictureStatic 消息处理程序




void CPictureStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
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