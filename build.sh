#!/bin/bash

cd `dirname $0`

echo `pwd`

[ "$1" = "clear" ] && rm -rf build

cmake -Bbuild
cmake --build build/ -j8
