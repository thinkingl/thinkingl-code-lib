// UnitTestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "UnitTestDialog.h"
#include "afxdialogex.h"
#include "matrix.h"

// CUnitTestDialog 对话框

IMPLEMENT_DYNAMIC(CUnitTestDialog, CDialogEx)

CUnitTestDialog::CUnitTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnitTestDialog::IDD, pParent)
{

}

CUnitTestDialog::~CUnitTestDialog()
{
}

void CUnitTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HISTOGRAM, m_histogramCtrl);
}


BEGIN_MESSAGE_MAP(CUnitTestDialog, CDialogEx)
END_MESSAGE_MAP()


// CUnitTestDialog 消息处理程序


BOOL CUnitTestDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDigitalImage::THistogramData hisData;
	hisData.push_back( 128 );
	hisData.push_back( 64 );

	this->m_histogramCtrl.SetHistoramData( hisData );

	CMatrix<int>::CMatrixDataList dataList;
	dataList.push_back( 1 );
	dataList.push_back( 0 );
	dataList.push_back( 2 );
	dataList.push_back( -1 );
	dataList.push_back( 3 );
	dataList.push_back( 1 );
	CMatrix<int> m1( 3, 2, dataList );

	CMatrix<int> m2( 2, 3, 0, 1 );
	m2.Value( 0, 0 ) = 3;
	m2.Value( 1, 0 ) = 1;
	m2.Value( 0, 1 ) = 2;
	m2.Value( 1, 1 ) = 1;
	m2.Value( 0, 2 ) = 1;
	m2.Value( 1, 2 ) = 0;

	CMatrix<int> result = m1 * m2;

	result.Value( 10, 20 );


	CMatrix<int>::CMatrixDataList dettestData;
	dettestData.push_back( -2 );
	dettestData.push_back( 2 );
	dettestData.push_back( 3 );
	dettestData.push_back( -1 );
	dettestData.push_back( 1 );
	dettestData.push_back( 3 );
	dettestData.push_back( 2 );
	dettestData.push_back( 0 );
	dettestData.push_back( -1 );
	CMatrix<int> dettestM( 3, 3, dettestData );
	int det = 0;
	dettestM.GetDeterminant( det );
	assert( det == 6 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

