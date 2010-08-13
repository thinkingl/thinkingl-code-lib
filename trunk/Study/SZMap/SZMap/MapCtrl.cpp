// MapCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "SZMap.h"
#include "MapCtrl.h"


// CMapCtrl

IMPLEMENT_DYNAMIC(CMapCtrl, CWnd)

CMapCtrl::CMapCtrl()
{

}

CMapCtrl::~CMapCtrl()
{
}


BEGIN_MESSAGE_MAP(CMapCtrl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMapCtrl 消息处理程序




void CMapCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
}
