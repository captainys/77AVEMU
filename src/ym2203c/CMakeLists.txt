# Re-use code from Tsugaru
set(TARGET_NAME ym2203c)
set(YM2203C_DIR "${TSUGARU_SOURCE_DIR}/ym2612")
add_compile_definitions(FM77AV)
add_library(${TARGET_NAME} ${YM2203C_DIR}/ym2612.h ${YM2203C_DIR}/ym2612.cpp ${YM2203C_DIR}/ym2612wave.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${YM2203C_DIR})
