
#include "portabledefine.h"
#include "ClassFactory.h"

using namespace std;

int main()
{
    tcout << _T( "Web Fetch run!" ) << endl;

 
	IApplication *pApp = CClassFactory::CreateApp();
	if ( pApp )
	{
		pApp->RunWebFetch();
	}

    delete pApp;
    pApp = NULL;

    return 0;
}

