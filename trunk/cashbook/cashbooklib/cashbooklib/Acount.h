#pragma once

/** �˻�. */

#include "portabledefine.h"

typedef tstring TAcountID;
typedef tstring TAcountClassID;

class CAcount
{
public:
	CAcount(void);
	virtual ~CAcount(void);



private:
	/** �˻�ID. */
	TAcountID m_acountId;

	/** �˻����ID */
	TAcountClassID m_acountClassId;

	/** �˻���ʼʱ��, �Լ�����ʱ����ʼֵ. 
	*	�˻��ӳ�ʼʱ�俪ʼ�ۼǸ�����Ŀ. 
	*/
	time_t m_acountInitTime;
	double m_acountInitValue;
};

