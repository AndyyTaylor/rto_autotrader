
cmake_minimum_required(VERSION 3.13)

project("AUTOTRADER" CXX)
add_library(${PROJECT_NAME} INTERFACE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config")

enable_testing()
include(CTest)

find_package(gsl-lite CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)

include(6771)

add_subdirectory(source)
# add_subdirectory(test)

include_directories(include)