set(TARGET_NAME fm77av)
add_library(${TARGET_NAME} fm77av.h fm77av.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} device cpputil mc6809 mc6809util vmbase)



add_library(fm77avthread fm77avthread.cpp fm77avthread.h)
target_link_libraries(fm77avthread fm77av)
target_include_directories(fm77avthread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
