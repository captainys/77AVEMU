add_library(fm77avfdc fm77avfdc.h fm77avfdc.cpp)
target_link_libraries(fm77avfdc fm77av diskdrive memory cpputil fm77avdef)
target_include_directories(fm77avfdc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
