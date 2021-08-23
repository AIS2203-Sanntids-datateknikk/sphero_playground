cmake_minimum_required(VERSION 3.10)
project(sphero_playground)

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (MSVC)
  add_compile_definitions("_WIN32_WINNT=0x0601")
endif()

add_compile_definitions("_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING")

# ==============================================================================
# Dependencies
# ==============================================================================

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
          "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES
        opencv/4.5.2
        boost/1.71.0
        thrift/0.14.1
        libcurl/7.75.0
        paho-mqtt-cpp/1.2.0
        nlohmann_json/3.9.1
        # conflict resolution
        openssl/1.1.1k
        GENERATORS
        cmake
        IMPORTS "res, haarcascades* -> cascades"
        IMPORTS "res, lbpcascades* -> cascades"
        BASIC_SETUP CMAKE_TARGETS)


file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(examples)

