set(TARGET_NAME fm77avcuithread)
add_library(${TARGET_NAME} fm77avcuithread.cpp fm77avcuithread.h)
target_link_libraries(${TARGET_NAME} fm77av fm77avthread fm77avparam)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
