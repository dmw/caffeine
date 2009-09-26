### -*- mode: cmake; indent-tabs-mode: t; tab-width: 4; cmake-tab-width: 4; -*-
### $Id$
### project
PROJECT(caffeine C)
cmake_minimum_required(VERSION 2.6)

SUBDIRS(
	caf
	src
	test)

include(FindDoxygen)

if (EXISTS "${DOXYGEN}")
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc.cfg")
        set (DOXYGEN_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/doc.cfg")
        add_custom_target ("updoc" "${DOXYGEN}" "-u ${DOXYGEN_CONFIG_FILE}")
        add_custom_target ("doc" "${DOXYGEN}" "${DOXYGEN_CONFIG_FILE}")
    endif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc.cfg")
endif (EXISTS "${DOXYGEN}")

set (ADDITIONAL_MAKE_CLEAN_FILES
    cmake_install.cmake
    CMakeFiles
    CMakeCache.txt)

