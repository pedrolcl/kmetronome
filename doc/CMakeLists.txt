# KMetronome - ALSA Sequencer based MIDI metronome
# Copyright (C) 2005-2021 Pedro Lopez-Cabanillas <plcl@users.sourceforge.net>
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
# along with this program. If not, see <http://www.gnu.org/licenses/>.

set(MANPAGE ${PROJECT_NAME}.1)
include(PandocMacros)
if (BUILD_DOCS)
    if (UNIX AND NOT APPLE)
        add_manpage(${MANPAGE})
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MANPAGE}
                DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
    endif()
    update_helpfiles(cs de fr en es tr)
else()
    if (UNIX AND NOT APPLE)
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${MANPAGE}
                DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
    endif()
    foreach(_lang IN ITEMS "en;es")
        install( FILES ${_lang}/index.html
                 DESTINATION ${CMAKE_INSTALL_DOCDIR}/${PROJECT_NAME}/${_lang} )
    endforeach()
    #install( FILES kmetronome_1.png kmetronome_2.png kmetronome_3.png
    #         DESTINATION ${CMAKE_INSTALL_DOCDIR}/${PROJECT_NAME} )
endif()
