#include <QCoreApplication>
#include <QTcpServer>
#include <QCommandLineParser>
#include "echotcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("echo-test");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Echo test");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("localport", QCoreApplication::translate("main", "Local port to listen."));

    // Process the actual command line arguments given by the user
    parser.process( a );

    const QStringList args = parser.positionalArguments();

    quint16 localPort = 22222;
    localPort = args.at(0).toInt();

    EchoTCPServer echo( &a );

    echo.Start( localPort );


    return a.exec();
}
