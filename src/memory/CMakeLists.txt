set(TARGET_NAME memory)
add_library(${TARGET_NAME} memory.h memory.cpp)
target_link_libraries(${TARGET_NAME} fm77av device mc6809util)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
