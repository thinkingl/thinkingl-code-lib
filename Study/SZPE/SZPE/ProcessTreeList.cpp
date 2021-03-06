#include "StdAfx.h"
#include "ProcessTreeList.h"
#include "SZPE.h"


CProcessTreeList::CProcessTreeList(void)
{
}


CProcessTreeList::~CProcessTreeList(void)
{
}

CProcessTreeList::CTreeItem * CProcessTreeList::InsertItem( const CProcess& process )
{
	if( NULL == this->GetRootItem() )
	{
		SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FLATSB);

		LPTSTR lpszCols[] = {_T("Process"), _T( "PID" ), _T( "CPU" ), 
			_T( "Private Bytes (K)" ), _T( "Working Set (K)" ), _T( "Description" ),
			_T( "Company Name" ), 0};
		LV_COLUMN   lvColumn;
		//initialize the columns
		lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 200;
		for(int x = 0; lpszCols[x]!=NULL; x++)
		{
			//make the secondary columns smaller
			if(x)
			  lvColumn.cx = 150;

			lvColumn.pszText = lpszCols[x];
			InsertColumn(x,&lvColumn);
		}

		CItemInfo *pRoot = new CItemInfo();
		TCHAR szComputerName[MAX_PATH] = { 0 };
		DWORD dwLen = MAX_PATH;
		::GetComputerName( szComputerName, &dwLen );
		pRoot->SetItemText( szComputerName );
		this->CreateTreeCtrl( pRoot );
	}

	if( NULL != this->GetRootItem() )
	{
		CItemInfo *pItemInfo = new CItemInfo();
		pItemInfo->SetItemText( process.GetName().c_str() );
		
		CString strPID;
		strPID.Format( _T( "%d" ), process.GetPID() ); 
		pItemInfo->AddSubItemText( strPID );

		CString strCPU;
		pItemInfo->AddSubItemText( strCPU );

		CString strMemPrivate;
		strMemPrivate.Format( _T( "%d" ), process.GetMemPrivate()/1024 );
		pItemInfo->AddSubItemText( strMemPrivate );

		CString strMemWorkingset;
		strMemWorkingset.Format( _T( "%d" ), process.GetMemWorkingSet()/1024 );
		pItemInfo->AddSubItemText( strMemWorkingset );

		CTreeItem *pItem = __super::InsertItem( this->GetRootItem(),
			pItemInfo, TRUE );
		return pItem;
	}
	else
		return NULL;	
}BEGIN_MESSAGE_MAP(CProcessTreeList, CSuperGridCtrl)
	ON_WM_CREATE()
	END_MESSAGE_MAP()


int CProcessTreeList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//associate imagelist with listviewctrl	
	if(!m_image.Create(IDB_FILE_VIEW,16,1,RGB(0, 255, 255)))
		return -1;
	
	SetImageList(&m_image,LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;
	

	return 0;
}
