cmake_minimum_required(VERSION 3.6)
project(Proyecto_ComputacionDistribuida)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Hasher-SH-256/sha256.c
        Hasher-SH-256/sha256.h
        Hasher-SH-256/sha256_test.c)

include_directories(.)
include_directories(Hasher-SH-256)

add_executable(Proyecto_ComputacionDistribuida ${SOURCE_FILES})