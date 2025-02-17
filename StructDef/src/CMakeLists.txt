# Minimum CMake version required
cmake_minimum_required(VERSION 3.0)

# Project name
project(Struct_Def CXX)

# Find ROOT
find_program(ROOTCONF "root-config")
if (ROOTCONF)
    message("-- Detecting ROOT: found at ${ROOTCONF}")
else ()
    message(FATAL_ERROR "-- Detecting ROOT: not found")
endif()

# Fetch ROOT configuration details
execute_process(COMMAND root-config --prefix OUTPUT_VARIABLE ROOT_PREFIX  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs   OUTPUT_VARIABLE ROOT_LINK    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Set C++ flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CFLAGS}")

# Include paths
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Generate ROOT dictionary
set(DICTIONARY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/StructDef.h
    ${CMAKE_CURRENT_SOURCE_DIR}/StructDef_LinkDef.h
)
add_custom_command(
    OUTPUT StructDef_Dict.cpp StructDef_rdict.pcm
    COMMAND rootcint -f StructDef_Dict.cpp -c ${DICTIONARY_SOURCES}
    DEPENDS ${DICTIONARY_SOURCES}
)

# Build the shared library
set(SOURCES StructDef.cc StructDef_Dict.cpp)
add_library(Struct_Def SHARED ${SOURCES})
target_link_libraries(Struct_Def ${ROOT_LINK})

# Install header files and library
install(FILES StructDef.h DESTINATION include)
install(TARGETS Struct_Def DESTINATION lib)

# Install the PCM file for ROOT 6
execute_process(COMMAND root-config --version OUTPUT_VARIABLE ROOT_VER)
string(SUBSTRING ${ROOT_VER} 0 1 ROOT_VER)
if (ROOT_VER GREATER 5)
    install(FILES ${CMAKE_BINARY_DIR}/StructDef_Dict_rdict.pcm DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()
