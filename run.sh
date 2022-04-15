#!/usr/bin/env bash

_BINDIR="bin"

if [ ! -d "${_BINDIR}" ]; then
    echo "Directory './${_BINDIR}' missing. Aborting."
    exit 1
fi

cd "${_BINDIR}"


if [ $# -eq 1 ] && [[ "$1" == r* ]]; then
    ./gameproj
else
    ./gameproj-dbg
fi

