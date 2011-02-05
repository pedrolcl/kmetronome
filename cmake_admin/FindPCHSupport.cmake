# Copyright (C) 2005-2011 Pedro Lopez-Cabanillas <plcl@users.sourceforge.net>
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Try to find precompiled headers support for GCC 3.4 and 4.x
# Once done this will define:
#
# Variable:
#   PCHSupport_FOUND
#
# Macro:
#   ADD_PRECOMPILED_HEADER

cmake_policy(SET CMP0007 OLD)

IF(CMAKE_COMPILER_IS_GNUCXX)
    EXEC_PROGRAM(${CMAKE_CXX_COMPILER}
                 ARGS -dumpversion
                 OUTPUT_VARIABLE gcc_compiler_version)
    IF(gcc_compiler_version MATCHES "4\\.[0-9].*")
        SET(PCHSupport_FOUND TRUE)
    ELSE(gcc_compiler_version MATCHES "4\\.[0-9].*")
        IF(gcc_compiler_version MATCHES "3\\.4.*")
            SET(PCHSupport_FOUND TRUE)
        ENDIF(gcc_compiler_version MATCHES "3\\.4.*")
    ENDIF(gcc_compiler_version MATCHES "4\\.[0-9].*")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

MACRO(ADD_PRECOMPILED_HEADER _targetName _input)
    GET_FILENAME_COMPONENT(_name ${_input} NAME)
    SET(_source "${CMAKE_CURRENT_SOURCE_DIR}/${_input}")
    SET(_outdir "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch")
    MAKE_DIRECTORY(${_outdir})
    IF(NOT CMAKE_BUILD_TYPE)
        SET(_output "${_outdir}/Custom.c++")
    ELSE(NOT CMAKE_BUILD_TYPE)
        SET(_output "${_outdir}/${CMAKE_BUILD_TYPE}.c++")
    ENDIF(NOT CMAKE_BUILD_TYPE)
    STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
    SET(_compiler_FLAGS ${CMAKE_CXX_FLAGS} ${${_flags_var_name}})
    SEPARATE_ARGUMENTS(_compiler_FLAGS)
    GET_DIRECTORY_PROPERTY(_flags COMPILE_FLAGS)
    SEPARATE_ARGUMENTS(_flags)
    LIST(APPEND _compiler_FLAGS ${_flags})
    #MESSAGE("_compiler_FLAGS: ${_compiler_FLAGS}")
    GET_DIRECTORY_PROPERTY(_dir_FLAGS INCLUDE_DIRECTORIES)
    FOREACH(item ${_dir_FLAGS})
        LIST(APPEND _include_FLAGS "-I${item}")
    ENDFOREACH(item)
    #MESSAGE("_include_FLAGS: ${_include_FLAGS}")
    GET_DIRECTORY_PROPERTY(_definitions24 DEFINITIONS)
    #MESSAGE("_definitions24: ${_definitions24}")
    GET_DIRECTORY_PROPERTY(_definitions26 COMPILE_DEFINITIONS)
    #MESSAGE("_definitions26: ${_definitions26}")
    SET(_definitions ${_definitions24})
    SEPARATE_ARGUMENTS(_definitions)
    FOREACH(def ${_definitions26})
        LIST(APPEND _definitions "-D${def}")
    ENDFOREACH(def)
    STRING(TOUPPER "COMPILE_DEFINITIONS_${CMAKE_BUILD_TYPE}" _defs_var_name)
    GET_DIRECTORY_PROPERTY(_definitions_buildtype ${_defs_var_name})
    #MESSAGE("_definitions_buildtype: ${_definitions_buildtype}")
    FOREACH(def ${_definitions_buildtype})
        LIST(APPEND _definitions "-D${def}")
    ENDFOREACH(def)
    LIST(REMOVE_DUPLICATES _definitions)    
    #MESSAGE("_definitions: ${_definitions}")
    ADD_CUSTOM_COMMAND(
        OUTPUT ${_output}
        COMMAND ${CMAKE_CXX_COMPILER}
           ${_compiler_FLAGS}
           ${_include_FLAGS}
           ${_definitions}
           -x c++-header
           -o ${_output} ${_source}
        DEPENDS ${_source} )
    ADD_CUSTOM_TARGET(${_targetName} DEPENDS ${_output})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${CMAKE_CURRENT_BINARY_DIR}/${_name} -Winvalid-pch")
ENDMACRO(ADD_PRECOMPILED_HEADER)
