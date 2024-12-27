#[===========================================================================[
  Drumstick Metronome - ALSA Sequencer based MIDI metronome
  Copyright (C) 2005-2024 Pedro Lopez-Cabanillas <plcl@users.sourceforge.net>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
#]===========================================================================]

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
    if (QT_VERSION VERSION_LESS 5.15.0)
        qt5_add_translation(QM_FILES ${TRANSLATIONS})
    else()
        qt_add_translation(QM_FILES ${TRANSLATIONS})
    endif()
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
    ../doc/docs.qrc
    ${APP_RES}
    ${QT_RES}
)

if (QT_VERSION VERSION_LESS 5.15.0)
    qt5_add_dbus_adaptor( kmetronome_SRCS
        net.sourceforge.kmetronome.xml
        kmetronome.h
        KMetronome
        kmetronome_adaptor
    )
else()
    qt_add_dbus_adaptor( kmetronome_SRCS
        net.sourceforge.kmetronome.xml
        kmetronome.h
        KMetronome
        kmetronome_adaptor
    )
endif()

set_property(SOURCE kmetronome_adaptor.h kmetronome_adaptor.cpp
    PROPERTY SKIP_AUTOGEN ON)

add_executable( kmetronome ${kmetronome_SRCS} )
if (BUILD_DOCS)
    add_dependencies ( kmetronome update_helpfiles )
endif()

target_link_libraries( kmetronome
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Svg
    Drumstick::ALSA
)

if (QT_VERSION VERSION_GREATER_EQUAL 6.0.0)
    target_link_libraries( kmetronome
        Qt6::Gui
        Qt6::SvgWidgets
     )
endif()

target_compile_definitions( kmetronome PRIVATE
    VERSION=${PROJECT_VERSION}
    $<$<BOOL:${EMBED_TRANSLATIONS}>:TRANSLATIONS_EMBEDDED>
)

if (DEFINED PROJECT_WC_REVISION)
    target_compile_definitions( ${PROJECT_NAME} PRIVATE
        REVISION=${PROJECT_WC_REVISION} )
endif()

install( TARGETS kmetronome
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} )

install( FILES net.sourceforge.kmetronome.xml
         DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/interfaces )
