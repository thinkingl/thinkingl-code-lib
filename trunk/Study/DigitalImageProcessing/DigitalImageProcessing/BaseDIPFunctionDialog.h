#pragma once

class CDigitalImage;
// CBaseDIPFunctionDialog �Ի���
class CBaseDIPFunctionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDIPFunctionDialog)

public:
	CBaseDIPFunctionDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBaseDIPFunctionDialog();

// �Ի�������
	enum { IDD = IDD_BASEDIPFUNCTIONDIALOG };
protected:
	void Priview();
	/**	����Ԥ��. */
	void ResetPreview();
	void Apply();

	CDigitalImage *GetImage();
	void ReleaseImage();

	/** ��Ҫ������ʵ���������, ��ͼ����д���, �������ͼ�񽫱���ʾ����. */
// 	virtual void OnPriview( CDigitalImage* pImg );
// 
// 	virtual void OnApply( CDigitalImage *pImg );
	virtual void OnDIP( CDigitalImage *pImg ) = 0;
private:
	CDigitalImage *m_pImage;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
