/********************************************************************************
** Form generated from reading UI file 'clientwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTWINDOW_H
#define UI_CLIENTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientWindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuClientWindow;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ClientWindow)
    {
        if (ClientWindow->objectName().isEmpty())
            ClientWindow->setObjectName("ClientWindow");
        ClientWindow->resize(500, 500);
        centralwidget = new QWidget(ClientWindow);
        centralwidget->setObjectName("centralwidget");
        ClientWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ClientWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 500, 25));
        menuClientWindow = new QMenu(menubar);
        menuClientWindow->setObjectName("menuClientWindow");
        ClientWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ClientWindow);
        statusbar->setObjectName("statusbar");
        ClientWindow->setStatusBar(statusbar);

        menubar->addAction(menuClientWindow->menuAction());

        retranslateUi(ClientWindow);

        QMetaObject::connectSlotsByName(ClientWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ClientWindow)
    {
        ClientWindow->setWindowTitle(QCoreApplication::translate("ClientWindow", "MainWindow", nullptr));
        menuClientWindow->setTitle(QCoreApplication::translate("ClientWindow", "ClientWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClientWindow: public Ui_ClientWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTWINDOW_H
