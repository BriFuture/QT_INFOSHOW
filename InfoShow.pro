#-------------------------------------------------
#
# Project created by QtCreator 2016-11-05T09:51:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InfoShow
TEMPLATE = app


SOURCES += main.cpp\
    ui/mainwindow.cpp \
    utils/StringUtil.cpp \
    utils/PrintUtil.cpp \
    utils/FileUtil.cpp \
    main/NetFlowObject.cpp \
    main/NetInfo.cpp \
    ui/FloatWidget.cpp \
    main/Configure.cpp

HEADERS  += ui/mainwindow.h \
    version.h \
    utils/StringUtil.h \
    utils/FileUtil.h \
    utils/PrintUtil.h \
    main/NetFlowObject.h \
    main/NetInfo.h \
    ui/FloatWidget.h \
    main/Configure.h

FORMS    += ui/mainwindow.ui \
    ui/FloatWidget.ui

RESOURCES += \
    res.qrc
