set(TARGET_NAME sound)
add_library(${TARGET_NAME} sound.h sound.cpp)
target_link_libraries(${TARGET_NAME} cpputil device ay38910)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
