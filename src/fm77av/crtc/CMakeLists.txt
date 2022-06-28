add_library(fm77avcrtc fm77avcrtc.h fm77avcrtc.cpp)
target_link_libraries(fm77avcrtc fm77av memory cpputil fm77avdef)
target_include_directories(fm77avcrtc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
