TEMPLATE = app
TARGET = kmetronome
DEPENDPATH += . src
INCLUDEPATH += . src
VERSION = 1.4.1
QT += core gui widgets dbus svg
CONFIG += c++11 link_pkgconfig lrelease embed_translations
DEFINES += VERSION=$$VERSION \
    TRANSLATIONS_PATH=':/'
LRELEASE_DIR='.'
QM_FILES_RESOURCE_PREFIX='/'

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
    src/helpwindow.h \
    src/iconutils.h \
    src/drumgridmodel.h \
    src/instrument.h \
    src/kmetronome.h \
    src/kmetropreferences.h \
    src/sequenceradapter.h \
    src/about.h \
    src/lcdnumberview.h

FORMS += src/about.ui \
    src/drumgrid.ui \
    src/kmetropreferencesbase.ui \
    src/kmetronome.ui

SOURCES += src/drumgrid.cpp \
    src/helpwindow.cpp \
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
    doc/docs.qrc \
    src/lcdnumbers.qrc \
    data/datafiles.qrc

TRANSLATIONS += \
    translations/kmetronome_cs.ts \
    translations/kmetronome_de.ts \
    translations/kmetronome_es.ts \
    translations/kmetronome_fr.ts \
    translations/kmetronome_tr.ts

LCONVERT_LANGS=cs de es fr tr
include(lconvert.pri)
