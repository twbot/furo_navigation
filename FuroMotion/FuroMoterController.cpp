#include "FuroMoterController.h"
#include <math.h>
#include <vector>



FuroMoterController::FuroMoterController(QObject *parent)
{

}

bool FuroMoterController::Init(const char *devName)
{

    devSerial.setPortName(devName);
    devSerial.setBaudRate(QSerialPort::Baud115200);
    devSerial.setDataBits(QSerialPort::Data8);
    devSerial.setParity(QSerialPort::NoParity);
    devSerial.setStopBits(QSerialPort::OneStop);
    devSerial.setFlowControl(QSerialPort::NoFlowControl);


    if(devSerial.open(QIODevice::ReadWrite))
    {
        connect(&devSerial, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(&devSerial, SIGNAL(bytesWritten(qint64)), this, SLOT(writeData(qint64)));


        SetWheelEncoderRead(FMC_ENABLE_ENCODER_READ);
        return true;
    }

    return false;
}

bool FuroMoterController::Release()
{
    devSerial.close();

    return true;
}
void FuroMoterController::SetWheelEncoderRead( unsigned char mode)
{


    std::vector<char> vecData;

    vecData.push_back(FMC_DXL_PREAMBLE);
    vecData.push_back(FMC_DXL_PREAMBLE);
    vecData.push_back(FMC_BOARDID_MOBILEBOARD);
    vecData.push_back(FMC_LENGTH_MOBILE_ENCODER_MODE - 4);
    vecData.push_back(FMC_COMMAND_SET);
    vecData.push_back(FMC_FUNCTION_MOBILE_ODOMETRY);
    vecData.push_back(mode);
    vecData.push_back(CalcCheckSum(&vecData[0], FMC_LENGTH_MOBILE_ENCODER_MODE));

    devSerial.write(&vecData[0],vecData.size());

}




int FuroMoterController::GetEncoder(int index)
{
    Q_ASSERT(index <FMC_MAX_ENCODER);
    return encoder[index];
}


char FuroMoterController::CalcCheckSum(char *msg, int len)
{
    char checkSum = (char)0;

    for (int i = 2; i < len - 1; i++)
    {
        checkSum += msg[i];
    }
    return (char)~checkSum;
}


void FuroMoterController::DriveWheel( short leftVelocity, int leftTime, short rightVelocity, int rightTime)
{
    std::vector<char> vecData;

    vecData.push_back(FMC_DXL_PREAMBLE);
    vecData.push_back(FMC_DXL_PREAMBLE);
    vecData.push_back(FMC_BOARDID_BROADCAST);
    vecData.push_back(FMC_LENGTH_MOBILE_PACKET - 4);
    vecData.push_back(FMC_COMMAND_SYNC);
    vecData.push_back(FMC_DXL_DXI_MOVING_SPEED_L);
    vecData.push_back(FMC_LENGTH_BODY_AXIS_DATA);
    vecData.push_back(FMC_AXISID_MOBILE_LEFT);
    vecData.push_back((0xFF & leftVelocity));
    vecData.push_back((0xFF & (leftVelocity >> 8)));
    vecData.push_back((0xFF & leftTime));
    vecData.push_back((0xFF & (leftTime >> 8)));
    vecData.push_back(FMC_AXISID_MOBILE_RIGHT);
    vecData.push_back((0xFF & rightVelocity));
    vecData.push_back((0xFF & (rightVelocity >> 8)));
    vecData.push_back((0xFF & rightTime));
    vecData.push_back((0xFF & (rightTime >> 8)));
    vecData.push_back(CalcCheckSum(&vecData[0], FMC_LENGTH_MOBILE_PACKET));


    devSerial.write(&vecData[0],vecData.size());
}
void FuroMoterController::readData(void)
{
    static std::vector<char> buff;

    QByteArray  arr=  devSerial.readAll();
    if(!arr.isEmpty())
    {

        for(int i = 0; i < arr.length() ; i++)
        {
            buff.push_back(arr.at(i));
        }
    }

    while(buff.size() >= FMC_LENGTH_ODOMETRY_PACKET)
    {
        if(buff[0] == (char)FMC_DXL_PREAMBLE && buff[1] == (char)FMC_DXL_PREAMBLE)
        {
            char check_sum = CalcCheckSum(&buff[0], FMC_LENGTH_ODOMETRY_PACKET);
            if(buff[FMC_LENGTH_ODOMETRY_PACKET-1] == check_sum)
            {
                memcpy(&encoder[FMC_ENCODER_CHANNEL1],&buff[6],sizeof(int));
                memcpy(&encoder[FMC_ENCODER_CHANNEL2],&buff[10],sizeof(int));
                memcpy(&encoder[FMC_ENCODER_CHANNEL3],&buff[14],sizeof(int));

                buff.erase(buff.begin(),buff.begin()+FMC_LENGTH_ODOMETRY_PACKET);
            }
            else
            {
                buff.erase(buff.begin());
            }

        }
        else
        {
            buff.erase(buff.begin());
        }
    }

}

void FuroMoterController::writeData(qint64 bytes)
{
}
