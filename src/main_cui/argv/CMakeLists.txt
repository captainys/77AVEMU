set(TARGET_NAME fm77avargv)
add_library(${TARGET_NAME} fm77avargv.cpp fm77avargv.h)
target_link_libraries(${TARGET_NAME} fm77avparam)
