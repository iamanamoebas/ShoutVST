project (ShoutVST)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../deps/FLTK)

include_directories (${CMAKE_CURRENT_BINARY_DIR}/../deps/FLTK)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../FLUID)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/..)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../ SOURCES)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../FLUID SOURCES)

add_library(${PROJECT_NAME}  SHARED ${SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} VST_SDK shout ogg vorbis mp3lame fltk)