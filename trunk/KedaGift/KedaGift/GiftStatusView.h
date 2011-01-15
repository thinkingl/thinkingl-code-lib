#pragma once
#include "afxcmn.h"


// CGiftStatusView �Ի���

#include "RandomPick.h"

class CGiftStatusView : public CDialogEx
{
	DECLARE_DYNAMIC(CGiftStatusView)

public:
	CGiftStatusView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGiftStatusView();

// �Ի�������
	enum { IDD = IDD_DIALOG_GIFT_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listGiftStatus;

	void SetRandomPicker( CRandomPick* pPicker );

private:
	enum EListGiftColumnHead
	{
		ColHead_SequenceNo,	// ���.
		ColHead_KedaNo,
		ColHead_Name,
		ColHead_Time,
	};

	CRandomPick* m_pRandomPick;
public:
	afx_msg void OnBnClickedClearAll();
};
