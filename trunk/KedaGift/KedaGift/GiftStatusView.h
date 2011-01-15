#pragma once
#include "afxcmn.h"


// CGiftStatusView 对话框

#include "RandomPick.h"

class CGiftStatusView : public CDialogEx
{
	DECLARE_DYNAMIC(CGiftStatusView)

public:
	CGiftStatusView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGiftStatusView();

// 对话框数据
	enum { IDD = IDD_DIALOG_GIFT_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listGiftStatus;

	void SetRandomPicker( CRandomPick* pPicker );

private:
	enum EListGiftColumnHead
	{
		ColHead_SequenceNo,	// 序号.
		ColHead_KedaNo,
		ColHead_Name,
		ColHead_Time,
	};

	CRandomPick* m_pRandomPick;
public:
	afx_msg void OnBnClickedClearAll();
};
