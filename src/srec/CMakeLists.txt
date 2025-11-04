add_library(srec srec.h srec.cpp)
target_include_directories(srec PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(srecdump srecdump.cpp)
target_link_libraries(srecdump srec)
