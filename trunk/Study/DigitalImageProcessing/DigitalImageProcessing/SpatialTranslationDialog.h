#pragma once


// CSpatialTranslationDialog �Ի���
class CDigitalImage;
class CSpatialTranslationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSpatialTranslationDialog)

public:
	CSpatialTranslationDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpatialTranslationDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_FUNCTION_SPATIAL_TRANSLATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	void ReleaseImage();
private:
	CDigitalImage *m_pImage;
public:
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	int m_newCoordX;
	int m_newCoordY;
};
