include(qble/qble.pri)

TARGET = decibici

CONFIG += sailfishapp

SOURCES += src/decibici.cpp \
    src/cowboydevice.cpp \
    src/devicemanager.cpp \
    src/onoffservice.cpp

DISTFILES += qml/decibici.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Main.qml \
    rpm/decibici.spec \
    translations/*.ts \
    decibici.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += sailfishapp_i18n

HEADERS += \
    src/cowboydevice.h \
    src/devicemanager.h \
    src/onoffservice.h
