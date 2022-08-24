set(TARGET_NAME exas_compiler)
add_library(${TARGET_NAME} exas_compiler.cpp exas_compiler.h)
#target_link_libraries(${TARGET_NAME} cpputil)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
