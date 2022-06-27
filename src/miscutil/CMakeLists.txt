add_library(miscutil miscutil.cpp miscutil.h)
target_link_libraries(miscutil mc6809 memory)
target_include_directories(cpputil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
