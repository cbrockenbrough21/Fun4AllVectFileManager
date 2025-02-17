# Minimum required CMake version
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Project name
project(VectorRewrite CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find ROOT
find_package(ROOT REQUIRED COMPONENTS Core RIO Tree Hist Physics)

# Include ROOT directories
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})  # Ensure source headers are included
include_directories(${PROJECT_SOURCE_DIR})  # Include project directory
include_directories(${CMAKE_SOURCE_DIR}/include)  # If headers are in include/
include_directories(${CMAKE_SOURCE_DIR}/src)  # If headers are in src/

# Source files
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB linkdef_headers ${PROJECT_SOURCE_DIR}/*LinkDef.h)
list(REMOVE_ITEM headers ${linkdef_headers})  # Remove LinkDef from headers

# Generate ROOT Dictionary
ROOT_GENERATE_DICTIONARY(vector_rewrite_example_Dict VectorRewrite.h LINKDEF LinkDef.h)

# Create shared library
add_library(vector_rewrite_example SHARED ${sources} vector_rewrite_example_Dict.cxx)
target_link_libraries(vector_rewrite_example PRIVATE ${ROOT_LIBRARIES})

# Ensure proper compilation options for the library
target_compile_options(vector_rewrite_example PRIVATE -std=c++17)

# Add executable
add_executable(VectorRewrite VectorRewrite.cc)
target_link_libraries(VectorRewrite PRIVATE vector_rewrite_example ${ROOT_LIBRARIES})

# Install executable
install(TARGETS VectorRewrite DESTINATION ${PROJECT_SOURCE_DIR}/../maker)

# Install shared library
install(TARGETS vector_rewrite_example DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# Install the pcm files if using ROOT 6
execute_process(COMMAND root-config --version OUTPUT_VARIABLE ROOT_VER)
string(SUBSTRING ${ROOT_VER} 0 1 ROOT_VER)
if (ROOT_VER GREATER 5)
   add_custom_target(install_pcm ALL COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/lib COMMAND cp -up *_rdict.pcm ${CMAKE_INSTALL_PREFIX}/lib)
add_dependencies(install_pcm vector_rewrite_example)
endif()
