### -*- mode: cmake; indent-tabs-mode: t; tab-width: 4; cmake-tab-width: 4; -*-
### $Id$
### project
PROJECT(caffeine C)
SUBDIRS(
	caf
	src
	test)

include(FindDoxygen)

if (EXISTS "${DOXYGEN}")
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc.cfg")
        set (DOXYGEN_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/doc.cfg")
        add_custom_target ("documentation" "${DOXYGEN}" "${DOXYGEN_CONFIG_FILE}")
    endif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc.cfg")
endif (EXISTS "${DOXYGEN}")

set (ADDITIONAL_MAKE_CLEAN_FILES
    cmake_install.cmake
    CMakeFiles
    CMakeCache.txt)

