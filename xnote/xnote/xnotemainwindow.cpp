#include "xnotemainwindow.h"
#include "ui_xnotemainwindow.h"

XNoteMainWindow::XNoteMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::XNoteMainWindow)
{
    ui->setupUi(this);
}

XNoteMainWindow::~XNoteMainWindow()
{
    delete ui;
}
