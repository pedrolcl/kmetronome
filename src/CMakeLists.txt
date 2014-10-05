# KMetronome - ALSA Sequencer based MIDI metronome
# Copyright (C) 2005-2014 Pedro Lopez-Cabanillas <plcl@users.sourceforge.net>
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

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(kmetronome_qtobject_SRCS
    about.h
    drumgrid.h
    drumgridmodel.h
    kmetronome.h
    kmetropreferences.h
    lcdnumberview.h
    sequenceradapter.h )

set(kmetronome_SRCS 
    defs.h
    about.h
    about.cpp
    drumgrid.h
    drumgrid.cpp
    drumgridmodel.h
    drumgridmodel.cpp
    instrument.h
    instrument.cpp
    kmetronome.h
    kmetronome.cpp
    kmetropreferences.h
    kmetropreferences.cpp
    lcdnumberview.h
    lcdnumberview.cpp
    main.cpp
    sequenceradapter.h
    sequenceradapter.cpp )

set(kmetronome_UI
    about.ui
    drumgrid.ui
    kmetronome.ui
    kmetropreferencesbase.ui )

qt5_wrap_ui( kmetronome_SRCS ${kmetronome_UI} )
qt5_wrap_cpp( kmetronome_moc_SRCS ${kmetronome_qtobject_SRCS} )
qt5_add_resources( kmetronome_SRCS kmetronome.qrc lcdnumbers.qrc)
qt5_add_dbus_adaptor( kmetronome_SRCS 
                      net.sourceforge.kmetronome.xml
                      kmetronome.h 
                      KMetronome 
                      kmetronome_adaptor )

add_executable( kmetronome ${kmetronome_SRCS} ${kmetronome_moc_SRCS} )

target_link_libraries( kmetronome
    Qt5::Widgets
    Qt5::DBus
    Qt5::Svg
    ${DRUMSTICK_LIBRARIES}
    ${ALSA_LIBRARIES}
)

# Install targets
install( TARGETS kmetronome
         RUNTIME DESTINATION bin )

install( FILES net.sourceforge.kmetronome.xml 
         DESTINATION share/dbus-1/interfaces )
