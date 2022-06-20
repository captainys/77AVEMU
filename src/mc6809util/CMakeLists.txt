set(TARGET_NAME mc6809util)
add_library(${TARGET_NAME} mc6809util.h mc6809util.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
