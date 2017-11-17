#!/bin/bash

if ! [ -f ./build.sh ]
then
    echo "need run in dir:fontGen"
    exit 1
fi

if ! [ -d build ]
then
    mkdir build
fi

cd build

if [ "$1" = "-r" ]
then
    #rebuilding
    rm -rfv ./* && cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON .. && cmake --build .
else
    cmake --build .
fi

