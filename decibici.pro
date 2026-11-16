include(qble/qble.pri)

TARGET = decibici

CONFIG += c++17 sailfishapp

SOURCES += src/decibici.cpp \
    src/cowboydevice.cpp \
    src/devicemanager.cpp \
    src/onoffservice.cpp \
    src/settingsservice.cpp

DISTFILES += qml/decibici.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Main.qml \
    qml/pages/About.qml \
    qml/resources/icons.qrc \
    rpm/decibici.spec \
    translations/*.ts \
    decibici.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += sailfishapp_i18n

RESOURCES += \
    qml/resources/icons.qrc \

HEADERS += \
    src/cowboydevice.h \
    src/devicemanager.h \
    src/onoffservice.h \
    src/protobufdecoder.h \
    src/settingsservice.h
