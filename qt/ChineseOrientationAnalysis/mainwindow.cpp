#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "libs_win32/ICTCLAS50.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_trainTextFileListModel = new QStandardItemModel ();
    ui->trainTextFileListView->setModel( m_trainTextFileListModel );

    this->InitTrainFileList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开文件"), "", tr("*.*"));

     QStandardItem* item1    = new QStandardItem(fileName);
     m_trainTextFileListModel->appendRow( item1 );

 //    ICTCLAS_Init();
  // m_trainTextFileList += fileName;

 //  ui->trainTextFileListView->update( );
    //QMessageBox::Critical( this, "d", "d" );
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
//    51            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
//    52
//    53        if (fileName != "") {
//    54            QFile file(fileName);
//    55            if (!file.open(QIODevice::ReadOnly)) {
//    56                QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
//    57                return;
//    58            }
//    59            QTextStream in(&file);
//    60            textEdit->setText(in.readAll());
//    61            file.close();
//    62        }
}

void MainWindow::InitTrainFileList()
{
    const int fileNum = 100;

}
