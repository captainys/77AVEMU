add_library(fm77avkeyboard fm77avkeyboard.h fm77avkeyboard.cpp fm77avrkana.h fm77avrkana.cpp)
target_link_libraries(fm77avkeyboard fm77av fm77avkey memory cpputil fm77avdef)
target_include_directories(fm77avkeyboard PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
