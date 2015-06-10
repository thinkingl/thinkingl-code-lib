// ConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "XPlayer.h"
#include "ConfigDialog.h"
#include "afxdialogex.h"


// CConfigDialog 对话框

IMPLEMENT_DYNAMIC(CConfigDialog, CDialogEx)

CConfigDialog::CConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDialog::IDD, pParent)
{

}

CConfigDialog::~CConfigDialog()
{
}

void CConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDialog, CDialogEx)
END_MESSAGE_MAP()


// CConfigDialog 消息处理程序
