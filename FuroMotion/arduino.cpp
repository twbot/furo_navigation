#include <QSerialPort>
#include <QtSerialPort>
#include <iostream>
#include "arduino.h"
#include "mainwindow.h"
#include <string>

using namespace std;


arduino::~arduino()
{


}


arduino::arduino(QObject *parent)
{


}

bool arduino::initialize()
{
    port.setPortName("COM12");
    port.setBaudRate(QSerialPort::Baud115200);
    port.setDataBits(QSerialPort::Data8);
    port.setParity(QSerialPort::NoParity);
    port.setStopBits(QSerialPort::OneStop);
    port.setFlowControl(QSerialPort::NoFlowControl);

    if(port.open(QIODevice::ReadWrite))
    {


        //connect(&port,SIGNAL(readyRead()),this,SLOT(getValues()));
       // connect(&port, SIGNAL(bytesWritten(qint64)), this, SLOT(setZero()));


        return true;
    }
    else return false;
}

void arduino::writeDir(const char dir){


    QByteArray data;
    data.append(dir);
    data.append('*');

    if(port.isWritable()){
        port.write(data);
    }
    else{
        qDebug() << "Cannot write to arduino" << endl;
    }

}

void arduino::writeDir(const char dir[50], int size){


    QByteArray data;

    for(int i = 0; i < size; i++ ){
        data.append(dir[i]);
    }

    if(port.isWritable()){
        port.write(data);
    }
    else{
        qDebug() << "Cannot write to arduino" << endl;
    }

}

void arduino::writeEnd(){
    QByteArray end = "e";
    if(port.isOpen()){
        if(port.isWritable()){
            port.write(end);
        }
        else{
            qDebug() << "Cannot write to arduino" << endl;
        }
    }
    else
        qDebug() << "Port not open" << endl;
}

void arduino::getValues()
{
    const qint64 maxLen = 256;
    bool check = false;

    if(port.waitForReadyRead(1000))
    {
        if(port.canReadLine())
        {
            while(!check)
            {
                //qDebug() << "made it here" << endl;
                QByteArray buffer = port.readLine(maxLen);
                QString qstring = QString::fromStdString(buffer.toStdString());
                QStringList bufferString = qstring.split(' ');
                if(bufferString.length() >= 3){

                    angle = bufferString[0].toInt();
                    leftWheel = bufferString[1].toInt();
                    rightWheel = bufferString[2].toInt();


                   // qDebug () << angle << " " << leftWheel << " " << rightWheel << endl;

                    check = true;
                }
            }
        }
    }

}


bool arduino::release()
{
    port.close();
    return true;
}
