#!/usr/bin/env bash

if [ -e build ]; then
  trash build
fi

mkdir build
cd build


cmake -DUSD_ROOT="/opt/usd" -DCMAKE_INSTALL_PREFIX="/Users/Dev/Projects/cache/mini-maya/miniMaya" ..

cmake --build  . -j8 -- mini-maya-executable

