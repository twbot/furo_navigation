#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include<array>

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/tracking.hpp>
#include <opencv/cv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/ccalib.hpp>

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QMouseEvent>

#include "map.h"
#include "arduino.h"
#include "mainwindow.h"

extern int angle;
extern int rightWheel;
extern int leftWheel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    int angle, leftWheel,rightWheel;

    bool eventFilter(QObject *object, QEvent *e);

    char movements[100];
    char path[100];


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:

    struct selection{
        std::vector<int> location;
        std::string name;
    };

    QLabel *createIcon(int i);
    QLabel *imageLabels[2][3];

    Ui::MainWindow *ui;
    Map *mapWidget;

    std::string base_dir_linux = "/home/twbot/Desktop/furo_images";
    std::string base_dir_windows = "C:\\Users\\FRS06-00002\\Desktop\\furo_images";
    selection locations[6];

    void startMappingActivity(int choice);
    void setGoalLocations();
    void loop(std::vector<int> go);
    void augmentPath(unsigned int size);

    void breakTime(int seconds);

    void testForward();

    int startX = 1;
    int startY = 1;
    int endX = 0;
    int endY = 0;

    int forward = 0;
    int right = 0;
    int left = 0;


    arduino serial;
    bool check = false;


};

#endif // MAINWINDOW_H
