#pragma once

/** 单元测试.
*	测试随机性. 
*/
class CUnitTest
{
public:
	CUnitTest(void);
	~CUnitTest(void);

private:
	/** 测试随机选号的公平性. 
	*	随机足够多的次数,看分布是否均匀.
	*/
	void UTRadomPick();
};

