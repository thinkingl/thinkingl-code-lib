#ifndef XNOTEMAINWINDOW_H
#define XNOTEMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class XNoteMainWindow;
}

class XNoteMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit XNoteMainWindow(QWidget *parent = 0);
    ~XNoteMainWindow();

private:
    Ui::XNoteMainWindow *ui;
};

#endif // XNOTEMAINWINDOW_H
