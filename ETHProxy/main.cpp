#include <QCoreApplication>

#include "ethproxyserver.h"
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("ETH Proxy");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("ETH Proxy xxx");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("localport", QCoreApplication::translate("main", "Local port to listen."));
    parser.addPositionalArgument("dest addr", QCoreApplication::translate("main", "Destination ip addr to trans."));
    parser.addPositionalArgument("destport", QCoreApplication::translate("main", "Destination port to trans."));

    // Process the actual command line arguments given by the user
    parser.process( a );

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    int localPort = args.at(0).toInt();
    QString ethPoolAddr = args.at(1);
    int ethPoolPort = args.at(2).toInt();

    ETHProxyServer server( &a );

    server.Start( localPort, ethPoolAddr, ethPoolPort );


    return a.exec();
}
