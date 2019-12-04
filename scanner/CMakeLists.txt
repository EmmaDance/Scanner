cmake_minimum_required(VERSION 3.15)
project(flcd)

set(CMAKE_CXX_STANDARD 14)

add_executable(flcd
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/flcd.cbp
        cmake-build-debug/flcd.exe
        cmake-build-debug/Makefile
        cmake-build-debug/src.txt
        main.cpp
        src.txt)
