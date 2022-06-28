add_subdirectory(argv)
add_subdirectory(command)
add_subdirectory(cuithread)


set(TARGET_NAME Mutsu_CUI)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} fssimplewindow_connection fm77av fm77avthread fm77avdef fm77avargv fm77avcommand fm77avcuithread)
