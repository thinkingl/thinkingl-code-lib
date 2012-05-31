#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
	
	MainWindow w;
    w.show();

    return a.exec();
}
