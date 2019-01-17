#!/bin/sh

if [ -e neovim.cpp/build ]; then
    cd neovim.cpp/build
else
    mkdir neovim.cpp/build
    cd neovim.cpp/build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
fi
make -j`expr 2 \* $(grep cpu.cores /proc/cpuinfo | sort -u | grep -o '[0-9]*')`

cd ../../
if [ -e build ]; then
    cd build/
else
    mkdir build
    cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
fi
make -j`expr 2 \* $(grep cpu.cores /proc/cpuinfo | sort -u | grep -o '[0-9]*')`
cd ../
