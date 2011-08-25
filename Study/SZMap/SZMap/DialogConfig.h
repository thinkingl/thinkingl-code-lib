#pragma once

#include "IniConfigFile.h"

// CDialogConfig 对话框

class CDialogConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogConfig)

public:
	CDialogConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:


public:
	virtual BOOL OnInitDialog();
	CString m_strImgDir;
	afx_msg void OnBnClickedButtonExplorer();
	afx_msg void OnBnClickedOk();
	CString m_strImgSubDir;


	CString m_strDatabasePath;
	afx_msg void OnBnClickedButtonExplorerDatabase();
	afx_msg void OnBnClickedButtonDatabaseImport();
};
