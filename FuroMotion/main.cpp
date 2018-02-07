#include "mainwindow.h"
#include "calculate.h"

#include <iostream>

#include <QDesktopWidget>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QRect>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f(":qdarkstyle/style.qss");

    if(!f.exists()){
        std::cout << "Unable to find stylesheet" << std::endl;
    }
    else{
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

    QDesktopWidget dw;
    MainWindow w;

    int width = dw.width()*0.8;
    int height = dw.height()*0.8;
//    w.setFixedSize(width, height);
    w.setWindowState(w.windowState() ^ Qt::WindowFullScreen);

    w.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            w.size(),
            a.desktop()->availableGeometry()
        )
    );
    w.show();
    return a.exec();
}

