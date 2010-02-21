# KMetronome - ALSA Sequencer based MIDI metronome
# Copyright (C) 2005-2010 Pedro Lopez-Cabanillas <plcl@users.sourceforge.net>
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

IF(NOT KMETRONOME_SOURCE_DIR)
    MESSAGE(FATAL_ERROR "You need to run cmake or ccmake at the root directory")
ENDIF(NOT KMETRONOME_SOURCE_DIR)

IF(USE_PCH)
    ADD_PRECOMPILED_HEADER(precompiled_headers qt_kde.h)
ENDIF(USE_PCH)

INCLUDE_DIRECTORIES(
    ${ALSA_INC_DIR}
    ${DRUMSTICK_INC_DIR}
)

LINK_DIRECTORIES(
    ${ALSA_LIB_DIR}
    ${DRUMSTICK_LIB_DIR}
)

SET(kmetronome_SRCS 
    classicstyle.h
    classicstyle.cpp
    knob.h
    knob.cpp
    kmetronome.h
    kmetronome.cpp
    kmetronomeview.h
    kmetronomeview.cpp
    kmetropreferences.h
    kmetropreferences.cpp
    main.cpp
    qt_kde.h
    defs.h
    sequenceradapter.h
    sequenceradapter.cpp 
)

SET(kmetronome_UI 
    kmetronomeviewbase.ui
    kmetropreferencesbase.ui )
	
QT4_ADD_RESOURCES(kmetronome_SRCS kmetronome.qrc)
KDE4_ADD_UI_FILES(kmetronome_SRCS ${kmetronome_UI})
QT4_ADD_DBUS_ADAPTOR(kmetronome_SRCS 
                     net.sourceforge.kmetronome.xml
                     kmetronome.h KMetronome)
KDE4_ADD_EXECUTABLE(kmetronome ${kmetronome_SRCS})

IF(USE_PCH)
    ADD_DEPENDENCIES(kmetronome precompiled_headers)
ENDIF(USE_PCH)

TARGET_LINK_LIBRARIES(kmetronome
    ${KDE4_KDEUI_LIBS}
    ${ALSA_LIBS}
    ${DRUMSTICK_LIBS}
)

# Install targets
INSTALL( TARGETS kmetronome
         RUNTIME DESTINATION ${BIN_INSTALL_DIR})

INSTALL( FILES net.sourceforge.kmetronome.xml 
         DESTINATION ${DBUS_INTERFACES_INSTALL_DIR} )

INSTALL( FILES kmetronomeui.rc 
         DESTINATION ${DATA_INSTALL_DIR}/kmetronome )
