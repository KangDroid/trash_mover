cmake_minimum_required(VERSION 3.15)
project(TrashMover)

if (APPLE)
    set(CMAKE_INSTALL_PREFIX "/usr/local/kangdroid")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(TM_VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "-DKDR_TRASH_MOVER_VER=\"\\\"${TM_VERSION}\\\"\" -Wall -O3 -Wno-deprecated-declarations -Wno-unneeded-internal-declaration")

if(DEFINED CUSTOM_TRASH_LOCATION)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEFAULT_TRASH_LOCATION=\"\\\"${CUSTOM_TRASH_LOCATION}\\\"\"")
endif(DEFINED CUSTOM_TRASH_LOCATION)

if(DEFINED CUSTOM_TRASHDATA_LOCATION)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEFAULT_TRASH_DATA_LOCATION=\"\\\"${CUSTOM_TRASHDATA_LOCATION}\\\"\"")
endif(DEFINED CUSTOM_TRASHDATA_LOCATION)

include_directories(${TrashMover_SOURCE_DIR}/include)

add_executable(TrashMover src/main.cpp src/UserDefinition.cpp src/TrashManager.cpp src/TrashData.cpp)