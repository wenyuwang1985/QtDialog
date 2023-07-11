QT += core gui widgets

TARGET = QtSign

TEMPLATE = lib
DEFINES += QTSIGNLIB_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QSignDialog/QCertDialog.cpp \
    QSignDialog/QCertEsealDialog.cpp \
    QSignDialog/QRevokeEsealDialog.cpp \
    QtSignDialogLib.cpp \
    clog.c \
    qtsignlib.cpp

HEADERS += \
    QSignDialog/QCertDialog.h \
    QSignDialog/QCertEsealDialog.h \
    QSignDialog/QRevokeEsealDialog.h \
    QtSignDialogLib.h \
    clog.h \
    cpplogex.h \
    qtsignlib.h

# Default rules for deployment.
unix {
    target.path = ./
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    QtSignLibResource.qrc

QMAKE_CFLAGS += -Wl,-rpath=./
QMAKE_RPATHDIR += ./
