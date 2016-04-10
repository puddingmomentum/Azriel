#-------------------------------------------------
#
# Project created by QtCreator 2016-03-26T19:55:11
#
#-------------------------------------------------

QT       += core gui sql positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Azriel
TEMPLATE = app


SOURCES += main.cpp\
        azriel.cpp \
    port.cpp \
    route.cpp \
    portmanage.cpp \
    newport.cpp \
    controller.cpp

HEADERS  += azriel.h \
    controller.h \
    port.h \
    route.h \
    portmanage.h \
    newport.h \

FORMS    += azriel.ui \
    newport.ui
