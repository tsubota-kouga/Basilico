#!/bin/sh

if [ -e neovim.cpp/build ]; then
    cd neovim.cpp/build
else
    mkdir neovim.cpp/build
    cd neovim.cpp/build && cmake ..
fi
# make

cd ../../
if [ -e build ]; then
    cd build/
else
    mkdir build
    cd build && cmake ..
fi
make
cd ../
