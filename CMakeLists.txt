
project(Basilico)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE string "clang compiler" FORCE)
set(CMAKE_CXX_STANDARD 17)

# Boost
find_package(Boost 1.55.0 REQUIRED COMPONENTS system coroutine context thread)
message("Boost:${Boost_INCLUDE_DIR}")

# std::thread
# find_package(Threads REQUIRED)

# NeoVim client

find_library (
    NEOVIM_LIB
    NAMES nvim_client libnvim_client.so # what to look for
    HINTS "${CMAKE_SOURCE_DIR}/neovim.cpp/build" # where to look
    NO_DEFAULT_PATH # do not search system default paths
    )


# Qt5
find_package(Qt5 5.11.0 REQUIRED Widgets Core Gui WebEngineWidgets QuickWidgets Quick Qml)
message(STATUS "Qt5: ${Qt5Widgets_VERSION_STRING}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    include
    neovim.cpp/include
    neovim.cpp/gen
    ${Boost_INCLUDE_DIR}
    )

link_directories(neovim.cpp/build)

file(GLOB EXECUTABLE src/main.cpp
                     src/NeoVim.cpp
                     src/Basilico.cpp
                     src/BasilPlugin.cpp
                     )

add_executable(basil ${EXECUTABLE})

qt5_use_modules(basil Widgets Core Gui WebEngineWidgets QuickWidgets Quick Qml)
target_link_libraries(basil ${NEOVIM_LIB})
target_link_libraries(basil ${Boost_LIBRARIES})
target_link_libraries(basil Threads::Threads)
target_compile_options(basil PRIVATE "-DMSGPACK_USE_BOOST")
target_compile_options(basil PRIVATE "-g")
