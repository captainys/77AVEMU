add_subdirectory(crtc)
add_subdirectory(fdc)
add_subdirectory(datarecorder)
add_subdirectory(keyboard)
add_subdirectory(sound)
add_subdirectory(gameport)
add_subdirectory(eventlog)
add_subdirectory(serialport)
add_subdirectory(dma)



set(TARGET_NAME fm77av)
add_library(${TARGET_NAME} fm77av.h fm77av.cpp fm77avint.cpp fm77avio.cpp fm77avstate.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} outside_world fm77aveventlog fm77avdef fm77avmemory fm77avrender fm77avcrtc fm77avfdc fm77avsound fm77avkeyboard fm77avkey fm77avgameport fm77avdatarecorder fm77avserialport fm77av40dmac vgmrecorder ym2203c device cpputil mc6809 mc6809util vmbase lineParser)
# lineparser is for MemoryEvaluation class.



add_library(fm77avdef fm77avdef.cpp fm77avdef.h)
target_include_directories(fm77avdef PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



add_library(fm77avthread fm77avthread.cpp fm77avthread.h)
target_link_libraries(fm77avthread fm77avdef fm77av fm77avcommand outside_world)
target_include_directories(fm77avthread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(UNIX)
    target_link_libraries(fm77avthread pthread)
endif()



add_library(fm77avparam fm77avparam.cpp fm77avparam.h)
target_link_libraries(fm77avparam fm77avdef fm77avkey fm77av cpputil)
target_include_directories(fm77avparam PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



add_library(fm77avrender fm77avrender.h fm77avrender.cpp)
target_link_libraries(fm77avrender fm77av fm77avcrtc memory cpputil fm77avdef)
target_include_directories(fm77avrender PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



add_library(fm77avkey fm77avkey.h fm77avkey.cpp)
target_link_libraries(fm77avkey cpputil)
