#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calculate.h"
#include "arduino.h"

#include <iostream>
#include <string>
#include <QMessageBox>
#include <QtCore>

#include <thread>
#include <chrono>

#define METER_ENCODER_VALUE 65444
#define FOOT_ENCODER_VALUE 17222

using namespace std;
using namespace this_thread;
using namespace chrono;

int angle,leftWheel,rightWheel;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FuroRobot::GetObj()->Initialize();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);
    installEventFilter(this);
    setFocusPolicy(Qt::ClickFocus);


    ui->centralwidget->setWindowTitle("Choose object");
    check = serial.initialize();
    serial.getValues();
    setGoalLocations();

    int index = 0;
    for(int row = 0; row < 2; row++){
        for(int col = 0; col < 3; col++){
            imageLabels[row][col] = createIcon(index);
            ui->gridLayout->addWidget(imageLabels[row][col], row, col);
            index++;
        }
    }
}


MainWindow::~MainWindow()
{
    FuroRobot::GetObj()->MoveFuro(0.0,0.0,0.0,0.0);
    delete ui;
    serial.release();
}


QLabel *MainWindow::createIcon(int i)
{
    QLabel *label = new QLabel;
    const std::string file = base_dir_windows + "\\" + locations[i].name + ".png";
    QString image = QString::fromStdString(file);
    QPixmap img(image);
    if(img.isNull()){
        QMessageBox::critical(this, "", "No image could be found!");
    }
    label->setPixmap(img);\
    label->setAlignment(Qt::AlignCenter);
    label->setFrameShape(QFrame::StyledPanel);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setMinimumSize(100, 100);
    return label;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton ){
       int x = event->pos().x();
       int y = event->pos().y();

       int choice = 10;


       if( y > 170 && y < 350){
           if(x > 150 && x < 400){
               choice = 1;
           }
           if(x > 700 && x < 960){
               choice = 2;
           }
           if(x > 1250 && x < 1500){
               choice = 3;
           }
        }
        if(y > 675 && y < 900){
            if(x > 150 && x < 400){
                choice = 4;
            }
            if(x > 700 && x < 960){
                choice = 5;
            }
            if(x > 1250 && x < 1500){
                choice = 6;
            }
        }

        if(!(choice <= 6)){
            QMessageBox::information(this, "","Please select an area within the black boundaries.");
        }
        else{
            startMappingActivity(choice);
        }
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        qApp->quit();
    }
}


void MainWindow::setGoalLocations()
{
    std::string goals[6] = {"Bed", "Coffee Pot", "Couch", "Desk", "Fridge", "TV"};
    std::vector<std::vector <int>> locs = {{11,16},{8,0},{4,12},{11,20},{4,0},{13,11}};
    int numObstacles = sizeof(goals)/sizeof(goals[0]);

    for(int i = 0; i < numObstacles; i++){

        locations[i].name = goals[i];
        locations[i].location.push_back(locs[i][0]);
        locations[i].location.push_back(locs[i][1]);
    }
}

void MainWindow::loop(std::vector<int> go){

//    movements[0] = 0x00;

    for(unsigned int i =0; i < go.size(); i++){
        int where = go[i];

        switch(where){
            case 0:
                movements[i] = 'f';
                //moveForward();
                break;
            case 1:
                movements[i] = 'r';
//                moveRight();
                break;
            case 2:
                movements[i] = 'b';
                //moveBackward();
                break;
            case 3:
                movements[i] = 'l';
//                moveLeft();
                break;
            case 4:
                movements[i] = 's';
//                stay();
                break;
           default:
                movements[i] = 's';
//                stay();
        }
    }
    augmentPath(go.size());
//    serial.writeDir(movements, go.size());
}

void MainWindow::augmentPath(unsigned int size)
{
    int j = 0;
    for(unsigned int i =0; i < size-1 ; i++){
       if(movements[i] == 'r'){
            path[j] = 'r';
            path[j+1] = 'f';
            j += 2;
       }
       else if(movements[i] == 'l'){
            path[j] = 'l';
            path[j+1] = 'f';
            j += 2;
       }
       else {
           path[j] = movements[i];
           j++;
       }
    }
    serial.writeDir(path, j);

}

void MainWindow::startMappingActivity(int choice)
{
    QString destination = QString::fromStdString(locations[choice-1].name);
    QMessageBox::information(this, "","Destination: " + destination);

    std::string file = "C:\\Users\\FRS06-00002\\Desktop\\Area.JPG";

    endX = locations[choice-1].location[0];
    endY = locations[choice-1].location[1];
    std::vector<std::vector<int>> path = Calculate::run(file, 24, 17, startX, startY,endX, endY);
    startX = endX;
    startY = endY;
    std::vector<int> go = Calculate::followPath(1, path);
    go.push_back(4);
    loop(go);
}


bool MainWindow::eventFilter(QObject *object, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if(keyEvent->key()==Qt::Key_W)
            FuroRobot::GetObj()->MoveFuro(1.0,1.0,1.0,1.0);
        else if(keyEvent->key()==Qt::Key_S)
            FuroRobot::GetObj()->MoveFuro(-1.0,-1.0,-1.0,-1.0);
        else if(keyEvent->key() == Qt::Key_A)
            FuroRobot::GetObj()->MoveFuro(1.0,1.0,-1.0,-1.0);
        else if(keyEvent->key() == Qt::Key_D)
            FuroRobot::GetObj()->MoveFuro(-1.0,-1.0,1.0,1.0);
        else if(keyEvent->key() == Qt::Key_Q)
            FuroRobot::GetObj()->MoveFuro(0.0,0.0,0.0,0.0);
        else if(keyEvent->key()==Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
        {

            FuroRobot::GetObj()->MoveFuro(0.0,0.0,0.0,0.0);

        }
        else if(keyEvent->key()== Qt::Key_Escape)
        {
            FuroRobot::GetObj()->MoveFuro(0.0,0.0,0.0,0.0);
            close();


        }
        else if(keyEvent->key() == Qt::Key_Space)
        {
            serial.writeEnd();
        }
        else
            QObject::eventFilter(object,e);

        return true;

    }
    else
        return QObject::eventFilter(object,e);

    return false;
}
