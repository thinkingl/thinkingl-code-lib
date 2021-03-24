#include "xnotemainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XNoteMainWindow w;
    w.show();

    return a.exec();
}
