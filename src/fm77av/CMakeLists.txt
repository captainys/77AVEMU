set(TARGET_NAME fm77av)
add_library(${TARGET_NAME} fm77av.h fm77av.cpp fm77avio.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} fm77avdef fm77avmemory device cpputil mc6809 mc6809util vmbase)



add_library(fm77avdef fm77avdef.cpp fm77avdef.h)
target_include_directories(fm77avdef PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



add_library(fm77avthread fm77avthread.cpp fm77avthread.h)
target_link_libraries(fm77avthread fm77avdef fm77av)
target_include_directories(fm77avthread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



add_library(fm77avparam fm77avparam.cpp fm77avparam.h)
target_link_libraries(fm77avparam fm77avdef fm77av)
target_include_directories(fm77avparam PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
