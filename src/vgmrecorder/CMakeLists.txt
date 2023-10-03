# Re-use code from Tsugaru
set(TARGET_NAME vgmrecorder)
set(VGMRECORDER_DIR "${TSUGARU_SOURCE_DIR}/vgmrecorder")
add_compile_definitions(MUTSU_FM77AV)
add_library(${TARGET_NAME} ${VGMRECORDER_DIR}/vgmrecorder.h ${VGMRECORDER_DIR}/vgmrecorder.cpp ${VGMRECORDER_DIR}/vgmrecorder_ym2203.cpp ${VGMRECORDER_DIR}/vgmrecorder_ay8910.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${VGMRECORDER_DIR})
target_link_libraries(${TARGET_NAME} ym2203c)
