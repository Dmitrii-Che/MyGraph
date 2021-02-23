QT += core gui
QT += sql
QT += sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyGraph
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mygraph.cpp \
        reader.cpp

HEADERS += \
        mygraph.h \
        reader.h

FORMS += \
        mygraph.ui
