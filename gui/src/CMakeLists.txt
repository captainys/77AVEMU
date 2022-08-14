cmake_minimum_required(VERSION 3.15)

project(MUTSU_GUI)

enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(MUTSU_FM77AV)

add_compile_definitions(YS_LITTLE_ENDIAN)
add_compile_definitions(YS_TWOS_COMPLEMENT)
# Believe or not, C/C++ does not assume 2's complement.
# C/C++ does not guarantee that 0xFFFF is interpreted as 16-bit signed negative one.
# In practice, pretty much all CPUs uses 2's complement, and safe to assume it is.
# This flag will skip a little bit of computation.

add_subdirectory(public/src)


set(TSUGARU_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU/src)

add_subdirectory(${TSUGARU_SOURCE_DIR}/cpputil ${CMAKE_BINARY_DIR}/cpputil)
add_subdirectory(${TSUGARU_SOURCE_DIR}/cheapmath ${CMAKE_BINARY_DIR}/cheapmath)
add_subdirectory(${TSUGARU_SOURCE_DIR}/device ${CMAKE_BINARY_DIR}/device)
add_subdirectory(${TSUGARU_SOURCE_DIR}/vmbase ${CMAKE_BINARY_DIR}/vmbase)
add_subdirectory(${TSUGARU_SOURCE_DIR}/lineparser ${CMAKE_BINARY_DIR}/lineparser)
# add_subdirectory(${TSUGARU_SOURCE_DIR}/ym2612 ${CMAKE_BINARY_DIR}/ym2612)  Should be included from ym2203c library.
add_subdirectory(${TSUGARU_SOURCE_DIR}/i8251 ${CMAKE_BINARY_DIR}/i8251)
add_subdirectory(${TSUGARU_SOURCE_DIR}/diskdrive ${CMAKE_BINARY_DIR}/diskdrive)
add_subdirectory(${TSUGARU_SOURCE_DIR}/diskimg ${CMAKE_BINARY_DIR}/diskimg)
add_subdirectory(${TSUGARU_SOURCE_DIR}/osdependent/gamepad ${CMAKE_BINARY_DIR}/gamepad)
add_subdirectory(${TSUGARU_SOURCE_DIR}/externals/d77 ${CMAKE_BINARY_DIR}/d77)
add_subdirectory(${TSUGARU_SOURCE_DIR}/externals/yspng ${CMAKE_BINARY_DIR}/yspng)

set(MUTSU_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

add_subdirectory(${MUTSU_SOURCE_DIR}/mc6809 mc6809)
add_subdirectory(${MUTSU_SOURCE_DIR}/mc6809util mc6809util)
add_subdirectory(${MUTSU_SOURCE_DIR}/memory memory)
add_subdirectory(${MUTSU_SOURCE_DIR}/fm77av fm77av)
add_subdirectory(${MUTSU_SOURCE_DIR}/main_cui main_cui)
add_subdirectory(${MUTSU_SOURCE_DIR}/miscutil miscutil)
add_subdirectory(${MUTSU_SOURCE_DIR}/ay38910 ay38910)
add_subdirectory(${MUTSU_SOURCE_DIR}/ym2203c ym2203c)
add_subdirectory(${MUTSU_SOURCE_DIR}/tests tests)
add_subdirectory(${MUTSU_SOURCE_DIR}/resources resources)
add_subdirectory(${MUTSU_SOURCE_DIR}/outside_world outside_world)
add_subdirectory(${MUTSU_SOURCE_DIR}/externals externals)

add_subdirectory(main_gui)
