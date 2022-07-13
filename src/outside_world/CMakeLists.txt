add_library(outside_world outside_world.h outside_world.cpp icons.h icons.cpp)
target_include_directories(outside_world PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(outside_world fm77av fm77avdef fm77avparam fm77avrender)
