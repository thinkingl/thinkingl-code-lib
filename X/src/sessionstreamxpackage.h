#include "sessionstream.h"

class SessionStreamXPackage : public SessionStream
{
public:
    SessionStreamXPackage( string sessionId, asio::ip::tcp::socket s, std::shared_ptr<XService> service );

protected:
    virtual void doRead();

    virtual shared_ptr<XMessage> parseXPackage( shared_ptr<XPackage> package );

    //virtual void doWrite();
private:
    void doReadHeader();
    void doReadBody( shared_ptr<XPackage> package );
};