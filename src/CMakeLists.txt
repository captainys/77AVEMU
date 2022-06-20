cmake_minimum_required(VERSION 3.2)
project(MUTSU)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../TOWNSEMU/src/cpputil ${CMAKE_BINARY_DIR}/cpputil)
add_subdirectory(../../TOWNSEMU/src/device ${CMAKE_BINARY_DIR}/device)
add_subdirectory(../../TOWNSEMU/src/vmbase ${CMAKE_BINARY_DIR}/vmbase)

add_subdirectory(mc6809)
add_subdirectory(mc6809util)
add_subdirectory(memory)
add_subdirectory(fm77av)
