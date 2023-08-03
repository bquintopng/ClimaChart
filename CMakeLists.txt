cmake_minimum_required(VERSION 3.24)
project(ClimaChart)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include(FetchContent)
FETCHCONTENT_DECLARE(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master) # or whatever tag you want
FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(ClimaChart main.cpp DataPoint.h)
target_link_libraries(ClimaChart PUBLIC matplot)
