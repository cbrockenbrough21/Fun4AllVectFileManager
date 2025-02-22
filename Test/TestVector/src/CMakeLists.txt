# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(vector_rewrite_example CXX)

# Include the source and header files
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB dist_headers ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB non_dist_headers ${PROJECT_SOURCE_DIR}/*LinkDef.h)
list(REMOVE_ITEM dist_headers ${non_dist_headers})

# ROOT dictionary generation
add_custom_command (
  OUTPUT vector_rewrite_example_Dict.cc
  COMMAND rootcint
  ARGS -f vector_rewrite_example_Dict.cc -c
    -I$ENV{OFFLINE_MAIN}/include/ -I${ROOT_PREFIX}/include/
    ${dist_headers}
    ${PROJECT_SOURCE_DIR}/LinkDef.h
  DEPENDS ${dist_headers} ${PROJECT_SOURCE_DIR}/LinkDef.h
)

# Find ROOT
find_program(ROOTCONF "root-config")
if(ROOTCONF)
  message("-- Detecting ROOT: found at ${ROOTCONF}")
else()
  message(FATAL_ERROR "-- Detecting ROOT: not found")
endif()

# Fetch ROOT configuration details
execute_process(COMMAND root-config --prefix OUTPUT_VARIABLE ROOT_PREFIX  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs   OUTPUT_VARIABLE ROOT_LINK    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Set C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -I$ENV{OFFLINE_MAIN}/include/ ${ROOT_CFLAGS}")

# Build the shared library
add_library(vector_rewrite_example SHARED ${sources} vector_rewrite_example_Dict.cc)
target_link_libraries(vector_rewrite_example)

# Add the executable target for VectorRewrite.cc
add_executable(VectorRewrite VectorRewrite.cc)  # Specify your executable name and source file
target_link_libraries(VectorRewrite vector_rewrite_example ${ROOT_LINK})  # Link against your library and ROOT

# Install the executable
install(TARGETS VectorRewrite DESTINATION ${PROJECT_SOURCE_DIR}/../maker)

# Install the library
install(TARGETS vector_rewrite_example DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# Install the pcm files if using ROOT 6
execute_process(COMMAND root-config --version OUTPUT_VARIABLE ROOT_VER)
string(SUBSTRING ${ROOT_VER} 0 1 ROOT_VER)
if (ROOT_VER GREATER 5)
   add_custom_target(install_pcm ALL COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/lib COMMAND cp -up *_rdict.pcm ${CMAKE_INSTALL_PREFIX}/lib)
   add_dependencies(install_pcm vector_rewrite_example)
endif()
