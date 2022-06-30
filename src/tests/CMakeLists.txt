add_executable(test_disasm disasmtest.cpp)
target_link_libraries(test_disasm mc6809 plainmemory cpputil)
add_test(NAME 6809disasm COMMAND test_disasm)

add_executable(test_division divisiontest.cpp)
target_link_libraries(test_division mc6809 plainmemory cpputil)
add_test(NAME 6809division COMMAND test_division)
