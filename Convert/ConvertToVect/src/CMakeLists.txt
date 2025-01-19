cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ConvertToVect CXX)

# 1) Find ROOT
find_program(ROOTCONF "root-config")
if(NOT ROOTCONF)
  message(FATAL_ERROR "-- Detecting ROOT: not found")
else()
  message("-- Detecting ROOT: found at ${ROOTCONF}")
endif()

execute_process(COMMAND root-config --prefix OUTPUT_VARIABLE ROOT_PREFIX  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LINK OUTPUT_STRIP_TRAILING_WHITESPACE)

# 2) Set C++ flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CFLAGS}")

# # Include directories for headers
# include_directories(/project/ptgroup/Catherine/Fun4AllVectFileManager/Fun4AllVectFileManager/src)

# # Add library paths
# link_directories(/project/ptgroup/spinquest/lib)

# Build the executable
add_executable(ConvertToVect ConvertToVect.C)
target_link_libraries(fun4all_vect_file_manager fun4all libcalibrator libktracker ${ROOT_LINK})

# Install the executable
install(TARGETS ConvertToVect DESTINATION ${PROJECT_SOURCE_DIR}/../maker)
