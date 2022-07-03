set(TARGET_NAME ay38910)
add_library(${TARGET_NAME} ay38910.h ay38910.cpp)
target_link_libraries(${TARGET_NAME} cpputil)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
