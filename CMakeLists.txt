cmake_minimum_required(VERSION 3.5)

project(h07_bulk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(h07_bulk
    main.cpp
    class_input.cpp
    class_input.h
    class_out.h
    )
