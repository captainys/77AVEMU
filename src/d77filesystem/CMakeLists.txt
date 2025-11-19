add_library(d77filesystem d77filesystem.h d77filesystem.cpp)
target_link_libraries(d77filesystem d77 fm7lib)
target_include_directories(d77filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
