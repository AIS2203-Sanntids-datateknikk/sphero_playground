
set(generatedSourcesDir "${CMAKE_BINARY_DIR}/generated")
set(thriftCppSourcesDir "${generatedSourcesDir}/thrift")
set(thriftPythonSourcesDir "${CMAKE_CURRENT_SOURCE_DIR}/python")

file(MAKE_DIRECTORY "${generatedSourcesDir}")
file(MAKE_DIRECTORY "${thriftCppSourcesDir}")
file(MAKE_DIRECTORY "${thriftPythonSourcesDir}")

find_program(THRIFT_EXECUTABLE NAMES thrift)
if (NOT THRIFT_EXECUTABLE)
    message(FATAL_ERROR "The thrift compiler was not found. Cannot generate Thrift sources.")
endif ()

execute_process(
        COMMAND
        "${THRIFT_EXECUTABLE}"
        "--gen" "cpp:no_skeleton"
        "--out" "${thriftCppSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/image_service.thrift"
)

execute_process(
        COMMAND
        "${THRIFT_EXECUTABLE}"
        "--gen" "py"
        "--out" "${thriftPythonSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/image_service.thrift"
)

set(thriftGenerated
        "${thriftCppSourcesDir}/CameraService.h"
        "${thriftCppSourcesDir}/CameraService.cpp"
        "${thriftCppSourcesDir}/image_service_types.h"
        )

add_library(camera-service OBJECT ${thriftGenerated})
target_link_libraries(camera-service PUBLIC CONAN_PKG::thrift)
list(APPEND generatedFiles "$<TARGET_OBJECTS:camera-service>")


add_executable(client_desktop client_desktop.cpp ${generatedFiles})
target_include_directories(client_desktop
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${generatedSourcesDir}")
target_link_libraries(client_desktop
        PUBLIC
        CONAN_PKG::thrift
        CONAN_PKG::opencv)