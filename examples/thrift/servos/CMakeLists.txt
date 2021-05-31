
set(generatedSourcesDir "${CMAKE_BINARY_DIR}/generated")
set(thriftCppSourcesDir "${generatedSourcesDir}/thrift")
set(thriftPythonSourcesDir "${CMAKE_CURRENT_SOURCE_DIR}/python")

file(MAKE_DIRECTORY "${generatedSourcesDir}")
file(MAKE_DIRECTORY "${thriftCppSourcesDir}")
file(MAKE_DIRECTORY "${thriftPythonSourcesDir}")

execute_process(
        COMMAND
        "${THRIFT_EXECUTABLE}"
        "--gen" "cpp:no_skeleton"
        "--out" "${thriftCppSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/servo_service.thrift"
)

execute_process(
        COMMAND
        "${THRIFT_EXECUTABLE}"
        "--gen" "py"
        "--out" "${thriftPythonSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/servo_service.thrift"
)

set(thriftGenerated
        "${thriftCppSourcesDir}/ServoService.h"
        "${thriftCppSourcesDir}/ServoService.cpp"
        "${thriftCppSourcesDir}/servo_service_types.h"
        )

add_library(servo-service OBJECT ${thriftGenerated})
target_link_libraries(servo-service PUBLIC CONAN_PKG::thrift)
list(APPEND generatedFiles "$<TARGET_OBJECTS:servo-service>")

add_executable(servo_client_test servo_client_test.cpp ${generatedFiles})
target_include_directories(servo_client_test
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${generatedSourcesDir}")
target_link_libraries(servo_client_test
        PUBLIC
        CONAN_PKG::thrift
        CONAN_PKG::opencv)
