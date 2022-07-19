set(TARGET_NAME fm77aveventlog)
add_library(${TARGET_NAME} fm77aveventlog.h fm77aveventlog.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} fm77av fm77avkey fm77avkeyboard cpputil)
