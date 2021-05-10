cmake_minimum_required(VERSION 3.16)
project(wet1_main)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(wet1_main
        AVLTree.h
        CarDealershipManager.cpp
        CarDealershipManager.h
        library.h
        main.cpp
        ModelGrades.h)
