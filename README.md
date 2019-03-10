# Basilico

> GUI Extension for NeoVim in C++

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

If you have dein.vim, you can write on your dein.toml
```toml
[[plugins]]
name='tsubota-kouga/Basilico'
build='bash build.sh'
```
or write in init.vim
```vimscript
call dein#add('tsubota-kouga/Basilico')
```
**Note**: You can **NOT** write on your dein_lazy.toml

And make alias
```shell
alias basil='bash path/to/sh/basil'
```

## Customize

You can customize, whether use gui tabline or not.
And if you use gui tabline, you can set Qt Style Sheet to tabline.

**Note**: To use Tabpage Plugin, `g:basilico#ext_tabline` must be `v:true`.

| global val                       | type     | default   | example                        |
|:--------------------------------:|:--------:|:---------:|:------------------------------:|
| `g:basilico#ext_tabline`         | `bool`   | `v:false` | `v:true`                       |
| `g:basilico#tabline_style_sheet` | `string` | `''`      | `'background-color: #444444;'` |


| command name       | explanation                              |
|:------------------:|:----------------------------------------:|
| `BasilGenerate`    | Genarate C++ header, source, and so on   |
| `BasilBuild`       | Build Basilico                           |

To Install Basilico Plugins, please do following

1. add to `dein.toml` or `init.vim`

2. start neovim

3. execute `BasilGenerate` command

4. execute `BasilBuild` command

## Feature

* Tabpage Plugin support

* Dock Widget Plugin support

* Multi Wide Charactor support

* ext_tabline ui_option support

* mouse support


## Future

- [ ] ext_multigrid ui_option support

- [ ] Window Plugin support

- [ ] cursor blinking support

- [ ] vimscript Basilico GUI library


## Licence
MIT Licence except `neovim.cpp` directory.

`neovim.cpp` directory has changed from original.

Original repository is [here](https://github.com/DaikiMaekawa/neovim.cpp). Thanks!

