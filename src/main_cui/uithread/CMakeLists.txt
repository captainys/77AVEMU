set(TARGET_NAME fm77avcuithread)
add_library(${TARGET_NAME} fm77avcuithread.cpp fm77avcuithread.h)
target_link_libraries(${TARGET_NAME} fm77av fm77avthread fm77avparam)
