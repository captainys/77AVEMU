set(TARGET_NAME mc6809)
add_library(${TARGET_NAME} mc6809.h mc6809.cpp mc6809debugger.cpp)
target_link_libraries(${TARGET_NAME} memory)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
