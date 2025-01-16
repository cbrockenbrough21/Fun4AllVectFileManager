cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ConvertVectToStruct CXX)

# 1) Find ROOT
find_program(ROOTCONF "root-config")
if(NOT ROOTCONF)
  message(FATAL_ERROR "-- Detecting ROOT: not found")
else()
  message("-- Detecting ROOT: found at ${ROOTCONF}")
endif()

execute_process(COMMAND root-config --prefix OUTPUT_VARIABLE ROOT_PREFIX  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs   OUTPUT_VARIABLE ROOT_LINK    OUTPUT_STRIP_TRAILING_WHITESPACE)

# 2) Set C++ flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CFLAGS}")

# 3) Where to find StructDef.h
include_directories(${CMAKE_SOURCE_DIR}/../../../StructDef/src)

# 4) Where to find libStruct_Def.so (or similar)
link_directories(${CMAKE_SOURCE_DIR}/../../../StructDef/inst/lib)

# 5) Build the single executable
add_executable(ConvertVectToStruct ConvertVectToStruct.cc)
target_link_libraries(ConvertVectToStruct Struct_Def ${ROOT_LINK}
  fun4all
  interface_main
  geom_svc
  ktracker
  phg4hit
  PHPythia8
  g4detectors
  UtilAna
)

# 6) (Optional) Install the executable
install(TARGETS ConvertVectToStruct DESTINATION ${PROJECT_SOURCE_DIR}/../maker)