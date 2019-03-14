cmake_minimum_required(VERSION 1.0.0)

project(neovim_client CXX)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE string "clang compiler" FORCE)
set(CMAKE_CXX_STANDARD 17)

# Boost
find_package(Boost 1.55.0 REQUIRED COMPONENTS system thread)

set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-DMSGPACK_USE_BOOST ${CMAKE_CXX_FLAGS}")

find_package(PythonInterp)
if (PYTHONINTERP_FOUND)
    add_custom_target(auto_gen_api
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/gen_api.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Autogen nvim interface"
    )
endif()

add_library(nvim_client SHARED src/neovim.cpp src/socket.cpp)
# target_compile_options(nvim_client PRIVATE "-g")

include_directories(
    include
    gen
    ${Boost_INCLUDE_DIR}
)

add_executable(test src/main.cpp)
target_link_libraries(test nvim_client)
target_link_libraries(test ${Boost_LIBRARIES})
add_dependencies(test auto_gen_api nvim_client)

