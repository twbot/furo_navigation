#-------------------------------------------------
#
# Project created by QtCreator 2017-06-09T18:01:25
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FuroMotion_v3
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


#INCLUDE PATHS###########
INCLUDEPATH += "C:\\opencv3.2\\install\\include"
INCLUDEPATH += "C:\\opencv3.2\\install\\x86\vc12\bin"

INCLUDEPATH += "C:\\PROGRA~1\\ZBar\\bin"
INCLUDEPATH += "C:\PROGRA~1\ZBar\include"

INCLUDEPATH += "C:\PROGRA~1\OpenNI2\Include"

INCLUDEPATH += "C:\PROGRA~1\PrimeSense\NITE\Include"

#########################


###Depend Path####
DEPENDPATH += "C:\\PROGRA~1\\ZBar\\bin"

DEPENDPATH += "C:\\opencv3.2\\install\\include"

DEPENDPATH += "C:\PROGRA~1\PrimeSense\NITE\Bin"

#DEPENDPATH += "C:\PROGRA~1\OpenNI2\Bin"

#########################
LIBS += -LC:\PROGRA~1\OpenNI2\Lib \
    -lOpenNI2


LIBS += -LC:\PROGRA~1\PrimeSense\NITE\Lib \
    -lXnVCNITE_1_5_2 \
    -lXnVFeatures_1_5_2 \
    -lXnVHandGenerator_1_5_2 \
    -lXnVNITE.jni \
    -lXnVNITE_1_5_2

LIBS += -LC:\PROGRA~1\Zbar\lib \
    -llibzbar-0

LIBS += -LC:\\opencv3.2\\install\\x86\\vc12\\bin \
    -lopencv_core320d \
    -lopencv_highgui320d \
    -lopencv_imgproc320d \
    -lopencv_features2d320d \
    -lopencv_calib3d320d \
    -lopencv_imgcodecs320d \
    -lopencv_objdetect320d \
    -lopencv_video320d \
    -lopencv_videoio320d \
    -lopencv_shape320d

LIBS += -LC:\opencv3.2\install\x86\vc12\lib \
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
    -lopencv_shape320d

SOURCES += main.cpp\
    mainwindow.cpp \
    test.cpp \
    FuroRobot.cpp \
    FuroMoterController.cpp \
    Device.cpp \
    node.cpp


HEADERS  += mainwindow.h \
    pidcontroller.h \
    test.h \
    FuroRobot.h \
    Device.h \
    FuroMoterController.h \
    node.h


FORMS    += mainwindow.ui
