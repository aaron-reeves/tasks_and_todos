QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = tasks
TEMPLATE = app
CONFIG += console
#CONFIG -= app_bundle



# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
  sharedCode

SOURCES += \
    main.cpp \
    sharedCode/ar_general_purpose/qcout.cpp \
    sharedCode/ar_general_purpose/strutils.cpp \
    sharedCode/ar_general_purpose/cfilelist2.cpp \
    nongui/ctask.cpp \
    cmainwindow.cpp \
    ctasktreewidgetitem.cpp \
    cmytreewidget.cpp \
    ctaskeditform.cpp
    
HEADERS += \
    sharedCode/ar_general_purpose/qcout.h \
    sharedCode/ar_general_purpose/strutils.h \
    sharedCode/ar_general_purpose/cfilelist2.h \
    nongui/ctask.h \
    cmainwindow.h \
    ctasktreewidgetitem.h \
    cmytreewidget.h \
    ctaskeditform.h

FORMS    += cmainwindow.ui \
    ctaskeditform.ui