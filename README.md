# Basilico

GUI Extension for NeoVim in C++

**Note**:Only support Linux, it may not work on others.

## Required

* Neovim 0.3.2 or later
    - [Neovim releases][Neovim]

* Qt5.7 or later
    - [Qt Download][Qt]

    - On Ubuntu18.04, default Qt version is 5.9 or later now

* Boost Library
    - You can install Boost Library by using package manager

    - Required `libboost-thread` and `libboost-system`
    ```shell
    sudo apt install libboost-thread-dev libboost-system-dev
    ```

* C++ compiler
    - compiler which can build c++17 such as clang, g++ etc (You will have one)

* CMake
```shell
sudo apt install cmake
```

* Msgpack-c 2.0 or later
    - [Msgpack git repository][Msgpack]

    - You have to do `git clone` and build
    ```shell
    cd path/to/anydir/
    git clone https://github.com/msgpack/msgpack-c
    cd msgpack-c
    mkdir build
    cd build && cmake .. && make
    make install
    ```

* Jinja2
    ```shell
    pip install jinja2
    ```

You can install with this

[Neovim]:https://github.com/neovim/neovim/releases
[Qt]:https://www.qt.io/download
[Msgpack]:https://github.com/msgpack/msgpack-c

## Install

If you have dein.vim, you can write in your dein.toml
```toml
[[plugins]]
repo='tsubota-kouga/Basilico'
build='bash build.sh'
```
or write in init.vim
```vimscript
call dein#add('tsubota-kouga/Basilico')
```
**Note**: You can **NOT** write in your dein_lazy.toml

