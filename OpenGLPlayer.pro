#-------------------------------------------------
#
# Project created by QtCreator 2020-07-07T09:16:47
#
#-------------------------------------------------

QT       += core gui xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += View \
            Setting \
            Model \
            Common \
            Player

SOURCES += \
        main.cpp \
        MainWindow.cpp \
    OpenGLPlayer.cpp \
    OpenRtspDialog.cpp \
    VideoDecoder.cpp

HEADERS += \
        MainWindow.h \
    OpenGLPlayer.h \
    OpenRtspDialog.h \
    VideoDecoder.h

FORMS += \
        MainWindow.ui \
    OpenRtspDialog.ui
  
# Default rules for deployment.
# Default rules for deployment.
unix {
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target
}

win32 {
    MOC_DIR = temp/moc
    RCC_DIR = temp/rcc
    UI_DIR = temp/ui
    OBJECTS_DIR = temp/obj
    DESTDIR = bin
}

#exe icon
#win32:RC_FILE = icon.rc

#RESOURCES += \
#    OpenGLPlayer.qrc

LIBS += -L$$PWD/ffmpeg/lib/ -lhi_h264dec_w \
        -L$$PWD/ffmpeg/lib/ -lavcodec \
        -L$$PWD/ffmpeg/lib/ -lavdevice \
        -L$$PWD/ffmpeg/lib/ -lavfilter \
        -L$$PWD/ffmpeg/lib/ -lavformat \
        -L$$PWD/ffmpeg/lib/ -lavutil \
        -L$$PWD/ffmpeg/lib/ -lpostproc \
        -L$$PWD/ffmpeg/lib/ -lswresample \
        -L$$PWD/ffmpeg/lib/ -lswscale
        
INCLUDEPATH += $$PWD/ffmpeg/include
