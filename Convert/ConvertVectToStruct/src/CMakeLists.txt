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
set(CMAKE_INSTALL_RPATH "/project/ptgroup/Catherine/Fun4AllVectFileManager/StructDef/inst/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# 3) Where to find StructDef headers
include_directories(${CMAKE_SOURCE_DIR}/../../../StructDef/src)

# 4) Link to StructDef shared library
link_directories(${CMAKE_SOURCE_DIR}/../../../StructDef/inst/lib)

# 5) Specify dependencies for dictionary files
find_file(STRUCT_DEF_PCM
  StructDef_Dict_rdict.pcm
  PATHS /project/ptgroup/Catherine/Fun4AllVectFileManager/StructDef/inst/lib
)

if(NOT STRUCT_DEF_PCM)
  message(FATAL_ERROR "StructDef_Dict_rdict.pcm not found. Ensure StructDef is built correctly.")
endif()

# Install the dictionary file
install(FILES ${STRUCT_DEF_PCM} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# 6) Build the ConvertVectToStruct executable
add_executable(ConvertVectToStruct ConvertVectToStruct.C)

# Link to required libraries
target_link_libraries(ConvertVectToStruct
  Struct_Def
  ${ROOT_LINK}
  fun4all
  interface_main
  geom_svc
  ktracker
  phg4hit
  PHPythia8
  g4detectors
  UtilAna
)

# 7) Install the executable
install(TARGETS ConvertVectToStruct DESTINATION ${PROJECT_SOURCE_DIR}/../maker)
