add_library(memory memory.h memory.cpp)
target_link_libraries(memory fm77av device mc6809util)
target_include_directories(memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



add_library(fm77avmemory fm77avmemory.h fm77avmemory.cpp)
target_link_libraries(fm77avmemory memory)
target_include_directories(fm77avmemory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



add_library(plainmemory plainmemory.h plainmemory.cpp)
target_link_libraries(plainmemory memory)
target_include_directories(plainmemory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
