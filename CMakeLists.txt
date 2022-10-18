cmake_minimum_required(VERSION 3.23)
project(lab6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")    

include_directories(.)

add_executable(lab6
        ConsoleGfx.cpp
        ConsoleGfx.h
        Image.cpp
        Image.h
        scaler.cpp)
