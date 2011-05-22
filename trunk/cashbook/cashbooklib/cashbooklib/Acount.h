#pragma once

/** 账户. */

#include "portabledefine.h"

typedef tstring TAcountID;
typedef tstring TAcountClassID;

class CAcount
{
public:
	CAcount(void);
	virtual ~CAcount(void);



private:
	/** 账户ID. */
	TAcountID m_acountId;

	/** 账户类别ID */
	TAcountClassID m_acountClassId;

	/** 账户初始时间, 以及创建时的起始值. 
	*	账户从初始时间开始累记各项帐目. 
	*/
	time_t m_acountInitTime;
	double m_acountInitValue;
};

