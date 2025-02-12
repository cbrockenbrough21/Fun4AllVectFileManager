cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ConvertVectToStruct CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find ROOT using root-config
find_program(ROOTCONF "root-config")
if(NOT ROOTCONF)
  message(FATAL_ERROR "-- Detecting ROOT: not found")
else()
  message("-- Detecting ROOT: found at ${ROOTCONF}")
endif()

# Get ROOT prefix, cflags, and libs
execute_process(COMMAND root-config --prefix OUTPUT_VARIABLE ROOT_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LINK OUTPUT_STRIP_TRAILING_WHITESPACE)

# ✅ Include ROOT Headers
include_directories(${ROOT_PREFIX}/include)

# ✅ Set the correct compiler flags for ROOT
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

# ✅ Build the ConvertVectToStruct executable
add_executable(ConvertVectToStruct ConvertVectToStruct.C)

# ✅ Link ConvertVectToStruct to ROOT and other dependencies
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

# ✅ Install the executable
install(TARGETS ConvertVectToStruct DESTINATION ${PROJECT_SOURCE_DIR}/../maker)
