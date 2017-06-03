#include <QCoreApplication>
#include <QCommandLineParser>
#include "proxychannel.h"

#include "udpserver.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("v");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("mode", QCoreApplication::translate("main", "Work mode of V [client] or [server]."));
    parser.addPositionalArgument("localport", QCoreApplication::translate("main", "Local port to listen."));
    parser.addPositionalArgument("destip", QCoreApplication::translate("main", "Destination ip addr to trans."));
    parser.addPositionalArgument("destport", QCoreApplication::translate("main", "Destination port to trans."));

    // Process the actual command line arguments given by the user
    parser.process( a );

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    QString workMode = args.at(0);

    int localPort = args.at(1).toInt();
    QString destIP = args.at(2);
    int destPort = args.at(3).toInt();

    if( workMode.toLower().compare( "client") == 0 )
    {
        ProxyChannel* proxyClient = new ProxyChannel(&a);
        proxyClient->Start( localPort, destIP, destPort );
    }
    else if( workMode.toLower().compare( "server" ) == 0 )
    {
        qDebug() << "Server mode run!";
        UDPServer* proxyServer = new UDPServer(&a);
        bool bOk = proxyServer->Start( localPort, destIP, destPort );
        if( !bOk )
        {
            qDebug() << "Proxy server start fail! port:[" << localPort << "]";
        }
        else
        {
            qDebug() << "Proxy server start at port:[" << localPort << "]";
        }
    }
    else
    {
        qDebug() << "Invalid work mode!";
    }
    //ProxyChannel proxy(&a);
    //proxy.Start( localPort, destIP, destPort );

    return a.exec();
}
