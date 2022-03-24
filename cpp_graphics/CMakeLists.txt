cmake_minimum_required(VERSION 3.10)
project(cpp_game)

include_directories(
        /usr/include
        /opt/homebrew/include
)

link_directories(
        /usr/lib
        /opt/homebrew/lib
)

link_libraries(
        -lsfml-graphics
        -lsfml-audio
        -lsfml-network
        -lsfml-system
        -lsfml-window
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(cpp_game main.cpp)