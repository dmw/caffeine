#!/bin/bash
#
#
#

export VERBOSE="1"
export CAFFINE_DEBUG="ON"
export GCT_DEBUG="ON"
export CFLAGS="-O2 -march=prescott"
export CXXFLAGS="-O2 -march=prescott"
export CMAKE_OPTIONS="-DCAFFEINE_ARCH=prescott -DCAFFINE_OPTLEVEL=2"
export CMAKE_OPTIONS="$CMAKE_OPTIONS -DGCT_ARCH=prescott -DGCT_OPTLEVEL=2"
export CMAKE_OPTIONS=""
export COMPILE_FLGS="$CFLAGS"
export cmake_files='
*.kdevelop
*.kdevelop.filelist
*.kdevelop.pcs
*.kdevses
*.tags
.kdbgrc.*
Doxyfile
cmake_install.cmake
CMakeCache.txt
CMakeLists.txt~
Makefile
CMakeFiles
config.h
.cccc
log.*
*.core
semantics.cache*
cscope.out*
semantic.cache*
'
export cmake_files_kde='
Doxyfile
*.kdevelop
*.kdevelop.filelist
*.kdevelop.pcs
*.kdevses
*.tags
'

function cmake_kdevelop () {
    if test -r ./CMakeLists.txt; then
	cmake $CMAKE_OPTIONS -G KDevelop3 .
    fi
}

function cmake_build () {
    if test -r ./CMakeLists.txt; then
	cmake $CMAKE_OPTIONS .
    fi
}

function cmake_clean_backups () {
    if test -r ./CMakeLists.txt; then
	for cmf in $cmake_files_kde; do
	    find . -name "*.*~" -exec rm -v -f -r {} \;
	done
    fi
}

function cmake_clean () {
    if test -r ./CMakeLists.txt; then
	for cmf in $cmake_files; do
	    find . -name "$cmf" -exec rm -v -f -r {} \;
	    find . -name "$cmf~" -exec rm -v -f -r {} \;
	    find . -name "*.*~" -exec rm -v -f -r {} \;
	done
    fi
}

function make_clean () {
    make clean
}

function make_build () {
    make_clean
    make
}

function cmk_help () {
    echo "usage:"
    echo "$0 -mcb        normal clean/build"
    echo "$0 -mb         normal build"
    echo "$0 -mc         normal clean"
    echo "$0 -kcb        kdevelop clean/build"
    echo "$0 -kc         kdevelop build"
}

if test "$#" = "1";
then
    case "$1" in
	-mcb)
	    make_clean
	    cmake_clean
	    cmake_build
	    make_build
	    ;;
	-mb)
	    cmake_build
	    make_build
	    ;;
	-mc)
	    make_clean
	    cmake_clean
	    ;;
	-kcb)
	    make_clean
	    cmake_clean
	    cmake_kdevelop
	    ;;
	-kc)
	    make_clean
	    cmake_clean_backups
	    ;;
	-h)
	    cmk_help
	    ;;
	*)
	    cmk_help
	    ;;
    esac
    echo "$0: done all jobs"
else
    echo "$0: too few arguments"
    exit 1
fi


