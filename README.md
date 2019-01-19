# Basilico

GUI Extension for NeoVim in C++

**Note**:Only support Linux OS, it may not work on others.

## Required

* Neovim 0.3.2 or later
    - [Neovim releases][Neovim]

* Qt5.7 or later
    - [Qt Download][Qt]

    - **Note**: tested with only Qt5.11

* Boost Library
    - You can install Boost Library by using package manager

    - Required `libboost-thread` and `libboost-system`

* Msgpack-c 2.0 or later
    - [Msgpack git repository][Msgpack]

    - You have to do `git clone` and build

* C++ compiler
    - compiler which can build c++17 such as clang, g++ etc

* CMake

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

## How to install Basilico GUI Plugins

You can install Basilico GUI Plugins by using your plugin manager.

If you have dein.vim, you can write in your dein.toml
```toml
[[plugins]]
repo='tsubota-kouga'
build='./build.sh'
hooksource="""
let g:basilico_plugin_default_dir=expand('~/.cache/dein/repos/github.com')
"""
```
