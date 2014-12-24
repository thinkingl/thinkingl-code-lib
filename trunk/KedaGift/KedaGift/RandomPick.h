#pragma once

/** ���ѡ��. */

#include "KedaGiftDefine.h"
#include "EmployerGiftConfig.h"

class CRandomPick
{
public:
	CRandomPick(void);
	~CRandomPick(void);
		

	/** �����б�. */
	void SetEmployerList( const TEmployerList& employerList );

	/** ��ȡ�����Ա�н�.. 
	*	ÿ����ֻ��һ���н�����,���ظ�.
	*/
	CEmployer GetLukyOne( int raund );

	/** ���ѡȡһ����û���н���,������ʾ. */
	CEmployer RandomPickOneNoGiftToShow() const;

	/** �����н����.. */
	void ResetGift();

private:
	/** ���ѡ��һ�����. */
	int GetRadomNum( int nBegin, int nEnd ) const;

	/** ��ʼ���Ѿ��н�������. */
	void InitAllReadyGifted();

	/** �����Ѿ��н���ͬ־. */
	void HandleGiftedLucyMan( int nIndex );

	/** д��־. */
	CArchive& Log();
private:
	/** ����Ա��. 
	*	�� 0 - m_nLuckyEnd ���Ѿ��н����ų�����Ա���.
	*/
	TEmployerList m_tAllEmployer;

	/** �Ѿ��н����˱��Ƶ�ǰ��. ����ǵ�һ��û���н�����. */
	int m_nFirstNoGift;

	/** �����ļ�. */
	CEmployerGiftConfig m_giftConfig;

	/** ��־�ļ�. */
	CFile m_fileLog;
	CArchive *m_pArchiveLog;
};

