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
    rm -rfv ./* && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && cmake --build .
else
    cmake --build .
fi

./fontGen ../../../resources/FZSTK.TTF gbFont
