#include <QCoreApplication>
#include <QCommandLineParser>
#include "proxychannel.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("v");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("localport", QCoreApplication::translate("main", "Local port to listen."));
    parser.addPositionalArgument("destip", QCoreApplication::translate("main", "Destination ip addr to trans."));
    parser.addPositionalArgument("destport", QCoreApplication::translate("main", "Destination port to trans."));

    // Process the actual command line arguments given by the user
    parser.process( a );

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    int localPort = args.at(0).toInt();
    QString destIP = args.at(1);
    int destPort = args.at(2).toInt();

    ProxyChannel proxy(&a);
    proxy.Start( localPort, destIP, destPort );

    return a.exec();
}
