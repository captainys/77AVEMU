set(TARGET_NAME fm77avcommand)
add_library(${TARGET_NAME} fm77avcommand.cpp fm77avcommand.h fm77avlineparser.cpp fm77avlineparser.h)
target_link_libraries(${TARGET_NAME} fm77av fm77avthread fm77avparam lineParser cpputil)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
