#ifndef FUROMOTERCONTROLLER_H
#define FUROMOTERCONTROLLER_H

#include "Device.h"
#include <QtSerialPort/QtSerialPort>
#include <QThread>



#define FMC_AXISID_HEAD_YAW      0x27
#define FMC_AXISID_HEAD_PITCH    0x28
#define FMC_AXISID_HEAD_ROLL     0x29
#define FMC_AXISID_WAIST_PITCH   0x2B
#define FMC_AXISID_WRIST_PITCH   0x2D
#define FMC_AXISID_MOBILE_LEFT   0x2E
#define FMC_AXISID_MOBILE_RIGHT  0x2F


#define FMC_BOARDID_MOBILEBOARD 0x2E
#define FMC_BOARDID_BODYBOARD   0x2F
#define FMC_BOARDID_BROADCAST   0xFE



#define FMC_COMMAND_PING       0x01
#define FMC_COMMAND_GET        0x02
#define FMC_COMMAND_SET        0x03
#define FMC_COMMAND_READY      0x04
#define FMC_COMMAND_ACT        0x05
#define FMC_COMMAND_SYNC       0x06
#define FMC_COMMAND_EVENT      0x07


#define FMC_FUNCTION_DEV_INFO         0x00
#define FMC_FUNCTION_MOBILE_ODOMETRY  0xA0
#define FMC_FUNCTION_OPERATION_STATE  0xA4
#define FMC_FUNCTION_ZOBE_BOARD_MODE  0xAA
#define FMC_FUNCTION_BODY_POSITON     0xB0
#define FMC_FUNCTION_BODY_LIMIT       0xB1
#define FMC_FUNCTION_BODY_GAIN        0xB2
#define FMC_FUNCTION_BODY_AUTO_HOME   0xB3
#define FMC_FUNCTION_HOME_BUTTON      0xA5
#define FMC_FUNCTION_ACCELATION_SET   0xAB



#define FMC_DXL_PREAMBLE                         0xFF
#define FMC_DXL_DXC_SYNC_WRITE                   0x83
#define FMC_DXL_DXI_GOAL_POSITION_L              0x1E
#define FMC_DXL_DXI_MOVING_SPEED_L               0x20
#define FMC_DXL_DXI_MOVING_SPEED_ACCELERATION    0x21


#define FMC_LENGTH_MOBILE_ENCODER_MODE         0x08
#define FMC_LENGTH_MOBILE_POSITION_MODE        0x08
#define FMC_LENGTH_BODY_POSITION               0x0F
#define FMC_LENGTH_BODY_GAIN                   0x14
#define FMC_LENGTH_BODY_LIMIT                  0x0E
#define FMC_LENGTH_BODY_GET_GAIN               0x14
#define FMC_LENGTH_BODY_POSITION_MODE          0x08
#define FMC_LENGTH_BODY_AXIS1                  0x0D
#define FMC_LENGTH_BODY_AXIS2                  0x12
#define FMC_LENGTH_BODY_AXIS3                  0x17
#define FMC_LENGTH_BODY_AXIS5                  0x21
#define FMC_LENGTH_BODY_AXIS_DATA              0x04
#define FMC_LENGTH_MOBILE_PACKET               0x12
#define FMC_LENGTH_BOARD_PACKET                0x0B
#define FMC_LENGTH_ODOMETRY_PACKET             0x13
#define FMC_LENGTH_MOBILE_ENCODER_PACKET       0x0F
#define FMC_LENGTH_QUERY                       0x07
#define FMC_LENGTH_REQUEST_LIMIT               0x08
#define FMC_LENGTH_VERSION                     0x0A
#define FMC_LENGTH_OPERATING_STATE             0x0B
#define FMC_LENGTH_HOME_BUTTON                 0x08
#define FMC_LENGTH_ZOBE_BOARD_MODE             0x08
#define FMC_LENGTH_ACCELATION_SET              0x0F

//#define FMC_ENABLE_ENCODER_READ                0x05
#define FMC_ENABLE_ENCODER_READ                0x04
#define FMC_DISABLE_ENCODER_READ               0x00


#define FMC_MAX_ENCODER                        0x03
#define FMC_ENCODER_CHANNEL1                   0x00
#define FMC_ENCODER_CHANNEL2                   0x01
#define FMC_ENCODER_CHANNEL3                   0x02


class FuroMoterController : public QObject
{
    Q_OBJECT

     int         encoder[3];
public:
     QSerialPort devSerial;

    explicit FuroMoterController(QObject *parent = 0);

    // Device interface
    char CalcCheckSum(char *msg, int len);


public:
    bool Init(const char *devName);
    bool Release();

    void SetWheelEncoderRead(unsigned char mode);
    int GetEncoder(int index);


    void DriveWheel(short leftVelocity, int leftTime, short rightVelocity, int rightTime);
private slots:
    void readData(void);
    void writeData(qint64 bytes);




};

#endif // FUROMOTERCONTROLLER_H
