# Basilico


Neovim GUI Implementation in C++

**Note**:Only support Linux OS, it may not work on others.

## Required

* Neovim 0.3.2-dev or later
    - [Neovim releases][Neovim]

* Qt5.11 or later
    - [Qt Download][Qt]

* Boost Library
    - You can install Boost Library by using package manager

* Msgpack-c
    - [Msgpack git repository][Msgpack]

* C++ compiler
    - required C++ compiler which can build c++17

[Neovim]:https://github.com/neovim/neovim/releases
[Qt]:https://www.qt.io/download
[Msgpack]:https://github.com/msgpack/msgpack-c

## Install


If you have dein.vim, you can write in your dein.toml
```toml
[[plugins]]
repo='tsubota-kouga/Basilico'
build='build.sh'
```
or write in init.vim
```vimscript
call dein#add('tsubota-kouga/Basilico')
```
**Note**: You can **NOT** write in your dein_lazy.toml

If you have NeoBundle,
```vimscript
NeoBundle 'tsubota-kouga/Basilico'
```

If you have Plug
```
Plug 'tsubota-kouga/Basilico'
```

## How to install Basilico

If you have dein.vim, you can write in your dein.toml
```toml
[[plugins]]
repo='tsubota-kouga'
build='./build.sh'
hooksource="""
let g:basilico_plugin_default_dir=expand('~/.cache/dein/repos/github.com')
"""
```

## How to install Basilico Plugins

You can install Basilico GUI Plugins by using your plugin manager.

