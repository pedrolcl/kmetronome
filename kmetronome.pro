TEMPLATE = app
TARGET = kmetronome
DEPENDPATH += . src
INCLUDEPATH += . src
VERSION = 1.0.1
QT += core gui widgets dbus svg

DEFINES += VERSION=$$VERSION

PKGCONFIG += alsa drumstick-alsa
CONFIG += link_pkgconfig

DBUS_ADAPTORS += src/net.sourceforge.kmetronome.xml

HEADERS += src/drumgrid.h \
    src/drumgridmodel.h \
    src/instrument.h \
    src/kmetronome.h \
    src/kmetropreferences.h \
    src/sequenceradapter.h \
    src/about.h \
    src/lcdnumberview.h

FORMS +=   src/about.ui \
    src/drumgrid.ui \
    src/kmetropreferencesbase.ui \
    src/kmetronome.ui

SOURCES += src/drumgrid.cpp \
    src/drumgridmodel.cpp \
    src/instrument.cpp \
    src/kmetronome.cpp \
    src/kmetropreferences.cpp \
    src/main.cpp \
    src/sequenceradapter.cpp \
    src/about.cpp \
    src/lcdnumberview.cpp

RESOURCES += src/kmetronome.qrc \
    src/lcdnumbers.qrc

TRANSLATIONS += translations/cs.ts \
    translations/de.ts \
    translations/es.ts \
    translations/fr.ts \
    translations/tr.ts