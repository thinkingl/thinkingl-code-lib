/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu May 31 19:40:09 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *labelTrainningFileDir;
    QPushButton *pushButtonExplorer;
    QPushButton *pushButtonUpdate;
    QListView *trainTextFileListView;
    QPushButton *pushButtonParse;
    QLabel *label;
    QLineEdit *lineEditMaxNum;
    QPushButton *pushButtonSave;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QListWidget *listWidgetReport;
    QMenuBar *menuBar;
    QMenu *menu_1110332040;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1015, 731);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        groupBox = new QGroupBox(splitter_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelTrainningFileDir = new QLabel(groupBox);
        labelTrainningFileDir->setObjectName(QString::fromUtf8("labelTrainningFileDir"));

        gridLayout_2->addWidget(labelTrainningFileDir, 0, 0, 1, 4);

        pushButtonExplorer = new QPushButton(groupBox);
        pushButtonExplorer->setObjectName(QString::fromUtf8("pushButtonExplorer"));

        gridLayout_2->addWidget(pushButtonExplorer, 0, 5, 1, 1);

        pushButtonUpdate = new QPushButton(groupBox);
        pushButtonUpdate->setObjectName(QString::fromUtf8("pushButtonUpdate"));

        gridLayout_2->addWidget(pushButtonUpdate, 0, 6, 1, 1);

        trainTextFileListView = new QListView(groupBox);
        trainTextFileListView->setObjectName(QString::fromUtf8("trainTextFileListView"));

        gridLayout_2->addWidget(trainTextFileListView, 1, 0, 1, 7);

        pushButtonParse = new QPushButton(groupBox);
        pushButtonParse->setObjectName(QString::fromUtf8("pushButtonParse"));

        gridLayout_2->addWidget(pushButtonParse, 2, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label, 2, 1, 1, 1);

        lineEditMaxNum = new QLineEdit(groupBox);
        lineEditMaxNum->setObjectName(QString::fromUtf8("lineEditMaxNum"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditMaxNum->sizePolicy().hasHeightForWidth());
        lineEditMaxNum->setSizePolicy(sizePolicy);
        lineEditMaxNum->setInputMethodHints(Qt::ImhPreferNumbers);

        gridLayout_2->addWidget(lineEditMaxNum, 2, 2, 1, 1);

        pushButtonSave = new QPushButton(groupBox);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));

        gridLayout_2->addWidget(pushButtonSave, 2, 3, 1, 1);

        splitter_2->addWidget(groupBox);

        gridLayout->addWidget(splitter_2, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listWidgetReport = new QListWidget(groupBox_2);
        listWidgetReport->setObjectName(QString::fromUtf8("listWidgetReport"));

        gridLayout_3->addWidget(listWidgetReport, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1015, 17));
        menu_1110332040 = new QMenu(menuBar);
        menu_1110332040->setObjectName(QString::fromUtf8("menu_1110332040"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_1110332040->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\203\205\346\204\237\350\257\215\345\210\206\346\236\220_\346\235\216\344\271\213\345\205\264_1110332040", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "\350\256\255\347\273\203\346\226\207\346\234\254\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        labelTrainningFileDir->setText(QApplication::translate("MainWindow", "./project_train_100", 0, QApplication::UnicodeUTF8));
        pushButtonExplorer->setText(QApplication::translate("MainWindow", "\346\265\217\350\247\210", 0, QApplication::UnicodeUTF8));
        pushButtonUpdate->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260", 0, QApplication::UnicodeUTF8));
        pushButtonParse->setText(QApplication::translate("MainWindow", "\345\210\206\346\236\220", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\346\234\200\345\244\232\346\235\241\346\225\260: ", 0, QApplication::UnicodeUTF8));
        lineEditMaxNum->setText(QApplication::translate("MainWindow", "100", 0, QApplication::UnicodeUTF8));
        pushButtonSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\347\273\223\346\236\234", 0, QApplication::UnicodeUTF8));
        menu_1110332040->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
