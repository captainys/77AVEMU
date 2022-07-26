add_library(fm77av40dmac fm77av40dmac.cpp fm77av40dmac.h)
target_include_directories(fm77av40dmac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fm77av40dmac fm77av fm77avdef device)
