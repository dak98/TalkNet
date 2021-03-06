#-------------------------------------------------
#
# Project created by QtCreator 2019-09-18T19:43:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
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

QMAKE_CXXFLAGS += -std=c++17

SOURCES += src/client.cpp \
           ../common/QConsole.cpp \
           src/cli_handlers.cpp

HEADERS += ../common/QConsole.hpp \
           ../common/utility.hpp \
           include/cli_handlers.hpp

INCLUDEPATH += include \
               ../common \
               /usr/local/include


LIBS += -L/usr/lib -L/usr/local/lib -lsockio

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
