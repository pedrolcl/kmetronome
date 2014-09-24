TEMPLATE = app
TARGET = kmetronome
DEPENDPATH += . src
INCLUDEPATH += . src
QT += core gui widgets

PKGCONFIG += alsa drumstick-alsa
CONFIG += link_pkgconfig

#DBUS_INTERFACES += src/net.sourceforge.kmetronome.xml

HEADERS += src/drumgrid.h \
           src/drumgridmodel.h \
           src/instrument.h \
           src/kmetronome.h \
           src/kmetronomeview.h \
           src/kmetropreferences.h \
           src/sequenceradapter.h

FORMS +=   src/drumgrid.ui \
           src/kmetronomeviewbase.ui \
           src/kmetropreferencesbase.ui

SOURCES += src/drumgrid.cpp \
           src/drumgridmodel.cpp \
           src/instrument.cpp \
           src/kmetronome.cpp \
           src/kmetronomeview.cpp \
           src/kmetropreferences.cpp \
           src/main.cpp \
           src/sequenceradapter.cpp

RESOURCES += src/kmetronome.qrc

TRANSLATIONS += translations/cs.ts \
          translations/de.ts \
          translations/es.ts \
          translations/fr.ts \
          translations/tr.ts
