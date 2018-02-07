#include "map.h"
#include "ui_map.h"
#include "calculate.h"
#include "mainwindow.h"

#include <QImage>
#include <QTimer>
#include <QKeyEvent>
#include <iostream>
#include <QString>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/calib3d.hpp>


Map::Map(QWidget *parent, int choice) :
    QDialog(parent),
    ui(new Ui::Map)

{
    ui->setupUi(this);
    QMessageBox::information(this, "",QString::number(choice));
}

Map::~Map()
{
    delete ui;
}


