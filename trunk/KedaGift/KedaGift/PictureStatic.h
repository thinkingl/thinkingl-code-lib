#pragma once


// CPictureStatic

class CPictureStatic : public CStatic
{
	DECLARE_DYNAMIC(CPictureStatic)

public:
	CPictureStatic();
	virtual ~CPictureStatic();

	/** ÒªÏÔÊ¾µÄÍ¼Æ¬. */
	void SetPicture( LPCTSTR strImage );

private:
	CImage *m_pImageShow;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


