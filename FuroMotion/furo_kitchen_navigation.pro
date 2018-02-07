QT       += core
QT       += gui
QT       += serialport
QT       += printsupport
CONFIG   += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Furo_Kitchen_Navigation
TEMPLATE = app



DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += ./qdarkstyle/style.qrc

INCLUDEPATH += "D:\OpenCV\opencv-3.2.0\build\install\include"
INCLUDEPATH += "D:\OpenCV\opencv-3.2.0\build\install\x86\vc12\bin"

INCLUDEPATH += "C:\PROGRA~1\OpenNI2\Include"

INCLUDEPATH += "C:\PROGRA~1\PrimeSense\NITE\Include"


DEPENDPATH += "D:\OpenCV\opencv-3.2.0\build\install\include"
DEPENDPATH += "C:\PROGRA~1\PrimeSense\NITE\Bin"
DEPENDPATH += "C:\PROGRA~1\OpenNI2\samples\bin"


LIBS += -LC:\PROGRA~1\OpenNI2\Lib \
    -lOpenNI2


LIBS += -LC:\PROGRA~1\PrimeSense\NITE\Lib \
    -lXnVCNITE_1_5_2 \
    -lXnVFeatures_1_5_2 \
    -lXnVHandGenerator_1_5_2 \
    -lXnVNITE.jni \
    -lXnVNITE_1_5_2


LIBS += -LD:\OpenCV\opencv-3.2.0\build\install\x86\vc12\lib \
    -lopencv_core320d \
    -lopencv_highgui320d \
    -lopencv_imgproc320d \
    -lopencv_features2d320d \
    -lopencv_calib3d320d \
    -lopencv_imgcodecs320d \
    -lopencv_imgcodecs320d \
    -lopencv_objdetect320d \
    -lopencv_video320d \
    -lopencv_videoio320d \
    -lopencv_shape320d \
    -lopencv_aruco320d


SOURCES += main.cpp\
    mainwindow.cpp \
    map.cpp \
    calculate.cpp \
    FuroRobot.cpp \
    FuroMoterController.cpp \
    node.cpp \
    device.cpp \
    arduino.cpp



HEADERS  += mainwindow.h \
    FuroRobot.h \
    map.h \
    FuroMoterController.h \
    calculate.h \
    device.h \
    node.h \
    arduino.h


FORMS    += mainwindow.ui \
    map.ui
