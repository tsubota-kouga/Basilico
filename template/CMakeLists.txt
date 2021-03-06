
project(Basilico)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE string "clang compiler" FORCE)
set(CMAKE_CXX_STANDARD 17)

# Boost
find_package(Boost {{message['boost_version']}} REQUIRED COMPONENTS {% for lib in message['boost_dependency']%}{{lib}} {% endfor %})
message("Boost:${Boost_INCLUDE_DIR}")

# NeoVim client
find_library (
    NEOVIM_LIB
    NAMES nvim_client libnvim_client.so # what to look for
    HINTS "${CMAKE_SOURCE_DIR}/neovim.cpp/build" # where to look
    NO_DEFAULT_PATH # do not search system default paths
    )


# Qt5
find_package(Qt5 {{message['qt_version']}} REQUIRED {% for lib in message['qt_dependency']%}{{lib}} {% endfor %})
message(STATUS "Qt5: ${Qt5Widgets_VERSION_STRING}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    include
    neovim.cpp/include
    neovim.cpp/gen
    ${Boost_INCLUDE_DIR}
{% for path in message['plugin_paths']%}
{{path}}/include
{% endfor %}
    )

link_directories(neovim.cpp/build)

qt5_add_resources(RESOURCES Basilico.qrc)

file(GLOB EXECUTABLE src/main.cpp
                     src/NeoVim.cpp
                     src/Basilico.cpp
                     src/BasilPlugin.cpp
{% for path in message['plugin_paths']%}
{{path}}/src/*
{% endfor %}
                     )

 add_executable(basil ${EXECUTABLE} ${RESOURCES})

qt5_use_modules(basil {% for lib in message['qt_dependency']%}{{lib}} {% endfor %})
target_link_libraries(basil ${NEOVIM_LIB})
target_link_libraries(basil ${Boost_LIBRARIES})
target_link_libraries(basil Threads::Threads)
target_compile_options(basil PRIVATE "-DMSGPACK_USE_BOOST")
# target_compile_options(basil PRIVATE "-g")
