cmake_minimum_required(VERSION 3.10)
project(OpenCV__)

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_STANDARD 17)

# ==============================================================================
# Dependencies
# ==============================================================================

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
else ()
  message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first!")
endif ()

file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/images)

add_subdirectory(examples)

