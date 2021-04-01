# Drumstick Metronome - ALSA Sequencer based MIDI metronome
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

cmake_minimum_required(VERSION 3.14)

project ( kmetronome
    VERSION 1.2.1
    LANGUAGES CXX
    DESCRIPTION "ALSA Sequencer based MIDI metronome"
    HOMEPAGE_URL "https://kmetronome.sourceforge.io"
)

option(EMBED_TRANSLATIONS "Embed translations instead of installing" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_admin")
include(SCMRevision)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED Yes)

find_package(Qt5 5.12 COMPONENTS Widgets DBus Svg LinguistTools REQUIRED)
if(Qt5_FOUND)
    message(STATUS "Found Qt5 version: ${Qt5Core_VERSION_STRING}")
endif()

find_package(Drumstick 2.0 COMPONENTS ALSA REQUIRED)
if(Drumstick_FOUND)
    message(STATUS "Found Drumstick version: ${Drumstick_VERSION}")
endif()

message (STATUS "${PROJECT_NAME} v${PROJECT_VERSION}
    install prefix: ${CMAKE_INSTALL_PREFIX}
    Build configuration: ${CMAKE_BUILD_TYPE}
    Processor: ${CMAKE_SYSTEM_PROCESSOR}
    Qt5: ${Qt5_VERSION}
    Drumstick: ${Drumstick_VERSION}
    Embed translations: ${EMBED_TRANSLATIONS}")

include(GNUInstallDirs)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wzero-as-null-pointer-constant -Wsuggest-override")
endif()

add_subdirectory(icons)
if(NOT EMBED_TRANSLATIONS)
    add_subdirectory(translations)
endif()
add_subdirectory(doc)
add_subdirectory(data)
add_subdirectory(src)

include(DBusMacros)
dbus_add_activation_service(net.sourceforge.kmetronome.service.in)

configure_file("${CMAKE_SOURCE_DIR}/kmetronome.spec.in"
    "${CMAKE_SOURCE_DIR}/kmetronome.spec"
    IMMEDIATE @ONLY)

configure_file(
    "${CMAKE_SOURCE_DIR}/cmake_admin/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

add_custom_target ( tarball
    COMMAND mkdir -p kmetronome-${PROJECT_VERSION}
    COMMAND cp -r cmake_admin kmetronome-${PROJECT_VERSION}
    COMMAND cp -r data kmetronome-${PROJECT_VERSION}
    COMMAND cp -r doc kmetronome-${PROJECT_VERSION}
    COMMAND cp -r icons kmetronome-${PROJECT_VERSION}
    COMMAND cp -r translations kmetronome-${PROJECT_VERSION}
    COMMAND cp -r src kmetronome-${PROJECT_VERSION}
    COMMAND cp AUTHORS ChangeLog CMakeLists.txt configure.* COPYING INSTALL kmetronome.{desktop,lsm,pro,spec,spec.in} net.sourceforge.kmetronome.service.in NEWS README TODO kmetronome-${PROJECT_VERSION}
    COMMAND tar -cj -f kmetronome-${PROJECT_VERSION}.tar.bz2 kmetronome-${PROJECT_VERSION}
    COMMAND tar -cz -f kmetronome-${PROJECT_VERSION}.tar.gz kmetronome-${PROJECT_VERSION}
    COMMAND rm -rf kmetronome-${PROJECT_VERSION}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

install( FILES kmetronome.desktop
         DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications )
