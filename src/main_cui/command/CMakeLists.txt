set(TARGET_NAME fm77avcommand)
add_library(${TARGET_NAME} fm77avcommand.cpp fm77avcommand.h)
target_link_libraries(${TARGET_NAME} fm77av fm77avthread fm77avparam)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
