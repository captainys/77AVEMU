add_executable(test_disasm disasmtest.cpp)
target_link_libraries(test_disasm mc6809 plainmemory cpputil)
