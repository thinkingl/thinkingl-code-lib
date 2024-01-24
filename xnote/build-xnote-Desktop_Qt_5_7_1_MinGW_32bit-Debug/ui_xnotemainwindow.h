/********************************************************************************
** Form generated from reading UI file 'xnotemainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XNOTEMAINWINDOW_H
#define UI_XNOTEMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XNoteMainWindow
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XNoteMainWindow)
    {
        if (XNoteMainWindow->objectName().isEmpty())
            XNoteMainWindow->setObjectName(QStringLiteral("XNoteMainWindow"));
        XNoteMainWindow->resize(400, 300);
        menuBar = new QMenuBar(XNoteMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        XNoteMainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XNoteMainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        XNoteMainWindow->addToolBar(mainToolBar);
        centralWidget = new QWidget(XNoteMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        XNoteMainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(XNoteMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        XNoteMainWindow->setStatusBar(statusBar);

        retranslateUi(XNoteMainWindow);

        QMetaObject::connectSlotsByName(XNoteMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *XNoteMainWindow)
    {
        XNoteMainWindow->setWindowTitle(QApplication::translate("XNoteMainWindow", "XNoteMainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class XNoteMainWindow: public Ui_XNoteMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XNOTEMAINWINDOW_H
