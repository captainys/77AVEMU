add_library(fm77avserialport fm77avserialport.h fm77avserialport.cpp)
target_link_libraries(fm77avserialport device cpputil fm77av fm77avdef i8251)
target_include_directories(fm77avserialport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
