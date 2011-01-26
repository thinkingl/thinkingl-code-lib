#ifndef _3ASSN_H
#define _3ASSN_H
#include "osp.h"
#include "aaasdk.h"

class CSdk3AIns:public CInstance
{
public:
	void InstanceEntry(CMessage *const pcMsg){};
    void DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp);
private:
	void Init3ASSdk();
};

typedef zTemplate<CSdk3AIns,1> CSdk3AApp;
#endif //_3ASSN_H