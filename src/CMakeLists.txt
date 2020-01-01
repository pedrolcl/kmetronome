# KMetronome - ALSA Sequencer based MIDI metronome
# Copyright (C) 2005-2019 Pedro Lopez-Cabanillas <plcl@users.sourceforge.net>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(EMBED_TRANSLATIONS)
    set( TRANSLATIONS
         ../translations/kmetronome_cs.ts
         ../translations/kmetronome_de.ts
         ../translations/kmetronome_es.ts
         ../translations/kmetronome_fr.ts
         ../translations/kmetronome_tr.ts
    )
    qt5_add_translation(QM_FILES ${TRANSLATIONS})
    include(TranslationUtils)
    add_app_translations_resource(APP_RES ${QM_FILES})
    add_qt_translations_resource(QT_RES cs de es fr tr)
endif()

set(kmetronome_SRCS
    about.h
    drumgrid.h
    drumgridmodel.h
    iconutils.h
    kmetronome.h
    kmetropreferences.h
    lcdnumberview.h
    sequenceradapter.h
    defs.h
    instrument.h
    helpwindow.h
    about.cpp
    drumgrid.cpp
    drumgridmodel.cpp
    iconutils.cpp
    instrument.cpp
    kmetronome.cpp
    kmetropreferences.cpp
    lcdnumberview.cpp
    main.cpp
    sequenceradapter.cpp
    helpwindow.cpp
    about.ui
    drumgrid.ui
    kmetronome.ui
    kmetropreferencesbase.ui
    kmetronome.qrc
    lcdnumbers.qrc
    ../data/datafiles.qrc
    ${APP_RES}
    ${QT_RES}
)

qt5_add_dbus_adaptor( kmetronome_SRCS 
    net.sourceforge.kmetronome.xml
    kmetronome.h
    KMetronome
    kmetronome_adaptor
)

set_property(SOURCE kmetronome_adaptor.h kmetronome_adaptor.cpp
    PROPERTY SKIP_AUTOGEN ON)

add_executable( kmetronome ${kmetronome_SRCS} )

target_link_libraries( kmetronome
    Qt5::Widgets
    Qt5::DBus
    Qt5::Svg
    Drumstick::ALSA
)

target_compile_definitions( kmetronome PRIVATE
    -DVERSION=${PROJECT_VERSION}
    -DTRANSLATIONS_PATH=$<IF:$<BOOL:${EMBED_TRANSLATIONS}>,:/,${CMAKE_INSTALL_DATAROOTDIR}/kmetronome/translations/>
)

install( TARGETS kmetronome
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} )

install( FILES net.sourceforge.kmetronome.xml 
         DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/interfaces )
