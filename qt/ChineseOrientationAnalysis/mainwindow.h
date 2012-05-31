#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "cwordspliter.h"

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
    void OnExplorerDir();
	void OnUpdate();
	void OnParse();
	void OnSave();
private:

private:
    Ui::MainWindow *ui;
private:
    QStandardItemModel *m_trainTextFileListModel;
	CWordSpliter m_wordSpliter;
    //QStringList m_trainTextFileList;
};

#endif // MAINWINDOW_H
