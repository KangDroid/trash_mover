cmake_minimum_required(VERSION 3.15)
project(TrashMover)

if (APPLE)
    set(CMAKE_INSTALL_PREFIX "/usr/local/kangdroid")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-DDEFAULT_SAMPLE_DIR=\"\\\"${CMAKE_INSTALL_PREFIX}\\\"\" -Wall -O3 -Wno-deprecated-declarations -Wno-unneeded-internal-declaration")

include_directories(${TrashMover_SOURCE_DIR}/include)

add_executable(TrashMover src/main.cpp)