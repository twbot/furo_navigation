#include "FuroRobot.h"


FuroRobot::FuroRobot()
{

}

FuroRobot *FuroRobot::GetObj()
{
    static FuroRobot obj;
    return &obj;
}
void FuroRobot::Initialize()
{

   devLower.Init("COM3");
   QTimer *timer = new QTimer(this);
   connect(timer, SIGNAL(timeout()), this, SLOT(update()));
   timer->start(100);
}


void FuroRobot::TranslateX(float val_L,float val_R)
{
    mobile_linVel_R = val_R;
    mobile_linVel_L = val_L;

}

void FuroRobot::RotateZ(float val_L,float val_R)
{
    mobile_rotVel_R = val_R;
    mobile_rotVel_L = val_L;
}




FuroRobot::~FuroRobot()
{

}


//updates the wheels velocity
void FuroRobot::update(float lin_r,float lin_l)
{
    //first was negative
   //float desired_wheel_vel_r = (1.0f/MOBILE_WHEEL_DIAMETER)*mobile_linVel_R + (MOBILE_WHEEL_DISTANCE/MOBILE_WHEEL_DIAMETER)*mobile_rotVel_R;
   //float desired_wheel_vel_l = (1.0f/MOBILE_WHEEL_DIAMETER)*mobile_linVel_L + (MOBILE_WHEEL_DISTANCE/MOBILE_WHEEL_DIAMETER)*mobile_rotVel_L;

    float desired_wheel_vel_r = lin_r;
    float desired_wheel_vel_l = lin_l;


    float desired_rpm_r =  60 * MOBILE_GEAR_RATIO  * desired_wheel_vel_r;
    float desired_rpm_l =  60 * MOBILE_GEAR_RATIO  * desired_wheel_vel_l;


    if(desired_rpm_r >= 30000)
    {
        desired_rpm_r = 30000;
    }
    if(desired_rpm_r <= -30000)
    {
        desired_rpm_r = -30000;
    }


    if(desired_rpm_l >= 30000)
    {
        desired_rpm_l = 30000;
    }
    if(desired_rpm_l <= -30000)
    {
        desired_rpm_l = -30000;
    }

    devLower.DriveWheel(MOBILE_DIRECTION_LEFT*desired_rpm_l,1000, MOBILE_DIRECTION_RIGHT*desired_rpm_r,1000);

}




void FuroRobot::MoveFuro(float lin_r,float lin_l,float rot_r,float rot_l)
{
    mobile_linVel_L = lin_l;
    mobile_linVel_R = lin_r;
    mobile_rotVel_L = rot_l;
    mobile_rotVel_R = rot_r;


}
