set(TARGET_NAME png2cpp)
add_executable(${TARGET_NAME} png2cpp.cpp)
target_link_libraries(${TARGET_NAME} yspng cpputil)


set(ICON_SOURCES
	../outside_world/icons.h
	../outside_world/icons.cpp
)

set(PNGS
	FD_BUSY.png
	FD_IDLE.png
	INS.png
	KANA.png
	MENU.png
	PAUSE.png
	TAPE_IDLE.png
	TAPE_LOADING.png
	TAPE_SAVING.png
)

add_custom_command(
	OUTPUT ${ICON_SOURCES}
	DEPENDS ${PNGS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND echo ${YS_COMMAND_BINARY_DIR}/png2cpp
	COMMAND ${TARGET_NAME}
)

add_custom_target(ICONS DEPENDS ${ICON_SOURCES})
