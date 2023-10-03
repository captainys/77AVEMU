set(TARGET_NAME fm77avsound)
add_library(${TARGET_NAME} fm77avsound.h fm77avsound.cpp)
target_link_libraries(${TARGET_NAME} cpputil device vgmrecorder ay38910 ym2203c fm77av fm77avdef yssimplesound)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
