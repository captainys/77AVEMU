set(TARGET_NAME fm7lib)
add_library(${TARGET_NAME} fm7lib.cpp fm7lib.h cpplib.cpp cpplib.h)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} cpputil)
