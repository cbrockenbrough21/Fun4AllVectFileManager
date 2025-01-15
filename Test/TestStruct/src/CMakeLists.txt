# Minimum CMake version required
cmake_minimum_required(VERSION 3.0)

# Project name
project(StructRewrite CXX)

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
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CFLAGS}")

# Include path for StructDef.h
include_directories(/project/ptgroup/Catherine/Fun4AllVectFileManager/StructDef/src)

# Link path for libStruct_Def.so
link_directories(/project/ptgroup/Catherine/Fun4AllVectFileManager/StructDef/inst/lib)

# Build StructRewrite executable
add_executable(StructRewrite StructRewrite.cc)
target_link_libraries(StructRewrite Struct_Def ${ROOT_LINK})

# Install the executable
install(TARGETS StructRewrite DESTINATION ${PROJECT_SOURCE_DIR}/../maker)
