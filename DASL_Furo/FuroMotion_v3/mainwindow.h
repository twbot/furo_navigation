#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FuroRobot.h"
#include <opencv2/core/core.hpp>
using namespace cv;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool eventFilter(QObject *object, QEvent *e);
private:
    Ui::MainWindow *ui;

private slots:
    void update();

    void on_pushButtonUp_clicked();
    void on_pushButtonDown_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonLeft_clicked();
    void on_pushButtonRight_clicked();
    void on_pushButtonFollow_clicked();
    void on_pushButtonFollow2_clicked();
    void on_pushButtonObjectDetect_clicked();
    void on_pushButtonPersonDetect_clicked();
    void on_searchQRcode_clicked();
    void on_QR_Code_clicked();
    void FindAngle(Mat&,float&);
    void on_pushButtonErrorTest_clicked();
    void calcAngularVelocity(float,float);
    float objectDetectAngle(float,float);

    float x_offset(Mat,int);
    void move(float,int);

    // QWidget interface
};

#endif // MAINWINDOW_H
