
#include "portabledefine.h"
#include "WebFetcherApp.h"

using namespace std;

int main()
{
    tcout << _T( "Web Fetch run!" ) << endl;

    CWebFetcherApp app;
    app.Run();

    return 0;
}

