#include <Windows.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QKeyEvent>
#include <iostream>
#include <QProcess>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.h>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/features2d/features2d.hpp>
#include <math.h>
#include <time.h>
#include <vector>
#include <conio.h>
#include <zbar.h>
#include <OpenNI.h>
#include <PrimeSense.h>


using namespace zbar;
using namespace std;
using namespace cv;
using namespace openni;


static int flag = 0;

Moments mu;//used in follow and obj det
Moments moot;//used in follow2

Point p1,p2;//used in follow
bool exitLoop = true;


void MainWindow::on_searchQRcode_clicked()
{
    /*
   VideoCapture cap(CAP_OPENNI);

   Mat depthMap,bgrImage;

   namedWindow("depth",CV_WINDOW_AUTOSIZE);
   namedWindow("rgb",CV_WINDOW_AUTOSIZE);

   while(true)
   {
       cap.grab();
       cap.retrieve( depthMap, CAP_OPENNI_DEPTH_MAP );
       cap.retrieve( bgrImage, CAP_OPENNI_BGR_IMAGE );

       imshow("depth",depthMap);
       imshow("rgb",bgrImage);
       if( waitKey( 27 ) >= 0 )
            break;

   }
   */




}


void MainWindow::on_A_Star_clicked()
{
    

}

void MainWindow::on_QR_Code_clicked()
{


   VideoCapture cap(1); // open the video camera no. 0
   //cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
   //cap.set(CV_CAP_PROP_FRAME_HEIGHT,640);
   if (!cap.isOpened()) // if not success, exit program
   {
     cout << "Cannot open the video cam" << endl;
   }

   Mat frame,grey;
   ImageScanner scanner;
   scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
   cout << "Frame size : " << dWidth << " x " << dHeight << endl;
   namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"


   while (1)
   {

     bool bSuccess = cap.read(frame); // read a new frame from video
      if (!bSuccess) //if not success, break loop
     {
        cout << "Cannot read a frame from video stream" << endl;
        break;
     }

     cvtColor(frame,grey,CV_BGR2GRAY);
     int width = frame.cols;
     int height = frame.rows;
     uchar *raw = (uchar *)grey.data;
     // wrap image data
     Image image(width, height, "Y800", raw, width * height);
     // scan the image for barcodes
     int n = scanner.scan(image);
     // extract results
     for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
     {
        vector<Point> vp;
     // do something useful with results
        //cout << "decoded " << symbol->get_type_name() << " symbol " << symbol->get_data() << '"' << " "<< endl;
        cout << symbol->get_data() << endl;
        int n = symbol->get_location_size();
        for(int i=0;i<n;i++)
        {
         vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }

       RotatedRect r = minAreaRect(vp);
       Point2f pts[4];
       r.points(pts);
       for(int i=0;i<4;i++)
       {
         line(frame,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
       }
       //cout<<"Angle: "<<r.angle<<endl;
     }
     imshow("MyVideo", frame); //show the frame in "MyVideo" window
     if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
     {
       cout << "esc key is pressed by user" << endl;
       break;
     }
   }

  destroyAllWindows();


}


void MainWindow::on_pushButtonPersonDetect_clicked()
{
    VideoCapture cam(1);
    cam.set(CV_CAP_PROP_FRAME_WIDTH,320);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT,240);


    int width,height2Center;
    int centerOfRect;
    float offset,angle;
    bool exitLoop = true;
    bool personFound = false;
    Mat img;
    namedWindow("people detection",CV_WINDOW_AUTOSIZE);
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    while(exitLoop)
    {
          cam >> img;

          vector<Rect> found,found_filtered;

          hog.detectMultiScale(img,found,0,Size(16,16),Size(32,32),1.05,2);//first size = win_stride original (8,8)

          size_t i,j;

          for (i=0; i<found.size(); i++)
          {
              Rect r = found[i];
              for (j=0; j<found.size(); j++)
                  if (j!=i && (r & found[j]) == r)
                      break;
              if (j== found.size())
              {
                  personFound = true;
                  found_filtered.push_back(r);
              }
          }

          for (i=0; i<found_filtered.size(); i++)
          {
              Rect r = found_filtered[i];
              r.x += cvRound(r.width*0.1);
              r.width = cvRound(r.width*0.8);
              r.y += cvRound(r.height*0.07);
              r.height = cvRound(r.height*0.8);
              rectangle(img, r.tl(), r.br(), Scalar(0,255,0), 3);

              Point centerRect = (r.br() + r.tl())*0.5;
              circle(img,centerRect,3,Scalar(0,0,255));
              centerOfRect = r.x + r.width/2;
          }

          if(personFound == true)
          {
              width = img.size().width;
              height2Center = img.size().height;

              offset = (width/2)-centerOfRect;

              angle = tan(offset/float(height2Center));
              calcAngularVelocity(angle*(-1),50);//distance remains < 125 therefore furo will pivot

              cout << angle << endl;

              personFound = false;
          }


          imshow("people detection", img);
          if (waitKey(10) == 27)
              break;
    }

    destroyAllWindows();



}

/* This function detects a rectangular pink to red object.  I used a highlighter pink
 colored flash card for my object.  It also calc the distance to the obj in cm.  For accurate
measurements the program requires anm object about the size of two flashcards. (you may
need to experiment) The function also calls on other funtions in mainWindow to calc
angle and send the speed to the motors*/
void MainWindow::on_pushButtonObjectDetect_clicked()
{
    Mat img, img_gray, channel[3];
    VideoCapture cam(1); //cam 1
   // Point2f center;
    double distance = 0;
    int second = 1000;
    float angle;
    float offset;
    int width;
    int threshold_value = 0;
    bool good2Go = false;


    cam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    cam.set(CV_CAP_PROP_CONVERT_RGB, 1);
    namedWindow("Frame", CV_WINDOW_AUTOSIZE);
    namedWindow("gray",CV_WINDOW_AUTOSIZE);

    //createTrackbar("Determine Color","Frame",&threshold_value,255);

            while(exitLoop)
            {

               // Sleep(second/10); //update every tenth of second

                cam >> img;//get the next frame
                cvtColor(img, img_gray, CV_BGR2GRAY);//convert to gray
                split(img, channel);//split the image
                subtract(channel[2], img_gray, img_gray);


                //apply threshold
                //changing threshold can detect other colors
                threshold(img_gray, img_gray, /*threshold_value*/90, 255, THRESH_BINARY);


                //erode and dilate to remove unwanted objects from the frame
                erode(img_gray, img_gray, Mat(), Point(-1,-1), 4);
                dilate(img_gray, img_gray, Mat(), Point(-1,-1), 4);

                imshow("gray",img_gray); //show grayscale image




                        //these next four lines find the offset of the center of the object
                        //to the center of the screen
                        width = img.size().width;   //width of image
                        mu  = moments(img_gray,true); //create moment mu.  The variable declaration is global
                        Point2f center(mu.m10/mu.m00,mu.m01/mu.m00); //create center points for x y
                        offset = (width/2)-center.x;//offset is distance from center in pixels



                        //find contours
                        vector<vector<Size> > contors;
                        vector<Vec4i> heirarcy;
                        findContours(img_gray, contors, heirarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

                        vector<Rect> boundRect(contors.size());
                        vector<vector<Point> > contor_poly(contors.size());


                        //next identify the object and box it
                        for(int i =0; i < contors.size(); i++)
                        {
                            approxPolyDP(Mat(contors[i]), contor_poly[i], 3, true);
                            boundRect[i] = boundingRect(Mat(contor_poly[i]));
                        }

                        int max_index = 0,max_area = 0;

                        for(int i =0; i < boundRect.size(); i++)
                        {
                            int a = boundRect[i].area();
                            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(255,255,0), 2, 8, 0);
                            if( a > max_area)
                            {
                                max_area = a;
                                max_index = i;
                            }
                        }

                        int confidence = 0;

                        for(int i=0; i < boundRect.size(); i++)
                        {
                            if((boundRect[i].x < boundRect[max_index].x + boundRect[max_index].width && boundRect[i].x > boundRect[max_index].x - int(0.1*boundRect[max_index].width)) && (boundRect[i].y > boundRect[max_index].y) )
                                confidence += 45;

                        }

                        if(boundRect.size() > 0)
                        {
                            if(confidence > 99)
                                confidence = 0;



                            rectangle(img, boundRect[max_index].tl(), boundRect[max_index].br(), Scalar(0,255,0), 2, 8, 0);


                            distance = 6471*pow(boundRect[max_index].area(), -0.468); //original is 8414.7

                            if(isnan(offset))
                            {
                                //furo no move

                                FuroRobot::GetObj()->update(0.0,0.0);
                            }
                            else
                            {



                                    angle = objectDetectAngle(distance,offset);
                                    calcAngularVelocity(angle*(-1),distance);
                                    //angle = angle*180/CV_PI;    <-convert to degrees
                                    cout << distance << " cm." <<" angle: "<< angle <<endl;
                                    imshow("Frame", img);


                            }

                        }
                        else
                        {
                            imshow("Frame", img);

                        }

                        waitKey(25);

            }

            exitLoop = true;

            destroyAllWindows();

            cam.release();

}


//positive offset mean furo must turn left
//negative offset mean furo must turn right
float MainWindow::objectDetectAngle(float dist,float offset)
{
    float angle;

    //calc angle
    if(mu.m00 > 500)
    {
        angle = sin(offset/dist); //angle from center of screen to object


    }
    else
    {

        //furo doesnt move

    }



    return angle;

}



/* This is a trivial follow method.  It finds the center of the line on the ground and determines how far it deviates
from center as a percentage.  It updates the motors based on how far from center it is and which side the center is on(turn left or turn right)*/
void MainWindow::on_pushButtonFollow_clicked()
{

    Mat image;
    Mat imgROI;
    CvPoint bottom,top;
    float offset;
    int thresh = 80;
    int speed;
  //  int ROI_x,ROI_y,ROI_height;
    bool start_motion = false;


    namedWindow("follow",CV_WINDOW_AUTOSIZE);
    createTrackbar("threshold","follow",&thresh ,255);


    VideoCapture camera(1); //set up camera. May be different value than 0

    while(exitLoop)
    {
        camera >> image;
        if(camera.isOpened() == false) //check if cam is opened
            break;



        imgROI = image(Rect(0,190,image.size().width,100));//create region of interest (ROI)


        //convert the color
        cvtColor(imgROI,imgROI,CV_BGR2GRAY);
        GaussianBlur(imgROI,imgROI,Size(9,9),2,2);
        threshold(imgROI,imgROI,thresh,255,0);
        bitwise_not(imgROI,imgROI);
        erode(imgROI,imgROI,getStructuringElement(MORPH_ELLIPSE,Size(3,3)));
        dilate(imgROI,imgROI,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));

        //draw line in center of screen
        bottom.x = imgROI.size().width/2;
        bottom.y = 0;
        top.x = imgROI.size().width/2;
        top.y = imgROI.size().height;
        line(imgROI,bottom,top,Scalar(255,0,0),4);

        //calculate the offset from center of screen
        offset = x_offset(imgROI,imgROI.size().width);

        if(start_motion == true)
        {
            //move furo
            move(offset,speed);
        }

        imshow("follow",imgROI);
        if(waitKey(10) == 27)
        {
            //if "esc" stop furo and end follow
            FuroRobot::GetObj()->TranslateX(0.0,0.0);
            FuroRobot::GetObj()->RotateZ(0.0,0.0);
            break;
        }
        else if(waitKey(10) == 32) //else if space bar then allow furo to move
                start_motion = true;

    }

    exitLoop = true;
    destroyAllWindows();

}



/* Second method is more accurate as it finds the angle of teh line of the ground from the bottom of the frame
and then finds the velocity each wheel must turn at to turn furo and follow the line*/
void MainWindow::on_pushButtonFollow2_clicked()
{
    Mat image;
    Mat imgROI;
    float angle;

    namedWindow("follow2",CV_WINDOW_AUTOSIZE);

    VideoCapture camera(1);


    while(exitLoop)
    {
        if(camera.isOpened()==false)
            break;
        camera >> image;

       // Sleep(200);

        //convert color, blur image, use canny edge detection
        cvtColor(image,image,CV_BGR2GRAY);
        GaussianBlur(image,image,Size(3,3),2,2);
        Canny(image,image,50,150,3);
        //find the angle
        FindAngle(image,angle);

        //angle is returned in degrees.  +/- 90 depending on angle
        if(angle == 90)
            angle = 0;
        else if(angle >90)
            angle = angle -90;
        else if(angle <90)
            angle = angle +90;

        //cvt to radians
        angle = angle*CV_PI/180;
        cout << angle << endl;

        calcAngularVelocity(angle,0.0);//calc the velocity of each wheel


        imshow("follow2",image);
        if(waitKey(15)==27)
        {
            FuroRobot::GetObj()->update(0.0,0.0);
            break;
        }


    }

    exitLoop = true;
    destroyAllWindows();
    FuroRobot::GetObj()->update(0.0,0.0);


}



//calculates the angular velocity for each wheel and sends it to the motors
void MainWindow::calcAngularVelocity(float angle,float distance)
{
    float initial_velocity = 0.50f/(2.0*CV_PI*(MOBILE_WHEEL_DIAMETER/2.0)); // initial vel can be changed
    float vel_r = initial_velocity;
    float vel_l = initial_velocity;
    float s = (MOBILE_WHEEL_DIAMETER/2)*abs(angle); //find s (distance wheel must travel given the angle)
    float w = s/((MOBILE_WHEEL_DIAMETER/2));//find angular vel (w)

    w = w/2.0;//divide it by two.  One will be added to one wheel and the other subtracted from the other

    if(distance == 0.0)//other funtions such as follow and follow2 do not have distance funtions so they will
    {//send distance as 0.0. This if chunk is for those functions only
        if(angle >0)
        {

            vel_r = initial_velocity + w;
            vel_l = initial_velocity - w;

         }
        if(angle < 0)
        {

            vel_r = initial_velocity - w;
            vel_l = initial_velocity + w;
        }
        if(angle==0)
        {
            vel_r = initial_velocity;
            vel_l = initial_velocity;
        }
    }
    else //else the distance is given
    {
        if(distance < 125.0)
        {
            if(angle < 0)
            {
                vel_r = -w;
                vel_l = w;

            }
            if(angle > 0)
            {
                vel_r = w;
                vel_l = -w;
            }
        }
        else
        {
            if(angle >0)
            {

                vel_r = initial_velocity + w;
                vel_l = initial_velocity - w;

             }
            if(angle < 0)
            {

                vel_r = initial_velocity - w;
                vel_l = initial_velocity + w;
            }
            if(angle==0)
            {
                vel_r = initial_velocity;
                vel_l = initial_velocity;
            }

        }


    }


    FuroRobot::GetObj()->update(vel_r,vel_l); //update motors
}



//Specifically for the follow2 method. Finds line using houghline transform then finds angle in reference to bottom of frame
// angle is sent in degrees
void MainWindow::FindAngle(Mat& image,float& angle)
{

    vector<Vec4i> lines;
    HoughLinesP(image,lines,1,CV_PI/180,50,50,10);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        // draw the lines

        p1=Point(l[0], l[1]);
        p2=Point(l[2], l[3]);
        //calculate angle in radian,  if you need it in degrees just do angle * 180 / PI
        angle = atan2(p1.y - p2.y, p1.x - p2.x);
        angle = angle * 180/CV_PI;

    }

}

//find offset as a percentage from center for follow method
float MainWindow::x_offset(Mat img,int width)
{
    mu  = moments(img,true); //create moment mu.  The variable declaration is global

    Point2f center(mu.m10/mu.m00,mu.m01/mu.m00); //create center points for x y

    float offset = (1-2*center.x/width);
    if(isnan(offset))
        offset = -2.0;

    return offset;

}


//for follow method furo is moved based on the offset percentage
void MainWindow::move(float offset,int speed)
{
    if(offset == -2.0)
    {
        //no motion
       // FuroRobot::GetObj()->RotateZ(0.0,0.0);
       // FuroRobot::GetObj()->TranslateX(0.0,0.0);
        FuroRobot::GetObj()->MoveFuro(0.0,0.0,0.0,0.0);

    }
    else if(mu.m00 >2000)
    {
        //there is motion so apply checks to see if left,right,forward

        if(fabs(offset) >0.01 && fabs(offset)<0.4)
        {
            //move forward if line is within 1-50% of center screen
           // FuroRobot::GetObj()->RotateZ(0.0,0.0);
            //FuroRobot::GetObj()->TranslateX(0.75f/5.0,0.75f/5.0);
            FuroRobot::GetObj()->MoveFuro(0.5f/5.0,0.5f/5.0,0.5f/5.0,0.5f/5.0);

        }
        else if(fabs(offset)>0.4 && fabs(offset)<0.6 )
        {
           //else if the line moves out more than 50% check to see which side
            if(offset>0)
            {

                //if on the positive side then move left
               // FuroRobot::GetObj()->RotateZ(0.5f/5.0,0.5f/5.0);
                //FuroRobot::GetObj()->TranslateX(0.5f/5.0,0.5f/5.0);
                //FuroRobot::GetObj()->MoveFuro(0.5f/5.0,0.25f/5.0,0.5f/5.0,0.25f/5.0);
                FuroRobot::GetObj()->MoveFuro(0.25f/5.0,0.5f/5.0,0.25f/5.0,0.5f/5.0);

            }
            else
            {

               //if on the negative side then move right
                //FuroRobot::GetObj()->RotateZ(-0.5f/5.0,-0.5f/5.0);
               // FuroRobot::GetObj()->TranslateX(0.5f/5.0,0.5f/5.0);
               // FuroRobot::GetObj()->MoveFuro(0.25f/5.0,0.5f/5.0,0.25f/5.0,0.5f/5.0);
                 FuroRobot::GetObj()->MoveFuro(0.5f/5.0,0.25f/5.0,0.5f/5.0,0.25f/5.0);

            }
        }
        else if(fabs(offset)>0.6 && fabs(offset) < 0.8)
        {
            if(offset>0)
            {

                //if on the positive side then move left
               // FuroRobot::GetObj()->RotateZ(0.75f/5.0,0.75f/5.0);
               // FuroRobot::GetObj()->TranslateX(0.75f/5.0,0.75f/5.0);
              //  FuroRobot::GetObj()->MoveFuro(0.75f/5.0,0.15f/5.0,0.75f/5.0,0.15f/5.0);
                FuroRobot::GetObj()->MoveFuro(0.15f/5.0,0.75f/5.0,0.15f/5.0,0.75f/5.0);


            }
            else
            {

               //if on the negative side then move right
                //FuroRobot::GetObj()->RotateZ(-0.75f/5.0,-0.75f/5.0);
                //FuroRobot::GetObj()->TranslateX(0.75f/5.0,0.75f/5.0);
             //   FuroRobot::GetObj()->MoveFuro(0.15f/5.0,0.75f/5.0,0.15f/5.0,0.75f/5.0);
                FuroRobot::GetObj()->MoveFuro(0.75f/5.0,0.15f/5.0,0.75f/5.0,0.15f/5.0);

            }

        }
        else if(fabs(offset)>= 0.8)
        {
            if(offset>0)
            {
                FuroRobot::GetObj()->MoveFuro(0.5f/5.0,-0.25f/5.0,0.5f/5.0,0.25f/5.0);

            }
            else
            {
                FuroRobot::GetObj()->MoveFuro(-0.25f/5.0,0.5f/5.0,0.25f/5.0,0.5f/5.0);

            }

        }

    }
    else
    {
     //if offset isnan or mu00 < 3000 then stop
    // FuroRobot::GetObj()->RotateZ(0.0,0.0);
    // FuroRobot::GetObj()->TranslateX(0.00,0.00);
        FuroRobot::GetObj()->MoveFuro(0.0,0.0,0.0,0.0);
    }

}


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



}

MainWindow::~MainWindow()
{
    FuroRobot::GetObj()->update(0.0,0.0);
    //FuroRobot::GetObj()->TranslateX(0.0000,0.0000);

    delete ui;
}

void MainWindow::update(void)
{

}



//NOTE: the following functions need to be updated as the furo robot update function has been changed
void MainWindow::on_pushButtonUp_clicked()
{
    FuroRobot::GetObj()->update(5.0f/5.0,5.0f/5.0);
}

void MainWindow::on_pushButtonDown_clicked()
{
    FuroRobot::GetObj()->update(-5.0f/5.0,-5.0f/5.0);

}

void MainWindow::on_pushButtonStop_clicked()
{
    FuroRobot::GetObj()->update(0.0,0.0);

}

void MainWindow::on_pushButtonLeft_clicked()
{
    FuroRobot::GetObj()->update(0.5f,-0.5f);

}

void MainWindow::on_pushButtonRight_clicked()
{
    FuroRobot::GetObj()->update(-0.5f,0.5f);

}


void MainWindow::on_pushButtonErrorTest_clicked()
{
    //test to move furo one meter and determine percent error in accuracy
    time_t end,start;
    double elapsed;


    start = time(NULL);

    while(true)
    {
        end = time(NULL);
        elapsed = difftime(end, start);

        if(elapsed <= 1)
            FuroRobot::GetObj()->update(1.33f,1.33f);
        else
            break;
    }
    FuroRobot::GetObj()->update(0.0,0.0);

}




bool MainWindow::eventFilter(QObject *object, QEvent *e)
{

    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if(keyEvent->key()==Qt::Key_W)
            FuroRobot::GetObj()->update(0.5f,0.5f);
        else if(keyEvent->key()==Qt::Key_S)
            FuroRobot::GetObj()->update(-0.5f,-0.5f);
        else if(keyEvent->key() == Qt::Key_A)
            FuroRobot::GetObj()->update(0.5f,-0.5f);
        else if(keyEvent->key() == Qt::Key_D)
            FuroRobot::GetObj()->update(-0.5f,0.5f);
        else if(keyEvent->key() == Qt::Key_Q)
            FuroRobot::GetObj()->update(0.0f,0.0f);
        else if(keyEvent->key() == Qt::Key_1)
            MainWindow::on_pushButtonFollow_clicked();
        else if(keyEvent->key() == Qt::Key_2)
            MainWindow::on_pushButtonFollow2_clicked();
        else if(keyEvent->key() == Qt::Key_3)
            MainWindow::on_pushButtonObjectDetect_clicked();
        else if(keyEvent->key()==Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
        {
            exitLoop = false;
            destroyAllWindows();
        }
        else if(keyEvent->key()== Qt::Key_Escape)
        {

            exitLoop = false;
            destroyAllWindows();
            close();


        }
        else
            QObject::eventFilter(object,e);

        return true;


        /*
        switch(keyEvent->key())
        {
        case Qt::Key_Left:
            FuroRobot::GetObj()->update(0.5f,-0.5f);
            break;
        case Qt::Key_Right:
            FuroRobot::GetObj()->update(-0.5f,0.5f);
            break;
        case Qt::Key_Up:
            FuroRobot::GetObj()->update(1.0f,1.0f);
            break;
        case Qt::Key_Down:
            FuroRobot::GetObj()->update(-1.0f,-1.0f);
            break;
        case Qt::Key_Space:
            FuroRobot::GetObj()->update(0.0,0.0);
            break;
        case Qt::Key_1:

              //QProcess::execute ("SAPITest.exe Hello I am Rich the Robot I represent offercraft and rich robotics");
              //QProcess::execute ("SAPITest.exe We have a strategic partnership with Future Robot");

            break;
        case Qt::Key_2:

              //QProcess::execute ("SAPITest.exe Today You can win a prize");
              //QProcess::execute ("SAPITest.exe please Give us your business card.\n enter your email address \n pick a brief case to win your prize");


            break;

        }
        */

    }
    else
        return QObject::eventFilter(object,e);

    return false;
}



