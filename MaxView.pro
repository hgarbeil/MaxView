QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    modis_hdf.cpp \
    modisfiles.cpp \
    sinu_1km.cpp \
    surftemp.cpp

HEADERS += \
    imwidget.h \
    mainwindow.h \
    modis_hdf.h \
    modisfiles.h \
    sinu_1km.h \
    surftemp.h \
    trig.h

FORMS += \
    mainwindow.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += "C:\\Program Files\\HDF_Group\\HDF\\4.2.14\\include"
LIBS += "-LC:\\Program Files\HDF_Group\HDF\4.2.14\lib"
#INCLUDEPATH += /home/harold/lhome/external/hdf-4.2.13/include
#DEPENDPATH += /home/harold/lhome/external/hdf-4.2.13/include
#LIBS +=  -L/home/harold/lhome/external/hdf-4.2.13/lib

LIBS +=    -lmfhdf   -ljpeg -lm

