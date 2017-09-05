/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButtonUp;
    QPushButton *pushButtonDown;
    QPushButton *pushButtonLeft;
    QPushButton *pushButtonRight;
    QPushButton *pushButtonFollow;
    QPushButton *pushButtonFollow2;
    QPushButton *pushButtonObjectDetect;
    QPushButton *pushButtonStop;
    QPushButton *pushButtonErrorTest;
    QPushButton *pushButtonPersonDetect;
    QPushButton *QR_Code;
    QPushButton *searchQRcode;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButtonUp = new QPushButton(centralWidget);
        pushButtonUp->setObjectName(QStringLiteral("pushButtonUp"));
        pushButtonUp->setGeometry(QRect(160, 60, 75, 23));
        pushButtonDown = new QPushButton(centralWidget);
        pushButtonDown->setObjectName(QStringLiteral("pushButtonDown"));
        pushButtonDown->setGeometry(QRect(160, 100, 75, 23));
        pushButtonLeft = new QPushButton(centralWidget);
        pushButtonLeft->setObjectName(QStringLiteral("pushButtonLeft"));
        pushButtonLeft->setGeometry(QRect(90, 80, 75, 23));
        pushButtonRight = new QPushButton(centralWidget);
        pushButtonRight->setObjectName(QStringLiteral("pushButtonRight"));
        pushButtonRight->setGeometry(QRect(230, 80, 75, 23));
        pushButtonFollow = new QPushButton(centralWidget);
        pushButtonFollow->setObjectName(QStringLiteral("pushButtonFollow"));
        pushButtonFollow->setGeometry(QRect(80, 150, 75, 23));
        pushButtonFollow2 = new QPushButton(centralWidget);
        pushButtonFollow2->setObjectName(QStringLiteral("pushButtonFollow2"));
        pushButtonFollow2->setGeometry(QRect(160, 150, 75, 23));
        pushButtonObjectDetect = new QPushButton(centralWidget);
        pushButtonObjectDetect->setObjectName(QStringLiteral("pushButtonObjectDetect"));
        pushButtonObjectDetect->setGeometry(QRect(240, 150, 75, 23));
        pushButtonStop = new QPushButton(centralWidget);
        pushButtonStop->setObjectName(QStringLiteral("pushButtonStop"));
        pushButtonStop->setGeometry(QRect(160, 80, 75, 23));
        pushButtonErrorTest = new QPushButton(centralWidget);
        pushButtonErrorTest->setObjectName(QStringLiteral("pushButtonErrorTest"));
        pushButtonErrorTest->setGeometry(QRect(0, 10, 75, 23));
        pushButtonPersonDetect = new QPushButton(centralWidget);
        pushButtonPersonDetect->setObjectName(QStringLiteral("pushButtonPersonDetect"));
        pushButtonPersonDetect->setGeometry(QRect(160, 180, 75, 23));
        QR_Code = new QPushButton(centralWidget);
        QR_Code->setObjectName(QStringLiteral("QR_Code"));
        QR_Code->setGeometry(QRect(160, 210, 75, 23));
        searchQRcode = new QPushButton(centralWidget);
        searchQRcode->setObjectName(QStringLiteral("searchQRcode"));
        searchQRcode->setGeometry(QRect(80, 180, 75, 23));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButtonUp->setText(QApplication::translate("MainWindow", "Up", Q_NULLPTR));
        pushButtonDown->setText(QApplication::translate("MainWindow", "Down", Q_NULLPTR));
        pushButtonLeft->setText(QApplication::translate("MainWindow", "Left", Q_NULLPTR));
        pushButtonRight->setText(QApplication::translate("MainWindow", "Right", Q_NULLPTR));
        pushButtonFollow->setText(QApplication::translate("MainWindow", "Follow", Q_NULLPTR));
        pushButtonFollow2->setText(QApplication::translate("MainWindow", "Follow2", Q_NULLPTR));
        pushButtonObjectDetect->setText(QApplication::translate("MainWindow", "Obj Det", Q_NULLPTR));
        pushButtonStop->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        pushButtonErrorTest->setText(QApplication::translate("MainWindow", "Error Test", Q_NULLPTR));
        pushButtonPersonDetect->setText(QApplication::translate("MainWindow", "Person Detect", Q_NULLPTR));
        QR_Code->setText(QApplication::translate("MainWindow", "QR Code", Q_NULLPTR));
        searchQRcode->setText(QApplication::translate("MainWindow", "3d Map", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
