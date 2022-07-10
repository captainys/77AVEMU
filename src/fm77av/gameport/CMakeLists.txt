add_library(fm77avgameport fm77avgameport.h fm77avgameport.cpp)
target_link_libraries(fm77avgameport device cpputil fm77av fm77avdef cheapmath)
target_include_directories(fm77avgameport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
