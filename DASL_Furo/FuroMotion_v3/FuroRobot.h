
#ifndef FURO_H
#define FURO_H

#include "FuroMoterController.h"
#include <qthread.h>
#define   MOBILE_DIRECTION_RIGHT            1
#define   MOBILE_DIRECTION_LEFT            -1
#define   MOBILE_WHEEL_DIAMETER          0.15  //in meters
#define   MOBILE_WHEEL_DISTANCE         0.333
#define   MOBILE_SLOPE_COEFFICIENT        1.5
#define   MOBILE_GEAR_RATIO                64
#define   INITIAL_VELOCITY                0.1


class FuroRobot : public QObject
{
    Q_OBJECT

    FuroRobot();
    float mobile_linVel_L,mobile_linVel_R;
    float mobile_rotVel_L,mobile_rotVel_R;
public:
    FuroMoterController devLower;
    static FuroRobot * GetObj();
    void Initialize();
    ~FuroRobot();

    // QThread interface
    void TranslateX(float val_L,float val_R);
    void RotateZ(float val_L,float val_R);

    //opencv interactions
    void MoveFuro(float lin_r,float lin_l,float rot_r,float rot_l);
    void update(float lin_r,float lin_l);

protected:


private slots:

    //void update();


};

#endif // FURO_H
