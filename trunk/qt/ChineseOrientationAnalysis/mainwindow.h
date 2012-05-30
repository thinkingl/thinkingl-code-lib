#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    void InitTrainFileList();
private:
    Ui::MainWindow *ui;
private:
    QStandardItemModel *m_trainTextFileListModel;
    //QStringList m_trainTextFileList;
};

#endif // MAINWINDOW_H
