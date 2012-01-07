#pragma once

class CDigitalImage;
// CBaseDIPFunctionDialog 对话框
class CBaseDIPFunctionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDIPFunctionDialog)

public:
	CBaseDIPFunctionDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseDIPFunctionDialog();

// 对话框数据
	enum { IDD = IDD_BASEDIPFUNCTIONDIALOG };
protected:
	void Preview();
	/**	重置预览. */
	void CancelPreview();
	void Apply();

	CDigitalImage *GetImage();
	void ReleaseImage();

	/** 需要派生类实现这个函数, 对图像进行处理, 处理完的图像将被显示出来. */
// 	virtual void OnPriview( CDigitalImage* pImg );
// 
// 	virtual void OnApply( CDigitalImage *pImg );
protected:
	virtual void OnDIP( CDigitalImage *pImg ) = 0;
private:
	CDigitalImage *m_pImage;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
