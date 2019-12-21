# Drumstick Metronome - ALSA Sequencer based MIDI metronome
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

cmake_minimum_required(VERSION 3.9)

project(kmetronome
        VERSION 1.0.1.99
        DESCRIPTION "ALSA Sequencer based MIDI metronome"
        LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_admin")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED Yes)

message(STATUS "kmetronome ${PROJECT_VERSION} install prefix: ${CMAKE_INSTALL_PREFIX}")
message (STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")

find_package(Qt5 5.12 COMPONENTS Widgets DBus Svg LinguistTools REQUIRED)
if(Qt5_FOUND)
    message(STATUS "Found Qt5 version: ${Qt5Core_VERSION_STRING}")
endif()

find_package(Drumstick 1.1.3 COMPONENTS ALSA REQUIRED)
if(Drumstick_FOUND)
    message("Found Drumstick version: ${Drumstick_VERSION}")
endif()

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(translations)
add_subdirectory(doc)
add_subdirectory(data)

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
         DESTINATION share/applications )
