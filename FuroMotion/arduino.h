#ifndef ARDUINO_H
#define ARDUINO_H
#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QObject>
#include <QThread>

class arduino:public QObject
{

Q_OBJECT

public:
    QSerialPort port;
    explicit arduino(QObject *parent = 0);
    ~arduino();
    bool initialize();
    bool release();
//    void recieveDone(const int size);



public slots:
    void writeDir(char dir);
    void writeDir(const char dir[50], int size);
    void getValues();
    void writeEnd();

private:

};

#endif // ARDUINO_H

