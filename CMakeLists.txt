cmake_minimum_required(VERSION 3.6)
project(Proyecto_ComputacionDistribuida)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        crackerSecuencial.c
        CrackerSecuencial/prueba.c)

include_directories(.)
include_directories(Hasher-SHA256)

add_executable(Proyecto_ComputacionDistribuida ${SOURCE_FILES})
