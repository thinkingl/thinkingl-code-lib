#include "picsave.h"
#include <QtWidgets/QApplication>
#include <QDebug>

// Æ¤·ô.
void InstallSkin(QApplication* pApp)
{
	if ( !pApp )
	{
		qDebug() << "App is null!";
		return;
	}
	QFile qssFile( "./pic.qss" );
	if ( !qssFile.open( QFile::ReadOnly ) )
	{
		qDebug() << "Open qss file fail!";
	}
	else
	{
		QString qssContent = qssFile.readAll();
		pApp->setStyleSheet( qssContent );
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	InstallSkin(&a);

	picsave w;
	w.show();
	return a.exec();
}
