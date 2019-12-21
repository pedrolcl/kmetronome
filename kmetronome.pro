TEMPLATE = app
TARGET = kmetronome
DEPENDPATH += . src
INCLUDEPATH += . src
VERSION = 1.0.1
QT += core gui widgets dbus svg
CONFIG += link_pkgconfig
DEFINES += VERSION=$$VERSION

packagesExist(alsa) {
    PKGCONFIG += alsa
}

packagesExist(drumstick-alsa) {
    PKGCONFIG += drumstick-alsa
} else {
    INCLUDEPATH += $$(DRUMSTICKINCLUDES)
    LIBS += -L$$(DRUMSTICKLIBS) -ldrumstick-alsa
}

DBUS_ADAPTORS += src/net.sourceforge.kmetronome.xml

HEADERS += src/drumgrid.h \
    src/iconutils.h \
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
    src/iconutils.cpp \
    src/drumgridmodel.cpp \
    src/instrument.cpp \
    src/kmetronome.cpp \
    src/kmetropreferences.cpp \
    src/main.cpp \
    src/sequenceradapter.cpp \
    src/about.cpp \
    src/lcdnumberview.cpp

RESOURCES += src/kmetronome.qrc \
    src/lcdnumbers.qrc \
    data/datafiles.qrc

TRANSLATIONS += translations/cs.ts \
    translations/de.ts \
    translations/es.ts \
    translations/fr.ts \
    translations/tr.ts
