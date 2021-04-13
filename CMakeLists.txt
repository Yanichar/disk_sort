cmake_minimum_required(VERSION 3.19)
project(disk_sort)

set(CMAKE_CXX_STANDARD 14)

add_executable(disk_sort main.cpp
        ISO_Creator_CDFS.cpp
        ISO_Creator_CDFS.h
        CDFS_ISO_9660.h)