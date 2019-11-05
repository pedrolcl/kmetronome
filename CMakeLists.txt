# Drumstick Metronome - ALSA Sequencer based MIDI metronome
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

cmake_minimum_required(VERSION 3.1)

project(kmetronome)

set(CMAKE_USE_RELATIVE_PATHS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_admin")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED Yes)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "1")
set(VERSION_SUFFIX "")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_SUFFIX})
add_definitions(-DVERSION=${VERSION})

# User options
message (STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")

# Check for Qt
find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

find_package(PkgConfig REQUIRED)
if(PKG_CONFIG_FOUND)
    message(STATUS "Program pkg-config found (${PKG_CONFIG_EXECUTABLE})")
else()
    message(FATAL_ERROR "Program pkg-config not found")
endif()

pkg_check_modules(DRUMSTICK REQUIRED drumstick-alsa>=1.0.0)
if(DRUMSTICK_FOUND)
    link_directories(${DRUMSTICK_LIBDIR})
    include_directories(${DRUMSTICK_INCLUDEDIR})
    #link_libraries(${DRUMSTICK_LIBRARIES})
else()
    message(FATAL_ERROR "Please install drumstick libraries and headers (package drumstick-devel)")
endif()

pkg_check_modules(ALSA REQUIRED alsa>=1.0)
if(ALSA_FOUND)
    link_directories(${ALSA_LIBDIR})
    include_directories(${ALSA_INCLUDEDIR})
    #link_libraries(${ALSA_LIBRARIES})
else()
    message(FATAL_ERROR "Please install ALSA library and headers (libasound2)")
endif()

message(STATUS "kmetronome ${VERSION} install prefix: ${CMAKE_INSTALL_PREFIX}")

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(translations)
add_subdirectory(doc)
add_subdirectory(data)

INCLUDE(DBusMacros)
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
    COMMAND mkdir -p kmetronome-${VERSION}
    COMMAND cp -r cmake_admin kmetronome-${VERSION}
    COMMAND cp -r data kmetronome-${VERSION}
    COMMAND cp -r doc kmetronome-${VERSION}
    COMMAND cp -r icons kmetronome-${VERSION}
    COMMAND cp -r translations kmetronome-${VERSION}
    COMMAND cp -r src kmetronome-${VERSION}
    COMMAND cp AUTHORS ChangeLog CMakeLists.txt configure.* COPYING INSTALL kmetronome.{desktop,lsm,pro,spec,spec.in} net.sourceforge.kmetronome.service.in NEWS README TODO kmetronome-${VERSION}
    COMMAND tar -cj -f kmetronome-${VERSION}.tar.bz2 kmetronome-${VERSION}
    COMMAND tar -cz -f kmetronome-${VERSION}.tar.gz kmetronome-${VERSION}
    COMMAND rm -rf kmetronome-${VERSION}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

install( FILES kmetronome.desktop
         DESTINATION share/applications )
