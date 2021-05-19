
set(generatedSourcesDir "${CMAKE_BINARY_DIR}/generated")
set(thriftCppSourcesDir "${generatedSourcesDir}/thrift")

file(MAKE_DIRECTORY "${generatedSourcesDir}")
file(MAKE_DIRECTORY "${thriftCppSourcesDir}")

find_program(THRIFT_EXECUTABLE NAMES thrift)
if (NOT THRIFT_EXECUTABLE)
    message(FATAL_ERROR "The thrift compiler was not found. Cannot generate Thrift sources.")
endif ()

execute_process(
        COMMAND
        "${THRIFT_EXECUTABLE}"
        "--gen" "cpp:no_skeleton"
        "--out" "${thriftCppSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/service.thrift"
)

set(thriftGenerated
        "${thriftCppSourcesDir}/ImageService.h"
        "${thriftCppSourcesDir}/ImageService.cpp"
        "${thriftCppSourcesDir}/service_types.h"
        "${thriftCppSourcesDir}/service_types.cpp"
        )

add_library(image-service OBJECT ${thriftGenerated})
target_link_libraries(image-service PUBLIC CONAN_PKG::thrift)
list(APPEND generatedFiles "$<TARGET_OBJECTS:image-service>")

set(sources
        server.cpp
        "ImageServiceHandler.hpp"
        "ImageServiceHandler.cpp"
        )

add_executable(thrift_image_server ${sources} ${generatedFiles})
target_include_directories(thrift_image_server
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${generatedSourcesDir}")
target_link_libraries(thrift_image_server
        PUBLIC
        CONAN_PKG::opencv
        CONAN_PKG::thrift)

