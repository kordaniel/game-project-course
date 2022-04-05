#!/usr/bin/env bash

# Executable script that sets up the buildsystem. Sets up a DEBUG target by default
# and a release target can be built by passing one single argument that starts with r
# ./configure.sh           => DEBUG build with tests
# ./configure.sh r[elease] => Optimized RELEASE version without any tests

# Note: You can pass cmake the flag -DCMAKE_CXX_COMPILER=<compiler-binary> to use
# any specific compiler of your chose, for example g++-11 on MacOS to use the GNU compiler.

_BUILDDIR="build"

if [ $# -eq 1 ] && [[ "$1" == r* ]]; then
    BUILDTYPE="Release"
else
    BUILDTYPE="Debug"
fi

_BUILDDIR="${_BUILDDIR}/$(echo "$BUILDTYPE" | tr '[:upper:]' '[:lower:]')"

echo "Configuring buildsystem for $(echo "$BUILDTYPE" | tr '[:lower:]' '[:upper:]') build"
cmake -DCMAKE_BUILD_TYPE="${BUILDTYPE}" -S. -B"${_BUILDDIR}"

