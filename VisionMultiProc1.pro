#-------------------------------------------------
#
# Project created by QtCreator 2017-05-29T08:28:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisionMultiProc1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    procconfiginfo.cpp \
    sliderinfo.cpp

HEADERS += \
        mainwindow.h \
    procconfiginfo.h \
    sliderinfo.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += C:\\opencv\\build\\install\\include

CONFIG( debug, debug|release ) {
LIBS += -LC:\opencv\build\install\x64\vc12\lib \
    -lopencv_core320d \
    -lopencv_cudaarithm320d \
    -lopencv_cudabgsegm320d \
    -lopencv_cudacodec320d \
    -lopencv_cudafeatures2d320d \
    -lopencv_cudafilters320d \
    -lopencv_cudaimgproc320d \
    -lopencv_cudalegacy320d \
    -lopencv_cudaobjdetect320d \
    -lopencv_cudaoptflow320d \
    -lopencv_cudastereo320d \
    -lopencv_cudawarping320d \
    -lopencv_cudev320d \
    -lopencv_face320d \
    -lopencv_features2d320d \
    -lopencv_flann320d \
    -lopencv_highgui320d \
    -lopencv_imgcodecs320d \
    -lopencv_imgproc320d \
    -lopencv_ml320d \
    -lopencv_objdetect320d \
    -lopencv_photo320d \
    -lopencv_shape320d \
    -lopencv_stitching320d \
    -lopencv_superres320d \
    -lopencv_video320d \
    -lopencv_videoio320d \
    -lopencv_videostab320d \
    -lopencv_cudafeatures2d320d \
    -lopencv_xfeatures2d320d \
}
else {
LIBS += -LC:\opencv\build\install\x64\vc12\lib \
    -lopencv_core320 \
    -lopencv_cudaarithm320 \
    -lopencv_cudabgsegm320 \
    -lopencv_cudacodec320 \
    -lopencv_cudafeatures2d320 \
    -lopencv_cudafilters320 \
    -lopencv_cudaimgproc320 \
    -lopencv_cudalegacy320 \
    -lopencv_cudaobjdetect320 \
    -lopencv_cudaoptflow320 \
    -lopencv_cudastereo320 \
    -lopencv_cudawarping320 \
    -lopencv_cudev320 \
    -lopencv_face320 \
    -lopencv_features2d320 \
    -lopencv_flann320 \
    -lopencv_highgui320 \
    -lopencv_imgcodecs320 \
    -lopencv_imgproc320 \
    -lopencv_ml320 \
    -lopencv_objdetect320 \
    -lopencv_photo320 \
    -lopencv_shape320 \
    -lopencv_stitching320 \
    -lopencv_superres320 \
    -lopencv_video320 \
    -lopencv_videoio320 \
    -lopencv_videostab320 \
    -lopencv_cudafeatures2d320 \
    -lopencv_xfeatures2d320 \
}

