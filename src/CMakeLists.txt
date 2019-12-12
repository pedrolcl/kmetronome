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

set(kmetronome_SRCS
    about.h
    drumgrid.h
    drumgridmodel.h
    kmetronome.h
    kmetropreferences.h
    lcdnumberview.h
    sequenceradapter.h
    defs.h
    instrument.h
    about.cpp
    drumgrid.cpp
    drumgridmodel.cpp
    instrument.cpp
    kmetronome.cpp
    kmetropreferences.cpp
    lcdnumberview.cpp
    main.cpp
    sequenceradapter.cpp
    about.ui
    drumgrid.ui
    kmetronome.ui
    kmetropreferencesbase.ui
    kmetronome.qrc
    lcdnumbers.qrc
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

target_compile_definitions( kmetronome
    PRIVATE -DVERSION=${PROJECT_VERSION}
)

install( TARGETS kmetronome
         RUNTIME DESTINATION bin )

install( FILES net.sourceforge.kmetronome.xml 
         DESTINATION share/dbus-1/interfaces )
