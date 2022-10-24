QT       += core gui openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source/databridge.cpp \
    source/maprenderer.cpp \
    source/main.cpp \
    source/mainwindow.cpp

HEADERS += \
    headers/databridge.h \
    headers/mainwindow.h \
    headers/maprenderer.h

FORMS += \
    forms/mainwindow.ui

TRANSLATIONS += \
    locales/HurrigatorsProject_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -L$$PWD/lib/ -llibosmscout.dll

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32: LIBS += -L$$PWD/lib/ -llibosmscout_client_qt.dll

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32: LIBS += -L$$PWD/lib/ -llibosmscout_map_qt.dll

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
