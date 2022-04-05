#!/usr/bin/env bash

# This script will build the debug or release target, pass r as the only argument
# to this script to build the release target.
# ./configure.sh           => DEBUG build with tests
# ./configure.sh r[elease] => Optimized RELEASE version without any tests

# Set _VERBOSE=1 to set make to produce verbose output
_VERBOSE=0
_BUILDDIR="build"

if [ $# -eq 1 ] && [[ "$1" == r* ]]; then
    BUILDTYPE="Release"
else
    BUILDTYPE="Debug"
fi

_BUILDDIR="${_BUILDDIR}/$(echo "$BUILDTYPE" | tr '[:upper:]' '[:lower:]')"

echo "Attempting to build the $(echo "$BUILDTYPE" | tr '[:lower:]' '[:upper:]') build"

if [ ! -d "${_BUILDDIR}" ]; then
    echo "The build directory './${_BUILDDIR}' is missing. Aborting."
    echo "Run ./configure.sh [r] first to initialize the build environment"
    exit 1
fi

echo "Build system found, building"

if [[ "${_VERBOSE}" == 1 ]]; then
    make VERBOSE=1 -C "${_BUILDDIR}"
else
    make -C "${_BUILDDIR}"
fi

