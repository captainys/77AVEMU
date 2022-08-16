set(TARGET_NAME t77lib)
add_library(${TARGET_NAME} t77.cpp t77.h)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} cpputil fm7lib)
